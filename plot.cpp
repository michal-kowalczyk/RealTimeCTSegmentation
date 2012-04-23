/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#include "plot.h"

Plot::Plot (QWidget *parent) : QwtPlot( parent ){
    // panning with the left mouse button
    (void) new QwtPlotPanner( canvas() );

    setAutoFillBackground( true );
    setPalette( QPalette( QColor( 165, 193, 228 ) ) );
    updateGradient();


    // axes
    this -> setAxisFont (xBottom, QFont ("Lucida Grande", 8, QFont::Normal)) ;
    setAxisTitle(xBottom, "alpha" );
    setAxisScale(xBottom, 0.0, 1.0);

    this -> setAxisFont (yLeft, QFont ("Lucida Grande", 8, QFont::Normal)) ;
    setAxisTitle(yLeft, "num of points");
    setAxisScale(yLeft, 0.0, 1.0);

    // canvas
    canvas()->setLineWidth( 1 );
    canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas()->setBorderRadius( 15 );

    QPalette canvasPalette( Qt::white );
    canvasPalette.setColor( QPalette::Foreground, QColor( 133, 190, 232 ) );
    canvas()->setPalette( canvasPalette );

    curve = new QwtPlotCurve ();
    curve -> setPen (QPen (Qt::red));

    histogramRed = new QwtPlotHistogram ();
    histogramRed -> setPen (QPen (Qt::red));

    histogramBlue = new QwtPlotHistogram ();
    histogramBlue -> setPen (QPen (Qt::blue));
}


void Plot::updateGradient()
{
    QPalette pal = palette();

    const QColor buttonColor = pal.color( QPalette::Button );

#ifdef Q_WS_X11
    // Qt 4.7.1: QGradient::StretchToDeviceMode is buggy on X11

    QLinearGradient gradient( rect().topLeft(), rect().bottomLeft() );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );
#else
    QLinearGradient gradient( 0, 0, 0, 1 );
    gradient.setCoordinateMode( QGradient::StretchToDeviceMode );
    gradient.setColorAt( 0.0, Qt::white );
    gradient.setColorAt( 0.7, buttonColor );
    gradient.setColorAt( 1.0, buttonColor );
#endif

    pal.setBrush( QPalette::Window, gradient );
    setPalette( pal );
}

void Plot::resizeEvent( QResizeEvent *event )
{
    QwtPlot::resizeEvent( event );
#ifdef Q_WS_X11
    updateGradient();
#endif
}


void Plot::SetCurve (QVector<double> x, QVector<double> y){
    histogramRed -> detach ();
    histogramBlue -> detach ();
    curve -> detach ();
    curve -> setSamples (x, y);
    curve -> attach (this);
    replot ();
}


void Plot::SetHistogram (QVector<QwtIntervalSample> red, QVector<QwtIntervalSample> blue){
    curve -> detach ();
    histogramRed -> detach ();
    histogramRed -> setSamples (red);
    histogramRed -> attach (this);
    histogramBlue -> detach ();
    histogramBlue -> setSamples (blue);
    histogramBlue -> attach (this);
    replot ();
}
