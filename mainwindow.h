/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTimer>

#include "camerasupport.h"
#include "segmentationbasedoncomponenttree.h"
#include "timer.h"
#include "subimage.h"

#include "componenttreeinputgenerator.h"

#define MILISECONDS_FOR_REFRESH 1
#define WIDTH 400
#define HEIGHT 300

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    enum ScreenOrientation {
        ScreenOrientationLockPortrait,
        ScreenOrientationLockLandscape,
        ScreenOrientationAuto
    };

    explicit MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

    // Note that this will only have an effect on Symbian and Fremantle.
    void setOrientation(ScreenOrientation orientation);

    void showExpanded();

protected:
    void paintEvent (QPaintEvent *event);
    void mouseMoveEvent (QMouseEvent * event);

    void createComponentTreeInput ();
    unsigned int calculateFoundedPointsNumber ();
    unsigned int calculateFoundedMarkedPointsNumber ();
    double jaccardIndex (QImage &image1, QImage &image2);

private:
    Ui::MainWindow *ui;
    QTimer *timer;

    CameraSupport cameraSupport;
    Subimage markedPixels;
    SegmentationBasedOnComponentTree segmentationAlgorithm;
    ComponentTreeInputGenerator inputGenerator;

    QImage frame;
    QImage input;
    QImage marker;
    QImage segmentationResult;
    QImage contour;
    QImage boxes;
    QImage groundTruth;

    double alpha;

    unsigned long long *frameTime;
    unsigned int totalFrames;
    unsigned int currentFrame;
    unsigned int fps;

    bool repaint;

    unsigned long long frameCounter;
    Timer timers[11];

    bool changeInput;
    bool changeResult;
    bool changeMeaningulScales;

    bool groundTruthAvaible;

private slots:
    void updateFrame ();
    void alphaChanged (int);
    void penSizeChanged (int);
    void transparencyChanged (int);
    void fastSwitch1 (bool);
    void fastSwitch2 (bool);
    void startStopCamera ();
    void save ();
    void load ();
    void addRemoveColourCanal (bool);
    void changeOutOfRangeSolution (bool checked);
    void changeNegationMode (bool checked);
    void changeCanalsMixingMode (bool checked);
    void changeImageTransformationMode (bool checked);

    void on_bisectionPushButton_clicked();
    void on_plotAlphaButton_clicked();
    void on_meaningulScalesCheckBox_toggled(bool checked);
    void on_plotMarkerHistogramPushButton_clicked();
    void on_bisectionWithGroundTruthPushButton_clicked();
    void on_msForAlphaPushButton_clicked();
};

#endif // MAINWINDOW_H
