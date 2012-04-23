/**
  This file is an effect of work of Michal Kowalczyk, student of University of Lodz (Poland), doing his master 2 as a erasmus student at Universite de Lorraine (France) - former Universite Henri Poincare during his intership in Loria (France).
  */

#ifndef PLOT_H
#define PLOT_H

#include <qlayout.h>
#include "Qwt/qwt_plot.h"
#include "Qwt/qwt_plot_marker.h"
#include "Qwt/qwt_plot_curve.h"
#include "Qwt/qwt_plot_histogram.h"
#include "Qwt/qwt_legend.h"
#include "Qwt/qwt_series_data.h"
#include "Qwt/qwt_plot_canvas.h"
#include "Qwt/qwt_plot_panner.h"
#include "Qwt/qwt_plot_magnifier.h"
#include "Qwt/qwt_text.h"
#include "Qwt/qwt_math.h"
#include <math.h>

class Plot : public QwtPlot{
public:
    Plot( QWidget *parent = NULL);
    void SetCurve (QVector<double> x, QVector<double> y);
    void SetHistogram (QVector<QwtIntervalSample> red, QVector<QwtIntervalSample> blue);

protected:
    virtual void resizeEvent( QResizeEvent * );

private:
    void updateGradient();
    QwtPlotCurve *curve;
    QwtPlotHistogram *histogramRed;
    QwtPlotHistogram *histogramBlue;
};

#endif // PLOT_H
