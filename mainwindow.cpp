#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QDebug>

#include "camerasupport.h"
#include "meaningfulscales.h"


MainWindow::MainWindow (QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), frame (0), contour (0), tree (0){
    ui->setupUi(this);

    connect (ui -> alphaDial, SIGNAL (valueChanged (int)), this, SLOT (alphaChanged (int)));
    connect (ui -> penSizeDial, SIGNAL (valueChanged (int)), this, SLOT (penSizeChanged (int)));
    connect (ui -> transparencyDial, SIGNAL (valueChanged (int)), this, SLOT (transparencyChanged (int)));

    ui -> alphaDial -> setValue (10);
    ui -> penSizeDial -> setValue (10);
    ui -> transparencyDial -> setValue (100);

    cameraSupport = new CameraSupport (WIDTH, HEIGHT);

    timer = new QTimer (this);
    connect (timer, SIGNAL(timeout()), this, SLOT(updateFrame()));
    timer -> start (MILISECONDS_FOR_REFRESH);

    totalFrames = 1000 / MILISECONDS_FOR_REFRESH;
    frameTime = new unsigned long long[totalFrames];
    for (unsigned int i = 0; i < totalFrames; i++)
        frameTime[i] = 0;
    currentFrame = 0;
    fps = 0;

    marker = new QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied);
    marker -> fill (qRgba (0, 0, 0, 0));

    segmentationResult = new QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied);
    segmentationResult -> fill (qRgba (0, 0, 0, 0));

    boxes = 0;

    repaint = true;

    frameCounter = 0;
    time1 = time2 = time3 = time4 = time5 = time6 = time7 = time8 = 0;
}


MainWindow::~MainWindow (){
    delete cameraSupport;

    if (tree)
        delete tree;

    if (frame != 0)
        delete frame;
    frame = 0;

    if (marker != 0)
        delete marker;
    marker = 0;

    delete ui;
}


void MainWindow::setOrientation(ScreenOrientation orientation)
{
#if defined(Q_OS_SYMBIAN)
    // If the version of Qt on the device is < 4.7.2, that attribute won't work
    if (orientation != ScreenOrientationAuto) {
        const QStringList v = QString::fromAscii(qVersion()).split(QLatin1Char('.'));
        if (v.count() == 3 && (v.at(0).toInt() << 16 | v.at(1).toInt() << 8 | v.at(2).toInt()) < 0x040702) {
            qWarning("Screen orientation locking only supported with Qt 4.7.2 and above");
            return;
        }
    }
#endif // Q_OS_SYMBIAN

    Qt::WidgetAttribute attribute;
    switch (orientation) {
#if QT_VERSION < 0x040702
    // Qt < 4.7.2 does not yet have the Qt::WA_*Orientation attributes
    case ScreenOrientationLockPortrait:
        attribute = static_cast<Qt::WidgetAttribute>(128);
        break;
    case ScreenOrientationLockLandscape:
        attribute = static_cast<Qt::WidgetAttribute>(129);
        break;
    default:
    case ScreenOrientationAuto:
        attribute = static_cast<Qt::WidgetAttribute>(130);
        break;
#else // QT_VERSION < 0x040702
    case ScreenOrientationLockPortrait:
        attribute = Qt::WA_LockPortraitOrientation;
        break;
    case ScreenOrientationLockLandscape:
        attribute = Qt::WA_LockLandscapeOrientation;
        break;
    default:
    case ScreenOrientationAuto:
        attribute = Qt::WA_AutoOrientation;
        break;
#endif // QT_VERSION < 0x040702
    };
    setAttribute(attribute, true);
}

void MainWindow::showExpanded()
{
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_SIMULATOR)
    showFullScreen();
#elif defined(Q_WS_MAEMO_5)
    showMaximized();
#else
    show();
#endif
}


void MainWindow::updateFrame (){
    clock_t start = clock ();
    bool result = cameraSupport -> UpdateFrame ();// = false
    clock_t stop = clock ();
    if (result){
        time8 += stop - start;

        if (frame != 0){
            delete frame;
            frame = 0;
        }
        frame = new QImage((unsigned char *)cameraSupport -> GetRGBA (), WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied);//Format_RGB888

        computeCT ();

        if (ui -> meaningulScalesCheckBox -> isChecked ()){
            time9 -= clock ();
            MeaningfulScales ms(*segmentationResult);
            time9 += clock ();
            if (boxes)
                delete boxes;
            boxes = new QImage (ms.GetResult());
        }

        repaint = true;
        update (0, 0, WIDTH, HEIGHT);

        frameTime[currentFrame] = clock();

        /*fps = 1;
        while (frameTime[currentFrame] - frameTime[(currentFrame - fps) % totalFrames] < CLOCKS_PER_SEC)
            fps++;*/

        fps++;
        while (frameTime[currentFrame] - frameTime[(currentFrame - fps) % totalFrames] > CLOCKS_PER_SEC)
            fps--;

        if (currentFrame < totalFrames - 1)
            currentFrame++;
        else
            currentFrame = 0;

        ui ->  fpsLabel -> setText (QString ("FPS: %1").arg (fps));
    }
}


