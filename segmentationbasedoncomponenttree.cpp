#include "segmentationbasedoncomponenttree.h"
#include <QImage>


SegmentationBasedOnComponentTree::SegmentationBasedOnComponentTree (unsigned int width, unsigned int height){
    this -> width = width;
    this -> height = height;

    transparency = 100;

    inputImage = Image<U8> (width, height);
    markerImage = Image<U8> (width, height);
    resultImage = Image<U8> (width, height);
    contourImage = Image<U8> (width, height);

    tree = 0;
}


SegmentationBasedOnComponentTree::~SegmentationBasedOnComponentTree(){
    if (tree)
        delete tree;
}

void SegmentationBasedOnComponentTree::DoSegmentationWithPreviousComponentTree (double alpha, QImage &segmentationResult, Timer timers[]){
    timers[3].StartCycle ();
    tree -> m_root -> calpha = computeCAlpha (tree -> m_root, alpha);
    timers[3].StopCycle ();

    timers[4].StartCycle ();
    computeResult (alpha);
    timers[4].StopCycle ();

    timers[5].StartCycle ();
    convertLibTIMToQImage (resultImage, segmentationResult, transparency);
    timers[5].StopCycle ();
}


void SegmentationBasedOnComponentTree::Compute(double alpha, QImage &input, QImage &marker, QImage &segmentationResult, Timer timers[]){
    timers[0].StartCycle ();
    convertQImageToLibTIM (input, inputImage);
    timers[0].StopCycle ();

    timers[1].StartCycle ();
    convertQImageToLibTIM (marker, markerImage);
    timers[1].StopCycle ();

    timers[9].StartCycle ();
    if (tree)
        delete tree;
    timers[9].StopCycle ();

    timers[2].StartCycle ();
    tree = new ComponentTree<U8> (inputImage, markerImage);
    timers[2].StopCycle ();

    DoSegmentationWithPreviousComponentTree (alpha, segmentationResult, timers);
}


void SegmentationBasedOnComponentTree::Compute(double alpha, QImage &input, QImage &marker, QImage &segmentationResult, QImage &contour, Timer timers[]){
    Compute(alpha, input, marker, segmentationResult, timers);

    contourImage = drawContourN4 (resultImage, 255);
    convertLibTIMToQImage (contourImage, contour, 255);
}


double SegmentationBasedOnComponentTree::computeCAlpha (Node *n, double alpha){
    double res = 0.0;
    double sum = 0.0;

    double exprl = alpha * n -> n;

    for (unsigned int i = 0; i < n -> childs.size (); i++)
        sum += computeCAlpha (n -> childs[i], alpha);

    double exprr = (1 - alpha) * n -> ps + sum;

    if (exprl < exprr)
        res = exprl;
    else
        res = exprr;

    n -> calpha = res;
    return res;
}


void SegmentationBasedOnComponentTree::computeResult (double alpha){
    double sum = 0.0;
    double exprl, exprr;

    //resultImage = Image<U8> (width, height);
    resultImage.fill (0);

    vector<Node *> selectedNodes;
    std::queue<Node *> fifo;
    fifo.push (tree -> m_root);

    while (!fifo.empty ()){
        Node *tmp = fifo.front ();
        fifo.pop ();

        exprl = alpha * tmp -> n;
        sum = 0.0;

        for (unsigned int i = 0; i < tmp -> childs.size (); i++)
            sum += tmp -> childs[i] -> calpha;
        exprr = (1 - alpha) * tmp -> ps + sum;

        if (exprl < exprr)
            selectedNodes.push_back (tmp);
        else
            for (unsigned int i = 0; i < tmp -> childs.size (); i++)
                fifo.push (tmp -> childs[i]);
    }

    for (unsigned int i = 0; i < selectedNodes.size (); i++)
        tree -> constructNode (resultImage, selectedNodes[i]);
}


