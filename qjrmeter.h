// $Id$
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
// $LastChangedBy$
// $LastChangedRevision$
// $LastChangeDate$
// $HeadURL$

#ifndef QVUMETER_H
#define QVUMETER_H

#include <QtGui>
#include <QtDesigner/QDesignerExportWidget>
#include <math.h>

#define QVU_MAXLEVEL 3
#define QVU_MINLEVEL -40
#define QVU_LEDHEIGHT 1
#define QVU_LEDWIDTH 10
#define QVU_BORDER 5

#define QVU_LEDCOUNT (QVU_MAXLEVEL - QVU_MINLEVEL)
#define QVU_LEDBARHEIGHT (QVU_LEDCOUNT*(QVU_LEDHEIGHT+1))
#define QVU_WIDTH (2*QVU_BORDER + QVU_LEDWIDTH)
#define QVU_HEIGHT (2*QVU_BORDER + QVU_LEDBARHEIGHT)

class QDESIGNER_WIDGET_EXPORT QJRMeter : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(QColor colorBack READ getColorBack WRITE setColorBack);
    Q_PROPERTY(QColor colorLevel READ getColorLevel WRITE setColorLevel);
    Q_PROPERTY(QColor colorLow READ getColorLow WRITE setColorLow);
    Q_PROPERTY(QColor colorHigh READ getColorHigh WRITE setColorHigh);
    Q_PROPERTY(double leftLevel READ getLeftLevel WRITE setLeftLevel);
    Q_PROPERTY(double rightLevel READ getRightLevel WRITE setRightLevel);
    Q_PROPERTY(double compLevel READ getCompLevel WRITE setCompLevel);

    QColor getColorBack() const
    {
        return colorBack;
    }
    QColor getColorLevel() const
    {
        return colorLevel;
    }
    QColor getColorLow() const
    {
        return colorLow;
    }
    QColor getColorHigh() const
    {
        return colorHigh;
    }
    double getLeftLevel() const
    {
        return leftLevel;
    }
    double getRightLevel() const
    {
        return rightLevel;
    }
    double getCompLevel() const
    {
        return compLevel;
    }


public:

    QJRMeter(QWidget *parent = 0);
    QSize minimumSizeHint() const;
    QSize sizeHint() const;


signals:

    void valueLChanged(double);
    void valueRChanged(double);

public slots:

    void setColorBack(QColor);
    void setColorLevel(QColor);
    void setColorHigh(QColor);
    void setColorLow(QColor);
    void setLeftLevel(double);
    void setRightLevel(double);
    void setCompLevel(double);


protected:

    void paintEvent(QPaintEvent *);
    void paintBorder();
    void paintBar();

private:

    double leftLevel;
    double rightLevel;
    double compLevel;
    QColor colorBack;
    QColor colorLevel;
    QColor colorHigh;
    QColor colorLow;


};

#endif