void MainWindow::alphaChanged (int value){
    alpha = (double)value / 1000.0;
    ui ->  alphaLabel -> setText (QString ("Alpha: %1").arg (alpha));
}


void MainWindow::penSizeChanged (int value){
    ui ->  penSizeLabel -> setText (QString ("Pen size: %1").arg (value));
}


void MainWindow::transparencyChanged (int value){
    ui ->  transparencyLabel -> setText (QString ("Transparency: %1").arg (value));
}


Image<U8> MainWindow::getLibTIMFromFrame (){
    const unsigned char *rgb = (unsigned char *)cameraSupport -> GetRGBA ();
    Image<U8> result (WIDTH, HEIGHT);
    unsigned int i;
    if (ui -> redRadioButton -> isChecked())
        i = 0;
    else if (ui -> greenRadioButton -> isChecked())
        i = 1;
    else if (ui -> blueRadioButton -> isChecked())
        i = 2;
    else
        i = 0;
    unsigned int size = WIDTH * HEIGHT;
    for (unsigned int j = 0; j < size; j++){
        result (j) = rgb[i];
        i += 4;
    }

    /*Image<U8> result2 (WIDTH, HEIGHT);
    i = 0;
    for (unsigned int y = 0; y < HEIGHT; y++)
        for (unsigned int x = 0; x < WIDTH; x++){
            result2 (x, y) = rgb[i];
            i += 3;
        }*/

    /*for (unsigned int x = 0; x < WIDTH; x++)
        for (unsigned int y = 0; y < HEIGHT; y++)
            if (result (x, y) != result2 (x, y)){
                qDebug () << x << ", " << y;
                break;
            }*/

    return result;
}


Image<U8> MainWindow::convertQImageToLibTIM (QImage *image){
    Image<U8> result (WIDTH, HEIGHT);
    unsigned int size = WIDTH * HEIGHT;
    const unsigned int *bits = (unsigned int *)image -> constBits ();
    for (unsigned int i = 0; i < size; i++)
        result (i) = qRed(bits[i]);

    /*Image<U8> result2 (WIDTH, HEIGHT);
    for (unsigned int x = 0; x < WIDTH; x++)
        for (unsigned int y = 0; y < HEIGHT; y++)
            result2 (x, y) = qRed (image -> pixel (x, y));*/

    /*for (unsigned int x = 0; x < WIDTH; x++)
        for (unsigned int y = 0; y < HEIGHT; y++)
            if (result (x, y) != result2 (x, y)){
                qDebug () << x << ", " << y;
                break;
            }*/
    return result;
}


QImage *MainWindow::convertLibTIMToQImage (Image<U8> &image, int alpha){
    QImage *result = new QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied);
    unsigned int size = WIDTH * HEIGHT;
    unsigned int *bits = (unsigned int *)result -> bits ();
    U8 value;
    for (unsigned int i = 0; i < size; i++){
        value = image (i);
        if (value > 0)
            bits[i] = qRgba (0, 0, 255, alpha);
        else
            bits[i] = qRgba (0, 0, 0, 0);
    }

    /*QImage *result2 = new QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied);
    for (unsigned int x = 0; x < WIDTH; x++)
        for (unsigned int y = 0; y < HEIGHT; y++){
            value = image (x, y);
            if (value > 0)
                result2 -> setPixel (x, y, qRgba (0, 0, 255, alpha));
            else
                result2 -> setPixel (x, y, qRgba (0, 0, 0, 0));
        }

    for (unsigned int x = 0; x < WIDTH; x++)
        for (unsigned int y = 0; y < HEIGHT; y++)
            if (result -> pixel (x, y) != result2 -> pixel (x, y)){
                qDebug () << x << ", " << y;
                break;
            }
    delete result2;*/
    return result;
}


