#include <QtGui>
#include "spectro.h"
#include "renderarea.h"
#include "timestamp.h"
#include "predictions.h"

RenderArea::RenderArea(QWidget *parent)
: QwtPlot(parent)
{
  setBackgroundRole(QPalette::Base);
  setAutoFillBackground(true);
  curve = new QwtPlotCurve;
  // Show the axes
  this->setAxisTitle( this->xBottom, "Wavelength (nm)" );
  this->setAxisTitle( this->yLeft, "Intensity (counts)" );
}

QSize RenderArea::minimumSizeHint() const
{
  return QSize();
}

QSize RenderArea::sizeHint() const
{
  return QSize();
}

void RenderArea::setPen(const QPen &pen)
{
  this->pen = pen;
  update();
}

void RenderArea::setBrush(const QBrush &brush)
{
  this->brush = brush;
  update();
}


void RenderArea::paintEvent(QPaintEvent *event)
{
  Q_UNUSED(event);
  extern unsigned short m_NrPixels;
  extern double m_pLambda[MAX_NR_PIXELS];
  extern double l_pSpectrum[MAX_NR_PIXELS];

  curve->setSamples(m_pLambda, l_pSpectrum, m_NrPixels);
  printf("number of pixels: %d\n",m_NrPixels);
  //for (int i = 0;i<m_NrPixels;i++)
    printf("2017 value: %f\n",l_pSpectrum[2017]);
    printf("2018 value: %f\n",l_pSpectrum[2018]);


  curve->attach(this);
  this->replot();
  this->show();

  /*QPainter painter(this);
  painter.setPen(pen);
  painter.setBrush(brush);
  //painter.scale(500.0/1100.0, 300.0/16384.0);
  painter.drawPolyline(points, m_NrPixels);
  painter.setPen(palette().dark().color());
  painter.setBrush(Qt::NoBrush);
  QRect rect(0, 0, 1100, 16384);
  painter.drawRect(rect);*/
}
