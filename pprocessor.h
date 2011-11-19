// $Id$
/***************************************************************************
 Copyright (C) 2011 - Olivier ROUITS <olivier.rouits@free.fr>

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

#ifndef PPROCESSOR_H
#define PPROCESSOR_H

#include <QString>

class PProcessor
{

    QString cmdLine;
    QString fileName;
    QString launchedFileName;

public:
    PProcessor();
    ~PProcessor();
    void launchFileProcess();

    QString getCmdLine() {return cmdLine;}
    void setCmdLine(QString line) {cmdLine = line;}
    QString getFileName() {return fileName;}
    void setFileName(QString fname) {fileName = fname;}
    QString getLaunchedFileName() {return launchedFileName;}
};

#endif // PPROCESSOR_H
