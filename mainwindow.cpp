#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QtCore/QCoreApplication>

#include <QTimer>
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QDebug>
#include <QFile>

#include "camerasupport.h"
#include "grayscaleimagehistogram.h"
#include "imagemarkerfile.h"
#include "meaningfulscales.h"
#include "meaningfulscalesalphaadjust.h"


//TEST FOR EACH FRAME (SIZE: 400 x 300)
//200 :  3945 ,  12038 ,  124553 ,  1330 ,  4156 ,  14740
//200 :  3459 ,  12158 ,  125187 ,  1438 ,  4266 ,  14495 //optimizing getLibTIMFromFrame function
//200 :  3495 ,  4211 ,  135086 ,  2054 ,  4956 ,  14784 //optimizing convertQImageToLibTIM function
//200 :  3677 ,  4268 ,  137953 ,  2564 ,  5684 ,  1523 //optimizing convertLibTIMToQImage function
//200 :  3976 ,  4644 ,  140883 ,  1807 ,  4823 ,  1502 ,  13841 ,  7185 // extended research version
//200 :  3916 ,  4655 ,  133632 ,  1609 ,  4645 ,  1562 ,  8590 ,  7054 // using Premultiplied image format
//200 :  3905 ,  4167 ,  128858 ,  1478 ,  4046 ,  1414 ,  4490 ,  6371 // using RGBA frame instead of RGB
//200 :  4607 ,  4665 ,  131061 ,  1030 ,  3950 ,  1434 ,  4875 ,  5203 // using optimized YUV to RGB converter
//200 :  2848 ,  3266 ,  132376 ,  1730 ,  2562 ,  1855 ,  6510 ,  4940 ,  0 ,  14505 . // using references to images in place of new objects


MainWindow::MainWindow (QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    cameraSupport (WIDTH, HEIGHT),
    markedPixels (WIDTH * HEIGHT),
    segmentationAlgorithm (WIDTH, HEIGHT),
    inputGenerator (WIDTH * HEIGHT),
    frame (QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied)),
    input (QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied)),
    marker (QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied)),
    segmentationResult (QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied)),
    contour (QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied)),
    boxes (QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied)),
    groundTruth (QImage (WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied))
{
    ui->setupUi(this);

    connect (ui -> alphaDial, SIGNAL (valueChanged (int)), this, SLOT (alphaChanged (int)));
    connect (ui -> penSizeDial, SIGNAL (valueChanged (int)), this, SLOT (penSizeChanged (int)));
    connect (ui -> transparencyDial, SIGNAL (valueChanged (int)), this, SLOT (transparencyChanged (int)));
    connect (ui -> cameraStartStopPushButton, SIGNAL (clicked ()), this, SLOT (startStopCamera ()));
    connect (ui -> savePushButton, SIGNAL (clicked ()), this, SLOT (save ()));
    connect (ui -> loadPushButton, SIGNAL (clicked ()), this, SLOT (load ()));

    connect (ui -> redCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> greenCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> blueCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> hueCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> saturationCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> valueCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> lightnessCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> cyanCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> magentaCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> yellowCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));
    connect (ui -> keyCheckBox, SIGNAL (toggled (bool)), this, SLOT (addRemoveColourCanal (bool)));

    connect (ui -> noOOPProcessing, SIGNAL (toggled (bool)), this, SLOT (changeOutOfRangeSolution (bool)));
    connect (ui -> oorARadioButton, SIGNAL (toggled (bool)), this, SLOT (changeOutOfRangeSolution (bool)));
    connect (ui -> oorBRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeOutOfRangeSolution (bool)));
    connect (ui -> oorFRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeOutOfRangeSolution (bool)));

    connect (ui -> noNegationRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeNegationMode (bool)));
    connect (ui -> negationRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeNegationMode (bool)));
    connect (ui -> autoNegationRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeNegationMode (bool)));

    connect (ui -> averageLightnessRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeCanalsMixingMode (bool)));
    connect (ui -> weightedAverageDifferenceRradioButton, SIGNAL (toggled (bool)), this, SLOT (changeCanalsMixingMode (bool)));
    connect (ui -> weightedBinaryHistogramRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeCanalsMixingMode (bool)));
    connect (ui -> weightedAverageDifferenceRradioButton, SIGNAL (toggled (bool)), this, SLOT (changeCanalsMixingMode (bool)));

    connect (ui -> noChangeLightnessRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeImageTransformationMode (bool)));
    connect (ui -> bubbleBrightingRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeImageTransformationMode (bool)));
    connect (ui -> maximalLightnessRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeImageTransformationMode (bool)));
    connect (ui -> averageLightnessRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeImageTransformationMode (bool)));
    connect (ui -> maximalAverageLightnessRadioButton, SIGNAL (toggled (bool)), this, SLOT (changeImageTransformationMode (bool)));

    ui -> alphaDial -> setValue (1000);
    ui -> penSizeDial -> setValue (10);
    ui -> transparencyDial -> setValue (100);

    frame.fill (qRgba (0, 0, 0, 0));
    input.fill (qRgba (0, 0, 0, 0));
    marker.fill (qRgba (0, 0, 0, 0));
    segmentationResult.fill (qRgba (0, 0, 0, 0));
    contour.fill (qRgba (0, 0, 0, 0));
    boxes.fill (qRgba (0, 0, 0, 0));
    groundTruth.fill (qRgba (0, 0, 0, 0));

    groundTruthAvaible = false;

    changeInput = false;
    changeResult = false;
    changeMeaningulScales = false;

    repaint = false;

    totalFrames = 1000 / MILISECONDS_FOR_REFRESH;
    frameTime = new unsigned long long[totalFrames];
    for (unsigned int i = 0; i < totalFrames; i++)
        frameTime[i] = 0;
    currentFrame = 0;
    fps = 0;

    frameCounter = 0;

    timer = new QTimer (this);
    connect (timer, SIGNAL (timeout ()), this, SLOT (updateFrame ()));
    timer -> start (MILISECONDS_FOR_REFRESH);

    connect (ui -> fastSwitch1RadioButton, SIGNAL (toggled (bool)), this, SLOT (fastSwitch1 (bool)));
    connect (ui -> fastSwitch2RadioButton, SIGNAL (toggled (bool)), this, SLOT (fastSwitch2 (bool)));

    ui -> plotWidget -> hide ();
}


