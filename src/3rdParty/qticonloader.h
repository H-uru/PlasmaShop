/****************************************************************************
**
** Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Qt Software Information (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Nokia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain
** additional rights. These rights are described in the Nokia Qt LGPL
** Exception version 1.0, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** If you are unsure which license is appropriate for your use, please
** contact the sales department at qt-sales@nokia.com.
**
****************************************************************************/


#ifndef QTICONLOADER_H
#define QTICONLOADER_H

#include <QtGui/QIcon>

// [Zrax] Set this up to load internal icons on Mac and Windows, but use
// QtIconLoader on Unix-based OSes
#if defined(Q_OS_WIN)
#  define qStdIcon(name) QIcon(":/res/win/" name ".png")
#elif defined(Q_OS_MAC)
#  define qStdIcon(name) QIcon(":/res/mac/" name ".png")
#else

// This is the QtIconLoader 
// Version 0.1
//

class QtIconLoader
{
public:
    static QIcon icon(const QString &name, const QIcon &fallback = QIcon());
};
#  define qStdIcon(name) QtIconLoader::icon(name, QIcon(":/res/def/" name ".png"))

#endif


#endif // QTICONLOADER_H
