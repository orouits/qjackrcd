/***************************************************************************
 Copyright (C) 2011 - Olivier ROUITS <olivier.rouits@free.fr>
 From Giuseppe Cigala code (qvumeter)

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the
 Free Software Foundation, Inc.,
 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 ***************************************************************************/
/**
* @file qjrmeter.cpp
* @author $Author$
* @date $Date$
* @version $Revision$
* @brief Implementation of QJRMeter class
*/

#include "qjrmeter.h"

QJRMeter::QJRMeter(QWidget *parent) : QWidget(parent)
{
    colorBack = QColor(60, 60, 60);
    colorLevel = Qt::white;
    colorHigh = Qt::red;
    colorLow = Qt::green;
    compLevel = -20;
    leftLevel = QVU_MINLEVEL;
    rightLevel = QVU_MINLEVEL;

}


void QJRMeter::paintEvent(QPaintEvent *)
{
    paintBorder();
    paintBar();
}

void QJRMeter::paintBorder()
{
    QPainter painter(this);
    QColor light = Qt::white;
    QColor dark = colorBack.darker(140);

    painter.setPen(QPen(colorBack, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));

    // left round side
    QLinearGradient gradL(0, 0, QVU_BORDER, 0);
    gradL.setColorAt(0, light);
    gradL.setColorAt(1, colorBack);
    gradL.setSpread(QGradient::PadSpread);
    painter.setBrush(gradL);
    QRect borderL(0, 0, QVU_WIDTH/2, QVU_HEIGHT);
    painter.drawRect(borderL);

    // right round side
    QLinearGradient gradR(QVU_WIDTH-QVU_BORDER, 0, QVU_WIDTH, 0);
    gradR.setColorAt(0, colorBack);
    gradR.setColorAt(1, dark);
    gradR.setSpread(QGradient::PadSpread);
    painter.setBrush(gradR);
    QRect borderR(QVU_WIDTH/2, 0, QVU_WIDTH/2, QVU_HEIGHT);
    painter.drawRect(borderR);
}

void QJRMeter::paintBar()
{
    QPainter painter(this);

    QLinearGradient grad(0, QVU_BORDER, 0, QVU_HEIGHT-QVU_BORDER);
    grad.setColorAt(0, colorHigh);
    grad.setColorAt(1, colorLow);
    grad.setSpread(QGradient::PadSpread);
    painter.setBrush(grad);

    // draw color bar
    QRect barL(QVU_BORDER, QVU_BORDER, QVU_LEDWIDTH, QVU_LEDBARHEIGHT);
    painter.drawRect(barL);

    // draw hiding bar
    painter.setBrush(QColor(40, 40, 40));    
    int leftHeight = QVU_LEDBARHEIGHT - (QVU_LEDHEIGHT+1) * (leftLevel-QVU_MINLEVEL);
    int rightHeight = QVU_LEDBARHEIGHT - (QVU_LEDHEIGHT+1) * (rightLevel-QVU_MINLEVEL);
    QRect hideBarL(QVU_BORDER, QVU_BORDER, QVU_LEDWIDTH/2, leftHeight );
    painter.drawRect(hideBarL);
    QRect hideBarR(QVU_BORDER+QVU_LEDWIDTH/2, QVU_BORDER, QVU_LEDWIDTH/2, rightHeight );
    painter.drawRect(hideBarR);

    // draw led intersections
    painter.setPen(QPen(Qt::black, 1));
    for (int i = 0; i < QVU_LEDCOUNT; i++) {
        int y = i*(QVU_LEDHEIGHT+1) + QVU_BORDER;
        painter.drawLine(QVU_BORDER, y, QVU_BORDER + QVU_LEDWIDTH, y);
    }

    // draw pause intersection
    int y = QVU_BORDER + QVU_LEDBARHEIGHT - (QVU_LEDHEIGHT+1) * (compLevel-QVU_MINLEVEL);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawLine(QVU_BORDER, y, QVU_BORDER + QVU_LEDWIDTH, y);

}


void QJRMeter::setColorBack(QColor color)
{
    colorBack = color;
    update();
}

void QJRMeter::setColorLevel(QColor color)
{
    colorLevel = color;
    update();
}

void QJRMeter::setColorHigh(QColor color)
{
    colorHigh = color;
    update();
}


void QJRMeter::setColorLow(QColor color)
{
    colorLow = color;
    update();
}

void QJRMeter::setLeftLevel(double level)
{
    if (level > QVU_MAXLEVEL)
    {
        leftLevel = QVU_MAXLEVEL;
        update();
    }
    else if (level < QVU_MINLEVEL)
    {
        leftLevel = QVU_MINLEVEL;
        update();
    }
    else
    {
        leftLevel = level;
        update();
    }
}

void QJRMeter::setRightLevel(double level)
{
    if (level > QVU_MAXLEVEL)
    {
        rightLevel = QVU_MAXLEVEL;
        update();
    }
    else if (level < QVU_MINLEVEL)
    {
        rightLevel = QVU_MINLEVEL;
        update();
    }
    else
    {
        rightLevel = level;
        update();
    }
}


void QJRMeter::setCompLevel(double level)
{
    if (level > QVU_MAXLEVEL)
    {
        compLevel = QVU_MAXLEVEL;
        update();
    }
    else if (level < QVU_MINLEVEL)
    {
        compLevel = QVU_MINLEVEL;
        update();
    }
    else
    {
        compLevel = level;
        update();
    }
}

QSize QJRMeter::minimumSizeHint() const
{
    return QSize(QVU_WIDTH, QVU_HEIGHT);
}

QSize QJRMeter::sizeHint() const
{
    return QSize(QVU_WIDTH, QVU_HEIGHT);
}


