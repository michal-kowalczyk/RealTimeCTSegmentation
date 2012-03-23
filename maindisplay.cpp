#include "maindisplay.h"
#include <QPainter>
#include <QPen>
#include <QMouseEvent>
#include <QDebug>


MainDisplay::MainDisplay (QWidget *parent): QWidget (parent){
    unsigned int width = static_cast<MainWindow *> (parentWidget ()) -> getWidth ();
    unsigned int height = static_cast<MainWindow *> (parentWidget ()) -> getHeight ();
    marker = new QImage (width, height, QImage::Format_ARGB32);
    marker -> fill (qRgba (0, 0, 0, 0));
}


MainDisplay::~MainDisplay (){
    if (marker != 0)
        delete marker;
    marker = 0;
}


void MainDisplay::mouseMoveEvent (QMouseEvent* event){
    if (marker != 0){
        int penWidth = 10;
        int transparency = 150;

        QPainter painter(marker);
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        QPen pen;  // creates a default pen
        QBrush brush;
        brush.setStyle(Qt::SolidPattern);
        pen.setStyle(Qt::SolidLine);
        //pen.setCapStyle(Qt::RoundCap);
        pen.setWidth(penWidth);
        //if(!this->erase)
            brush.setColor(QColor(255,0,0,transparency));
        //else
        //    brush.setColor(QColor(0,0,0,0));
        painter.setPen(Qt::NoPen);
        painter.setBrush(brush);

        qDebug () << "a";

        QRect rect (QPoint (event -> x () - penWidth, event -> y () - penWidth),
                    QSize (2 * penWidth, 2 * penWidth));
        painter.drawEllipse(rect);

        //ui -> myLabel -> setPixmap(QPixmap::fromImage(*marker));

        update();
        //update((rect.x()-1)*zoomFactor,(rect.y()-1)*zoomFactor,(rect.width()+3)*zoomFactor,(rect.height()+3)*zoomFactor);
    }

}


void MainDisplay::paintEvent(QPaintEvent *event)
{
    /*if(qtImgOri!=0 && qtImgResult!=0 && qtImgSeg!=0){

        QPainter painter(qtImgDisplay);
        painter.save();
        painter.scale(zoomFactor,zoomFactor);
        const QRect rect = event->rect();
        const QRect rectSrc(rect.topLeft().x()/zoomFactor,rect.topLeft().y()/zoomFactor,rect.width()/zoomFactor,rect.height()/zoomFactor);

        //qDebug() << "Rect: " << rect << "Scaled rect: " <<rectSrc <<"\n";

        if (static_cast<MainWindow *> (parentWidget ()) -> isOriginalRadioButtonChecked ()){
            //painter.drawImage(rect,*qtImgOri,rectSrc);
            //painter.drawImage(QPointF(0,0),*qtImgOri);
            painter.drawImage (rectSrc.topLeft (), *qtImgOri, rectSrc);
        } else if (static_cast<MainWindow *> (parentWidget ()) -> isGrayScaleRadioButtonChecked ()){
            if (qtImgGrayScale == 0){
                qtImgGrayScale = new QImage (*qtImgOri);

                int pixelColour, width = qtImgOri -> width (), height = qtImgOri -> height ();

                for (int x = 0; x < width; x++)
                    for (int y = 0; y < height; y++){
                        pixelColour = qGray (qtImgOri -> pixel (x, y));
                        qtImgGrayScale -> setPixel (x, y, qRgb (pixelColour, pixelColour, pixelColour));
                    }
            }

            painter.drawImage (rectSrc.topLeft (), *qtImgGrayScale, rectSrc);
        } else if (static_cast<MainWindow *> (parentWidget ()) -> isHueRadioButtonChecked ()){
            if (qtImgHue == 0){
                qtImgHue = new QImage (*qtImgOri);

                int pixelColour, hue, width = qtImgOri -> width (), height = qtImgOri -> height ();

                double colour;

                for (int x = 0; x < width; x++)
                    for (int y = 0; y < height; y++){
                        hue = QColor (qtImgOri -> pixel (x, y)).hslHue () % 360;
                        colour = (hue * 255.0) / 359.0;
                        pixelColour = colour;
                        qtImgHue -> setPixel (x, y, qRgb (pixelColour, pixelColour, pixelColour));
                    }
            }

            painter.drawImage (rectSrc.topLeft (), *qtImgHue, rectSrc);
        } else
            painter.fillRect (rect, QColor (0, 0, 0));

        if(static_cast<MainWindow *> (parentWidget())->isRbt_ResChecked() )
        {
        //painter.drawImage(QPointF(0,0),*qtImgResult);
        //painter.drawImage(rect,*qtImgResult,rectSrc);
        painter.drawImage(rectSrc.topLeft(), *qtImgResult, rectSrc);
        //painter.drawImage(QRect(0,0,dimX,dimY),*qtImgResult);
        }
        if(static_cast<MainWindow *> (parentWidget())->isRbt_SegChecked() )
        {
        //painter.drawImage(QPointF(0,0),*qtImgSeg);
        //painter.drawImage(rect,*qtImgSeg,rectSrc);
        painter.drawImage(rectSrc.topLeft(), *qtImgSeg, rectSrc);
        //painter.drawImage(QRect(0,0,dimX,dimY),*qtImgSeg);
        }
        if(static_cast<MainWindow *> (parentWidget())->isRbt_ContourChecked())
        {
        //painter.drawImage(QPointF(0,0),*qtImgContour);
        //painter.drawImage(rect,*qtImgContour,rectSrc);
        painter.drawImage(rectSrc.topLeft(), *qtImgContour, rectSrc);
        //painter.drawImage(QRect(0,0,dimX,dimY),*qtImgContour);
        }
        painter.restore();
//	QPixmap qpm;
//	qpm=QPixmap::fromImage(*qtImgDisplay);

        QPainter dispPainter(this);
        dispPainter.drawImage(rect.topLeft(), *qtImgDisplay, rect);
        //dispPainter.drawImage(QPointF(0,0), *qtImgDisplay);

        //qDebug() << "Time: " << (double)(t2-t1)/CLOCKS_PER_SEC << "Rect: " << (double)(t3-t2)/CLOCKS_PER_SEC<< "\n";
        //dispPainter.drawPixmap(QPointF(0,0),qpm);


//
//	oriChanged=false;
//	resultChanged=false;
//	segChanged=false;
//	contourChanged=false;
    }*/
}




void MainDisplay::updateWidget()
{
    /*if(qtImgOri!=0)
            {
            dimX=(int)(qtImgOri->width()*zoomFactor);
            dimY=(int)(qtImgOri->height()*zoomFactor);
            //qDebug() << dimX << " " << dimY << "\n";
            if(qtImgDisplay)
                delete qtImgDisplay;
            qtImgDisplay=new QImage(dimX,dimY,QImage::Format_ARGB32);
            //qtImgDisplay=new QImage(qtImgOri->width(),qtImgOri->height(),QImage::Format_ARGB32);
            resize(dimX,dimY);
            updateGeometry();
            update();

            this->parentWidget()->adjustSize();
            this->parentWidget()->setFixedSize(this->parentWidget()->size());
            }
    else
        update();*/

}
