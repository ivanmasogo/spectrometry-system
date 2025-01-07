#ifndef RENDERAREA_H
#define RENDERAREA_H

#include <QBrush>
#include <QPen>
#include <QWidget>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>



class RenderArea : public QwtPlot
{
  Q_OBJECT

public:
  RenderArea(QWidget *parent = 0);
  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  public slots:
  void setPen(const QPen &pen);
  void setBrush(const QBrush &brush);

protected:
  void paintEvent(QPaintEvent *event);

private:
  QPen pen;
  QBrush brush;
  QwtPlot *plot;
  QwtPlotCurve *curve;

};

#endif //RENDERAREA_H
