#ifndef MAINDISPLAY_H
#define MAINDISPLAY_H


#include "mainwindow.h"


class MainWindow;

class MainDisplay : public QWidget
{
    Q_OBJECT
public:
    MainDisplay(QWidget *parent=0);
    ~MainDisplay();
    void updateWidget();
    void updateImages();
protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent ( QMouseEvent * event );

private slots:
    void updateDisplay(int) {update();}

private:
    void updateImageDisplay();

    QImage *marker;

    //Image<U8> markerImage;

};

#endif // MAINDISPLAY_H