Image<U8> SegmentationBasedOnComponentTree::drawContourN4 (const Image <U8> &mask, const U8 val){
    int dx = mask.getSizeX ();
    int dy = mask.getSizeY ();
    int dz = mask.getSizeZ ();
    Image<U8> res (dx, dy, dz);
    res.fill (0);
    FlatSE nghb;
    nghb.make2DN4 ();

    for (int z = 0; z < dz; z++)
        for (int y = 0; y < dy; y++)
            for (int x = 0; x < dx; x++){
                if (mask (x, y, z) != 0)
                    for (int i = 0; i < nghb.getNbPoints (); i++){
                        Point <TCoord> p (x, y, z);
                        Point <TCoord> q = p + nghb.getPoint (i);
                        if (mask.isPosValid (q))
                            if (mask (q) == 0)
                                res (x, y, z) = val;
                    }
            }
    return res;
}


void SegmentationBasedOnComponentTree::convertLibTIMToQImage(Image<U8> &image, QImage &qImage, int transparency){
    unsigned int size = width * height;
    unsigned int *bits = (unsigned int *)qImage.bits ();
    for (unsigned int i = 0; i < size; i++){
        if (image (i) > 0)
            *bits = 255 | (transparency << 24);
        else
            *bits = 0;
        bits++;
    }
}


void SegmentationBasedOnComponentTree::convertQImageToLibTIM (QImage &qImage, Image<U8> &image){
    unsigned int size = width * height;
    unsigned char *bits = qImage.bits () + 2;
    for (unsigned int i = 0; i < size; i++){
        image (i) = *bits;
        bits += 4;
    }
}


/*Image<U8> SegmentationBasedOnComponentTree::convertQImageToLibTIM (QImage *image){
    Image<U8> result (width, height);
    unsigned int size = width * height;
    const unsigned int *bits = (unsigned int *)image -> constBits ();
    for (unsigned int i = 0; i < size; i++)
        result (i) = qRed(bits[i]);

//    Image<U8> result2 (WIDTH, HEIGHT);
//    for (unsigned int x = 0; x < WIDTH; x++)
//        for (unsigned int y = 0; y < HEIGHT; y++)
//            result2 (x, y) = qRed (image -> pixel (x, y));

//    for (unsigned int x = 0; x < WIDTH; x++)
//        for (unsigned int y = 0; y < HEIGHT; y++)
//            if (result (x, y) != result2 (x, y)){
//                qDebug () << x << ", " << y;
//                break;
//            }
    return result;
}*/


/*QImage *SegmentationBasedOnComponentTree::convertLibTIMToQImage (Image<U8> &image, int alpha){
    QImage *result = new QImage (width, height, QImage::Format_ARGB32_Premultiplied);
    unsigned int size = width * height;
    unsigned int *bits = (unsigned int *)result -> bits ();
    U8 value;
    for (unsigned int i = 0; i < size; i++){
        value = image (i);
        if (value > 0)
            bits[i] = qRgba (0, 0, 255, alpha);
        else
            bits[i] = qRgba (0, 0, 0, 0);
    }

//    QImage *result2 = new QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied);
//    for (unsigned int x = 0; x < WIDTH; x++)
//        for (unsigned int y = 0; y < HEIGHT; y++){
//            value = image (x, y);
//            if (value > 0)
//                result2 -> setPixel (x, y, qRgba (0, 0, 255, alpha));
//            else
//                result2 -> setPixel (x, y, qRgba (0, 0, 0, 0));
//        }

//    for (unsigned int x = 0; x < WIDTH; x++)
//        for (unsigned int y = 0; y < HEIGHT; y++)
//            if (result -> pixel (x, y) != result2 -> pixel (x, y)){
//                qDebug () << x << ", " << y;
//                break;
//            }
//    delete result2;
    return result;
}*/


void SegmentationBasedOnComponentTree::SetTransparency(int transparency){
    this -> transparency = transparency;
}
