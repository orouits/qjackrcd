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
* $Author$
* $Date$
* $Revision$
* @brief Implementation of QJRMeter class
*/

#include "qjrmeter.h"

#define QJRM_LEDHEIGHT 1
#define QJRM_LEDWIDTH 10
#define QJRM_BORDER 5

#define QJRM_LEDCOUNT (QJRM_MAXLEVEL - QJRM_MINLEVEL)
#define QJRM_LEDBARHEIGHT (QJRM_LEDCOUNT*(QJRM_LEDHEIGHT+1))
#define QJRM_WIDTH (2*QJRM_BORDER + QJRM_LEDWIDTH)
#define QJRM_HEIGHT (2*QJRM_BORDER + QJRM_LEDBARHEIGHT)

QJRMeter::QJRMeter(QWidget *parent) : QWidget(parent)
{
    colorBack = QColor(60, 60, 60);
    colorLevel = Qt::white;
    colorHigh = Qt::red;
    colorLow = Qt::green;
    compLevel = -20;
    leftLevel = QJRM_MINLEVEL;
    rightLevel = QJRM_MINLEVEL;

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
    QLinearGradient gradL(0, 0, QJRM_BORDER, 0);
    gradL.setColorAt(0, light);
    gradL.setColorAt(1, colorBack);
    gradL.setSpread(QGradient::PadSpread);
    painter.setBrush(gradL);
    QRect borderL(0, 0, QJRM_WIDTH/2, QJRM_HEIGHT);
    painter.drawRect(borderL);

    // right round side
    QLinearGradient gradR(QJRM_WIDTH-QJRM_BORDER, 0, QJRM_WIDTH, 0);
    gradR.setColorAt(0, colorBack);
    gradR.setColorAt(1, dark);
    gradR.setSpread(QGradient::PadSpread);
    painter.setBrush(gradR);
    QRect borderR(QJRM_WIDTH/2, 0, QJRM_WIDTH/2, QJRM_HEIGHT);
    painter.drawRect(borderR);
}

void QJRMeter::paintBar()
{
    QPainter painter(this);

    QLinearGradient grad(0, QJRM_BORDER, 0, QJRM_HEIGHT-QJRM_BORDER);
    grad.setColorAt(0, colorHigh);
    grad.setColorAt(1, colorLow);
    grad.setSpread(QGradient::PadSpread);
    painter.setBrush(grad);

    // draw color bar
    QRect barL(QJRM_BORDER, QJRM_BORDER, QJRM_LEDWIDTH, QJRM_LEDBARHEIGHT);
    painter.drawRect(barL);

    // draw hiding bar
    painter.setBrush(QColor(40, 40, 40));    
    int leftHeight = QJRM_LEDBARHEIGHT - (QJRM_LEDHEIGHT+1) * (leftLevel-QJRM_MINLEVEL);
    int rightHeight = QJRM_LEDBARHEIGHT - (QJRM_LEDHEIGHT+1) * (rightLevel-QJRM_MINLEVEL);
    QRect hideBarL(QJRM_BORDER, QJRM_BORDER, QJRM_LEDWIDTH/2, leftHeight );
    painter.drawRect(hideBarL);
    QRect hideBarR(QJRM_BORDER+QJRM_LEDWIDTH/2, QJRM_BORDER, QJRM_LEDWIDTH/2, rightHeight );
    painter.drawRect(hideBarR);

    // draw led intersections
    painter.setPen(QPen(Qt::black, 1));
    for (int i = 0; i < QJRM_LEDCOUNT; i++) {
        int y = i*(QJRM_LEDHEIGHT+1) + QJRM_BORDER;
        painter.drawLine(QJRM_BORDER, y, QJRM_BORDER + QJRM_LEDWIDTH, y);
    }

    // draw pause intersection
    int y = QJRM_BORDER + QJRM_LEDBARHEIGHT - (QJRM_LEDHEIGHT+1) * (compLevel-QJRM_MINLEVEL);
    painter.setPen(QPen(Qt::white, 1));
    painter.drawLine(QJRM_BORDER, y, QJRM_BORDER + QJRM_LEDWIDTH, y);

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
    if (level > QJRM_MAXLEVEL)
    {
        leftLevel = QJRM_MAXLEVEL;
        update();
    }
    else if (level < QJRM_MINLEVEL)
    {
        leftLevel = QJRM_MINLEVEL;
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
    if (level > QJRM_MAXLEVEL)
    {
        rightLevel = QJRM_MAXLEVEL;
        update();
    }
    else if (level < QJRM_MINLEVEL)
    {
        rightLevel = QJRM_MINLEVEL;
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
    if (level > QJRM_MAXLEVEL)
    {
        compLevel = QJRM_MAXLEVEL;
        update();
    }
    else if (level < QJRM_MINLEVEL)
    {
        compLevel = QJRM_MINLEVEL;
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
    return QSize(QJRM_WIDTH, QJRM_HEIGHT);
}

QSize QJRMeter::sizeHint() const
{
    return QSize(QJRM_WIDTH, QJRM_HEIGHT);
}