void MainWindow::computeCT (){
    if (tree != 0){
        delete tree;
        tree = 0;
    }

    if (cameraSupport -> GetRGBA () != 0){
        if (ui -> markerCheckbox -> isChecked () || ui -> resultCheckbox -> isChecked () || ui -> contourCheckbox -> isChecked ()){
            //TEST FOR EACH FRAME (SIZE: 400 x 300)
            //200 :  3945 ,  12038 ,  124553 ,  1330 ,  4156 ,  14740
            //200 :  3459 ,  12158 ,  125187 ,  1438 ,  4266 ,  14495 //optimizing getLibTIMFromFrame function
            //200 :  3495 ,  4211 ,  135086 ,  2054 ,  4956 ,  14784 //optimizing convertQImageToLibTIM function
            //200 :  3677 ,  4268 ,  137953 ,  2564 ,  5684 ,  1523 //optimizing convertLibTIMToQImage function
            //200 :  3976 ,  4644 ,  140883 ,  1807 ,  4823 ,  1502 ,  13841 ,  7185 // extended research version
            //200 :  3916 ,  4655 ,  133632 ,  1609 ,  4645 ,  1562 ,  8590 ,  7054 // using Premultiplied image format
            //200 :  3905 ,  4167 ,  128858 ,  1478 ,  4046 ,  1414 ,  4490 ,  6371 // using RGBA frame instead of RGB
            /* it's possible to make it better:
              1. time6 is ~2.5 times smaller then time1 and time2.
                 time6 - convertLibTIMToQImage  WIDTH * HEIGHT iterations   read LibTIM::Image  write QImage
                 time1 - getLibTIMFromFrame     WIDTH * HEIGHT iterations   write LibTIM::Image read QImage
                 time2 - convertQImageToLibTIM  WIDTH * HEIGHT iterations   write LibTIM::Image read QImage
                 Conclusion: writing LibTIM::Image is slowler then writing QImage
              2. in convertion between YUV and RGB removing clamp function would decrease time of execution
            */
            time1 -= clock ();
            inputImage = getLibTIMFromFrame ();
            time1 += clock ();

            time2 -= clock ();
            markerImage = convertQImageToLibTIM (marker);
            time2 += clock ();

            time3 -= clock ();
            tree = new ComponentTree<U8> (inputImage, markerImage);
            time3 += clock ();

            time4 -= clock ();
            tree -> m_root -> calpha = computeCAlpha (tree -> m_root, alpha);
            time4 += clock ();

            time5 -= clock ();
            computeResult ();
            time5 += clock ();

            if (segmentationResult)
                delete segmentationResult;
            time6 -= clock ();
            segmentationResult = convertLibTIMToQImage (resultImage, ui -> transparencyDial -> value());
            time6 += clock ();


            if (ui -> contourCheckbox -> isChecked ()){
                contourImage = drawContourN4 (resultImage, 255);
                if(contour)
                    delete contour;
                contour = convertLibTIMToQImage (contourImage, 255);
            }
        }
    }
}


double MainWindow::computeCAlpha (Node *n, double alpha){
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


void MainWindow::computeResult (){
    double sum = 0.0;
    double exprl, exprr;

    resultImage = Image<U8> (WIDTH, HEIGHT);
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


Image<U8> MainWindow::drawContourN4 (const Image <U8> &mask, const U8 val){
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


void MainWindow::mouseMoveEvent (QMouseEvent* event){
    if (ui -> markerCheckbox -> isChecked ()){
        int penWidth = ui -> penSizeDial -> value ();
        int transparency = ui -> transparencyDial -> value ();

        QPainter painter (marker);
        painter.setCompositionMode (QPainter::CompositionMode_Source);
        QPen pen;
        pen.setStyle (Qt::SolidLine);
        pen.setWidth (penWidth);
        QBrush brush;
        brush.setStyle (Qt::SolidPattern);
        if (ui -> eraseCheckbox -> isChecked ())
            brush.setColor (QColor (0, 0, 0, 0));
        else
            brush.setColor (QColor (255, 0, 0, transparency));
        painter.setPen (Qt::NoPen);
        painter.setBrush (brush);

        QRect rect (QPoint (event -> x () - penWidth, event -> y () - penWidth),
                    QSize (2 * penWidth, 2 * penWidth));
        painter.drawEllipse (rect);
    }
}


void MainWindow::paintEvent (QPaintEvent *event){
    if (!repaint)
        return;

    if (frame){
        time7 -= clock ();
        /*canvas -> SetFrame(frame);
        if (ui -> resultCheckbox -> isChecked () && segmentationResult != 0)
            canvas -> SetSegmentation(segmentationResult);
        if (ui -> contourCheckbox -> isChecked () && contour != 0)
            canvas -> SetContour(contour);
        if (ui -> markerCheckbox -> isChecked () && marker != 0)
            canvas -> SetMarker(marker);
        canvas -> Update();*/

        //THIS DISPLAYS FRAME ON SOME GUI ELEMENTS, BUT IT WORKS!
        /*QPainter displayPainter (this);
        displayPainter.beginNativePainting();
        glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, WIDTH, HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, cameraSupport -> GetRGB());
        displayPainter.endNativePainting();*/

        /*Display *display = QX11Info::display();
        Drawable hd = handle();
        GC gc = XCreateGC(display, hd, 0, 0);*/

        QPoint topLeft = event -> rect ().topLeft ();
        QPainter displayPainter (this);
        displayPainter.drawImage (topLeft, *frame);
        if (ui -> resultCheckbox -> isChecked () && segmentationResult != 0)
            displayPainter.drawImage (topLeft, *segmentationResult);
        if (ui -> contourCheckbox -> isChecked () && contour != 0)
            displayPainter.drawImage (topLeft, *contour);
        if (ui -> meaningulScalesCheckBox -> isChecked () && boxes != 0)
            displayPainter.drawImage (topLeft, *boxes);
        if (ui -> markerCheckbox -> isChecked () && marker != 0)
            displayPainter.drawImage (topLeft, *marker);
        time7 += clock ();

        frameCounter++;
        qDebug () << frameCounter << ": " << time1 / frameCounter << ", " << time2 / frameCounter << ", " << time3 / frameCounter << ", " << time4 / frameCounter << ", " << time5 / frameCounter << ", " << time6 / frameCounter << ", " << time7 / frameCounter << ", " << time8 / frameCounter << ", " << time9 / frameCounter << ". FPS: " << fps ;
    }
}