MainWindow::~MainWindow (){
    delete []frameTime;

    timer -> stop ();
    delete timer;

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


void MainWindow::alphaChanged (int value){
    changeResult = true;

    alpha = (double)value / 100000.0;
    ui ->  alphaLabel -> setText (QString ("Alpha: %1").arg (alpha));
}


void MainWindow::penSizeChanged (int value){
    ui ->  penSizeLabel -> setText (QString ("Pen size: %1").arg (value));
}


void MainWindow::transparencyChanged (int value){
    segmentationAlgorithm.SetTransparency (value);
    ui ->  transparencyLabel -> setText (QString ("Transparency: %1").arg (value));
}


void MainWindow::fastSwitch1 (bool value){
    if (value){
        ui -> noNegationRadioButton -> setChecked (true);
        ui -> noChangeLightnessRadioButton -> setChecked (true);
        ui -> noOOPProcessing -> setChecked (true);
        ui -> redCheckBox -> setChecked (true);
        ui -> greenCheckBox -> setChecked (false);
        ui -> blueCheckBox -> setChecked (false);
        ui -> hueCheckBox -> setChecked (false);
        ui -> saturationCheckBox -> setChecked (false);
        ui -> valueCheckBox -> setChecked (false);
        ui -> lightnessCheckBox -> setChecked (false);
        ui -> cyanCheckBox -> setChecked (false);
        ui -> magentaCheckBox -> setChecked (false);
        ui -> yellowCheckBox -> setChecked (false);
        ui -> keyCheckBox -> setChecked (false);
        ui -> arithmeticMeanRadioButton -> setChecked (true);
        if (!ui -> fastSwitch1RadioButton -> isChecked ())
            ui -> fastSwitch1RadioButton -> setChecked (true);
    }
}


void MainWindow::fastSwitch2 (bool value){
    if (value){
        ui -> autoNegationRadioButton -> setChecked (true);
        ui -> averageLightnessRadioButton -> setChecked (true);
        ui -> oorFRadioButton -> setChecked (true);
        ui -> redCheckBox -> setChecked (true);
        ui -> greenCheckBox -> setChecked (true);
        ui -> blueCheckBox -> setChecked (true);
        ui -> hueCheckBox -> setChecked (true);
        ui -> saturationCheckBox -> setChecked (true);
        ui -> valueCheckBox -> setChecked (true);
        ui -> lightnessCheckBox -> setChecked (false);
        ui -> weightedMeanRadioButton -> setChecked (true);
        if (!ui -> fastSwitch2RadioButton -> isChecked ())
            ui -> fastSwitch2RadioButton -> setChecked (true);
    }
}


void MainWindow::updateFrame (){
    bool result = false;
    if (cameraSupport.IsRunning ()){
        timers[7].StartCycle ();
        result = cameraSupport.UpdateFrame ();
        timers[7].StopCycle ();
    }

    if (result){
        changeInput = true;
        frame = QImage ((unsigned char *)cameraSupport.GetRGBA (), WIDTH, HEIGHT, QImage::Format_ARGB32_Premultiplied);
    }

    if (changeInput){
        timers[10].StartCycle ();
        createComponentTreeInput ();
        timers[10].StopCycle ();

        if (markedPixels.Size () > 0){
            if (ui -> contourCheckbox -> isChecked ())
                segmentationAlgorithm.Compute (alpha, input, marker, segmentationResult, contour, timers);
            else
                segmentationAlgorithm.Compute (alpha, input, marker, segmentationResult, timers);

            if (groundTruthAvaible)
                qDebug () << "Jaccard Index:" << jaccardIndex (segmentationResult, groundTruth);

            changeResult = false;
            if (ui -> meaningulScalesCheckBox -> isChecked ())
                changeMeaningulScales = true;
        }
    }

    if (changeResult){
        segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (alpha, segmentationResult, timers);

        if (groundTruthAvaible)
            qDebug () << "Jaccard Index:" << jaccardIndex (segmentationResult, groundTruth);

        if (ui -> meaningulScalesCheckBox -> isChecked ())
            changeMeaningulScales = true;
    }

    if (changeMeaningulScales){
        timers[8].StartCycle ();
        MeaningfulScales ms(segmentationResult);
        timers[8].StopCycle ();
        boxes = QImage (ms.GetResult());
    }

    if (changeInput || changeResult || changeMeaningulScales){
        repaint = true;
        update (0, 0, WIDTH, HEIGHT);

        changeInput = false;
        changeResult = false;
        changeMeaningulScales = false;
    }

    frameTime[currentFrame] = clock ();

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


void MainWindow::createComponentTreeInput (){
    InputGeneratingParameters parameters;
    parameters.redCanalIncluded = ui -> redCheckBox -> isChecked ();
    parameters.greenCanalIncluded = ui -> greenCheckBox -> isChecked ();
    parameters.blueCanalIncluded = ui -> blueCheckBox -> isChecked ();
    parameters.hueCanalIncluded = ui -> hueCheckBox -> isChecked ();
    parameters.saturationCanalIncluded = ui -> saturationCheckBox -> isChecked ();
    parameters.valueCanalIncluded = ui -> valueCheckBox -> isChecked ();
    parameters.lightnessCanalIncluded = ui -> lightnessCheckBox -> isChecked ();
    parameters.cyanCanalIncluded = ui -> cyanCheckBox -> isChecked ();
    parameters.magentaCanalIncluded = ui -> magentaCheckBox -> isChecked ();
    parameters.yellowCanalIncluded = ui -> yellowCheckBox -> isChecked ();
    parameters.keyCanalIncluded = ui -> keyCheckBox -> isChecked ();

    parameters.negationMode = NO_NEGATION;
    if (ui -> negationRadioButton -> isChecked ())
        parameters.negationMode = NEGATION;
    else if (ui -> autoNegationRadioButton -> isChecked ())
        parameters.negationMode = AUTONEGATION;

    if (ui -> oorARadioButton -> isChecked ())
        parameters.outOfRangeSolution = 1;
    else if (ui -> oorBRadioButton -> isChecked ())
        parameters.outOfRangeSolution = 2;
    else if (ui -> oorFRadioButton -> isChecked ())
        parameters.outOfRangeSolution = 6;

    parameters.canalsMixingMode = AVERAGE_MEAN;
    if (ui -> weightedMeanRadioButton -> isChecked ())
        parameters.canalsMixingMode = WEIGHTED_MEAN_BY_COLOURS_DIFFERENCES;
    else if (ui -> weightedBinaryHistogramRadioButton -> isChecked ())
        parameters.canalsMixingMode = WEIGHTED_MEAN_BY_BINARY_HISTOGRAM_COMPARISON;
    else if (ui -> weightedAverageDifferenceRradioButton -> isChecked ())
        parameters.canalsMixingMode = WEIGHTED_MEAN_BY_PROPORTIONAL_HISTOGRAM_COMPARISON;

    parameters.imageTransformationMode = NO_TRANSFORMATION;
    if (ui -> bubbleBrightingRadioButton -> isChecked ())
        parameters.imageTransformationMode = MARKED_PIXELS_HISTOGRAM_BUBBLE_BRIGHTENING;
    else if (ui -> maximalLightnessRadioButton -> isChecked ())
        parameters.imageTransformationMode = MARKED_PIXELS_MAXIMAL_VALUE_BRIGHTENING;
    else if (ui -> averageLightnessRadioButton -> isChecked ())
        parameters.imageTransformationMode = MARKED_PIXELS_AVERAGE_PIXEL_VALUE_BRIGHTNENING;
    else if (ui -> maximalAverageLightnessRadioButton -> isChecked ())
        parameters.imageTransformationMode = MARKED_PIXELS_MAXIMAL_AVERAGE_PIXEL_VALUE_BRIGHTENING;

    inputGenerator.GenerateInput ((const unsigned char *)cameraSupport.GetRGBA (), input, parameters, markedPixels);
}


void MainWindow::mouseMoveEvent (QMouseEvent* event){
    if (ui -> markerCheckbox -> isChecked ()){
        int penWidth = ui -> penSizeDial -> value ();

        QPainter painter (&marker);
        painter.setCompositionMode (QPainter::CompositionMode_Source);
        QPen pen;
        pen.setStyle (Qt::SolidLine);
        pen.setWidth (penWidth);
        QBrush brush;
        brush.setStyle (Qt::SolidPattern);
        if (ui -> eraseCheckbox -> isChecked ())
            brush.setColor (QColor (0, 0, 0, 0));
        else
            brush.setColor (QColor (255, 0, 0, ui -> transparencyDial -> value ()));
        painter.setPen (Qt::NoPen);
        painter.setBrush (brush);

        QRect rect (QPoint (event -> x () - penWidth, event -> y () - penWidth),
                    QSize (2 * penWidth, 2 * penWidth));
        painter.drawEllipse (rect);

        markedPixels.Update (marker);
        if (markedPixels.Size () == 0){
            segmentationResult.fill (QColor (0, 0, 0, 0));
            boxes.fill (QColor (0, 0, 0, 0));
            contour.fill (QColor (0, 0, 0, 0));
        }

        changeInput = true;
    }
}


void MainWindow::paintEvent (QPaintEvent *event){
    if (!repaint)
        return;

    timers[6].StartCycle ();
    QPoint topLeft = event -> rect ().topLeft ();
    QPainter displayPainter;
    displayPainter.begin (this);

    if (ui -> originalRadioButton -> isChecked())
        displayPainter.drawImage (topLeft, frame);
    else if (ui -> inputRadioButton -> isChecked())
        displayPainter.drawImage (topLeft, input);

    if (ui -> resultCheckbox -> isChecked ())
        displayPainter.drawImage (topLeft, segmentationResult);
    if (ui -> contourCheckbox -> isChecked ())
        displayPainter.drawImage (topLeft, contour);
    if (ui -> meaningulScalesCheckBox -> isChecked ())
        displayPainter.drawImage (topLeft, boxes);
    if (ui -> markerCheckbox -> isChecked ())
        displayPainter.drawImage (topLeft, marker);
    if (ui -> groundTruthCheckBox -> isChecked () && groundTruthAvaible)
        displayPainter.drawImage (topLeft, groundTruth);

    displayPainter.end ();
    timers[6].StopCycle ();

    frameCounter++;
    //qDebug () << frameCounter << ": " << timers[0].AverageTimePerCycle () << ", " << timers[1].AverageTimePerCycle () << ", " << timers[2].AverageTimePerCycle () << ", " << timers[3].AverageTimePerCycle () << ", " << timers[4].AverageTimePerCycle () << ", " << timers[5].AverageTimePerCycle () << ", " << timers[6].AverageTimePerCycle () << ", " << timers[7].AverageTimePerCycle () << ", " << timers[8].AverageTimePerCycle () << ", "  << timers[9].AverageTimePerCycle () << ", "  << timers[10].AverageTimePerCycle ()  << ". FPS: " << fps;
}


void MainWindow::startStopCamera (){
    if (cameraSupport.IsRunning ()){
        ui -> plotWidget -> show ();
        ui -> cameraStartStopPushButton -> setText ("Camera START!");
        cameraSupport.Stop ();
    } else{
        ui -> plotWidget -> hide ();
        ui -> cameraStartStopPushButton -> setText ("Camera STOP!");
        cameraSupport.Start ();
        groundTruthAvaible = false;
    }
}


void MainWindow::save (){
    QFile file ("/sdcard/imageMarkerFile.imf");
    QImage empty (0, 0, QImage::Format_ARGB32_Premultiplied);
    ImageMarkerFile::Write (file, frame, marker, empty);
}


void MainWindow::load (){
    QFile file ("/sdcard/imageMarkerFile.imf");
    groundTruth.fill (QColor (0, 0, 0, 0));
    ImageMarkerFile::Read (file, frame, marker, groundTruth);
    groundTruthAvaible = true;

    markedPixels.Update (marker);

    changeInput = true;
}


void MainWindow::on_bisectionPushButton_clicked (){
    double left = 0;
    segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (left, segmentationResult, timers);
    unsigned int numberForLeft = calculateFoundedMarkedPointsNumber ();

    double right = 1;
    double alpha = left;

    unsigned int i = 0;
    while (i < 50){
        alpha = left + (right - left) / 2;
        segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (alpha, segmentationResult, timers);
        unsigned int numberOfMarkedPoints = calculateFoundedMarkedPointsNumber ();
        if (numberOfMarkedPoints < numberForLeft)
            right = alpha;
        else
            left = alpha;

        i++;
    }

    ui -> alphaDial -> setValue (alpha * 100000);
}


unsigned int MainWindow::calculateFoundedMarkedPointsNumber (){
    unsigned int result = 0;
    const unsigned int *segmentationBits = (const unsigned int *)segmentationResult.constBits ();
    for (Subimage::PixelsPositionsIterator i = markedPixels.begin (); i != markedPixels.end (); ++i)
        if (qBlue (segmentationBits[*i]) > 0)
            result++;
    return result;
}


unsigned int MainWindow::calculateFoundedPointsNumber (){
    unsigned int result = 0;
    const unsigned int *segmentationResultPointer = (const unsigned int *)segmentationResult.constBits ();
    const unsigned int *segmentationResultEndPointer = segmentationResultPointer + WIDTH * HEIGHT;
    while (segmentationResultPointer != segmentationResultEndPointer){
        if (*segmentationResultPointer > 0)
            result++;
        segmentationResultPointer++;
    }
    return result;
}


void MainWindow::on_bisectionWithGroundTruthPushButton_clicked (){
    double alpha = 0.0;
    bool leftToRight = true;

    double step = 0.2;
    segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (alpha, segmentationResult, timers);

    double theBestAlpha = alpha;
    double theBestJaccard = jaccardIndex (segmentationResult, groundTruth);
    bool theBestLeftToRight = leftToRight;

    unsigned int i = 0;
    while (i < 50){
        if (leftToRight)
            if (alpha + step <= 1.0)
                alpha += step;
            else{
                leftToRight = false;
                step /= 2.0;
                continue;
            }
        else
            if (alpha >= step)
                alpha -= step;
            else{
                leftToRight = true;
                step /= 2.0;
                continue;
            }
        segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (alpha, segmentationResult, timers);
        double jaccard = jaccardIndex (segmentationResult, groundTruth);
        if (jaccard < theBestJaccard){
            alpha = theBestAlpha;
            theBestLeftToRight = !theBestLeftToRight;
            leftToRight = theBestLeftToRight;
            step /= 2.0;
        } else{
            theBestAlpha = alpha;
            theBestJaccard = jaccard;
            theBestLeftToRight = leftToRight;
        }

        i++;
    }

    ui -> alphaDial -> setValue (theBestAlpha * 100000);
}


void MainWindow::on_plotAlphaButton_clicked (){
    QImage previousSegmentationResult = segmentationResult;

    QVector<double> x;
    QVector<double> y;
    unsigned short stepsNumber = 100;
    double max = 0.0;
    double alpha = 0.0, step = 1.0 / (double)stepsNumber;
    for (unsigned short i = 0; i < stepsNumber; i++){
        x.append(alpha);
        segmentationAlgorithm.DoSegmentationWithPreviousComponentTree (alpha, segmentationResult, timers);
        y.append((double)calculateFoundedPointsNumber ());
        alpha += step;

        if (max < y.last ())
            max = y.last ();
    }

    for (unsigned short i = 0; i < x.size (); i++)
        y[i] /= max;

    ui -> plotWidget -> SetCurve (x, y);

    segmentationResult = previousSegmentationResult;
}


void MainWindow::on_meaningulScalesCheckBox_toggled (bool checked){
    if (checked)
        changeMeaningulScales = true;
}


/**
  @todo use Qwt logaritmic scale (without data scaling)
  */
void MainWindow::on_plotMarkerHistogramPushButton_clicked (){
    GrayScaleImageHistogram imageHistogram (input);
    GrayScaleImageHistogram markedPixelsHistogram (input, markedPixels);

    double max = 0;
    for (unsigned short i = 0; i < 256; i++)
        if (max < log (imageHistogram[i] + 1))
            max = log (imageHistogram[i] + 1);

    QVector<QwtIntervalSample> markerQVector, imageQVector;
    for (unsigned int i = 0; i < 256; i++){
        imageQVector.append (QwtIntervalSample ((double)log (imageHistogram[i] + 1) / (double)max, i / 256.0, i / 256.0 + 1 / 256.0));
        markerQVector.append (QwtIntervalSample ((double)log (markedPixelsHistogram[i] + 1) / (double)max, i / 256.0, i / 256.0 + 1 / 256.0));
    }

    ui -> plotWidget -> SetHistogram (imageQVector, markerQVector);
}


double MainWindow::jaccardIndex (QImage &image1, QImage &image2){
    unsigned int setsUnion = 0, setsIntersection = 0;
    unsigned int size = WIDTH * HEIGHT;
    const unsigned int *image1Pointer = (const unsigned int *)image1.constBits (), *image2Pointer = (const unsigned int *)image2.constBits ();
    for (unsigned int i = 0; i < size; i++){
        if (*image1Pointer > 0 && *image2Pointer > 0){
            setsIntersection++;
            setsUnion++;
        } else if (*image1Pointer > 0 || *image2Pointer > 0)
            setsUnion++;
        image1Pointer++;
        image2Pointer++;
    }
    if (setsUnion > 0)
        return (double)setsIntersection / (double)setsUnion;
    return 0;
}


void MainWindow::addRemoveColourCanal (bool){
    ui -> fastSwitchCustomRadioButton -> setChecked (true);
    changeInput = true;
}


void MainWindow::changeOutOfRangeSolution (bool checked){
    if (checked){
        ui -> fastSwitchCustomRadioButton -> setChecked (true);
        changeInput = true;
    }
}


void MainWindow::changeNegationMode (bool checked){
    if (checked){
        ui -> fastSwitchCustomRadioButton -> setChecked (true);
        changeInput = true;
    }
}


void MainWindow::changeCanalsMixingMode (bool checked){
    if (checked){
        ui -> fastSwitchCustomRadioButton -> setChecked (true);
        changeInput = true;
    }
}


void MainWindow::changeImageTransformationMode (bool checked){
    if (checked){
        ui -> fastSwitchCustomRadioButton -> setChecked (true);
        changeInput = true;
    }
}


void MainWindow::on_msForAlphaPushButton_clicked (){
    unsigned char indicator = 1;
    if (ui -> msIndicator1RadioButton -> isChecked ())
        indicator = 1;
    else if (ui -> msIndicator2RadioButton -> isChecked ())
        indicator = 2;
    else if (ui -> msIndicator3RadioButton -> isChecked ())
        indicator = 3;
    else if (ui -> msIndicator4RadioButton -> isChecked ())
        indicator = 4;
    else if (ui -> msIndicator5RadioButton -> isChecked ())
        indicator = 5;
    else if (ui -> msIndicator6RadioButton -> isChecked ())
        indicator = 6;
    else if (ui -> msIndicator7RadioButton -> isChecked ())
        indicator = 7;

    MeaningfulScalesAlphaAdjust msAlphaAdjust (segmentationAlgorithm, segmentationResult, indicator);
    ui -> alphaDial -> setValue (msAlphaAdjust.FindAlpha (0.01, 4) * 100000);
    changeResult = true;
}
