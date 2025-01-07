#include <QtGui>
#include "timestamp.h"

TimeStamp::TimeStamp(QWidget *parent)
: QLabel(parent)
{
	extern unsigned int l_Time;
	TimeStamp::setNum((int)l_Time);
}

void TimeStamp::paintEvent(QPaintEvent *event)
{
	Q_UNUSED(event);
	extern unsigned int l_Time;
	TimeStamp::setNum((int)l_Time);
	QPainter painter(this);
	painter.setBrush(QBrush(Qt::white,Qt::SolidPattern));
	painter.setPen(Qt::black);
    int textWidth = fontMetrics().horizontalAdvance(TimeStamp::text());
	if (textWidth < 1)
		return;
	painter.drawText(0, 0, textWidth, height(),
	Qt::AlignLeft | Qt::AlignVCenter, TimeStamp::text());
}
