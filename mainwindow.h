#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QTimer>

#include "include/Image.h"
#include "include/ComponentTree.h"

class CameraSupport;

using namespace LibTIM;

#define MILISECONDS_FOR_REFRESH 1
#define WIDTH 320
#define HEIGHT 240

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

    Image<U8> getLibTIMFromFrame ();
    Image<U8> convertQImageToLibTIM (QImage *img);
    QImage *convertLibTIMToQImage(Image<U8> &image, int alpha);

    void computeCT ();
    double computeCAlpha (Node *n, double alpha);
    void computeResult();
    Image<U8> drawContourN4 (const Image <U8> &mask, const U8 val);

protected:
    void paintEvent (QPaintEvent *event);
    void mouseMoveEvent (QMouseEvent * event);

private:
    CameraSupport *cameraSupport;

    Ui::MainWindow *ui;
    QTimer *timer;
    QImage *frame;
    QImage *marker;
    QImage *segmentationResult;
    QImage *contour;
    QImage *boxes;

    double alpha;

    unsigned long long *frameTime;
    unsigned int totalFrames;
    unsigned int currentFrame;
    unsigned int fps;

    Image<U8> inputImage;
    Image<U8> markerImage;
    Image<U8> resultImage;
    Image<U8> contourImage;

    ComponentTree<U8> *tree;
    vector<Node *> leafs;

    bool repaint;

    unsigned long long frameCounter;
    clock_t time1, time2, time3, time4, time5, time6, time7, time8, time9;

private slots:
    void updateFrame();
    void alphaChanged (int);
    void penSizeChanged (int);
    void transparencyChanged (int);
};

#endif // MAINWINDOW_H
