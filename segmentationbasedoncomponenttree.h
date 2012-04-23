#ifndef SEGMENTATIONBASEDONCOMPONENTTREE_H
#define SEGMENTATIONBASEDONCOMPONENTTREE_H

#include "include/Image.h"
#include "include/ComponentTree.h"
#include "timer.h"

using namespace LibTIM;

class QImage;

class SegmentationBasedOnComponentTree
{
public:
    SegmentationBasedOnComponentTree(unsigned int width, unsigned int height);
    ~SegmentationBasedOnComponentTree();

    void Compute (double alpha, QImage &input, QImage &marker, QImage &segmentationResult, Timer timers[]);
    void Compute (double alpha, QImage &input, QImage &marker, QImage &segmentationResult, QImage &contour, Timer timers[]);
    void DoSegmentationWithPreviousComponentTree (double alpha, QImage &segmentationResult, Timer timers[]);
    void SetTransparency(int transparency);

protected:
    void convertQImageToLibTIM (QImage &qImage, Image<U8> &image);
    void convertLibTIMToQImage(Image<U8> &image, QImage &qImage, int transparency);
    //Image<U8> convertQImageToLibTIM (QImage *img);
    //QImage *convertLibTIMToQImage(Image<U8> &image, int alpha);

    double computeCAlpha (Node *n, double alpha);
    void computeResult(double alpha);
    Image<U8> drawContourN4 (const Image <U8> &mask, const U8 val);

    unsigned int width;
    unsigned int height;

    unsigned int transparency;

    ComponentTree<U8> *tree;
    vector<Node *> leafs;

    Image<U8> inputImage;
    Image<U8> markerImage;
    Image<U8> resultImage;
    Image<U8> contourImage;
};

#endif // SEGMENTATIONBASEDONCOMPONENTTREE_H
