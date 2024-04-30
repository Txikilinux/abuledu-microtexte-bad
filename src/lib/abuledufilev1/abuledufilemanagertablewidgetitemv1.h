/**
  * Classe
  * @author 2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @see The GNU Public License (GNU/GPL) v3
  *
  * This program is free software; you can redistribute it and/or modify
  * it under the terms of the GNU General Public License as published by
  * the Free Software Foundation; either version 2 of the License, or
  * (at your option) any later version.
  *
  * This program is distributed in the hope that it will be useful, but
  * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
  * for more details.
  *
  * You should have received a copy of the GNU General Public License along
  * with this program; if not, write to the Free Software Foundation, Inc.,
  * 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
  */

#ifndef ABEFILEMANAGERTABLEWIDGETITEM_H
#define ABEFILEMANAGERTABLEWIDGETITEM_H

#include <QObject>
#include <QWidget>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QDebug>

class AbulEduFileManagerTableWidgetItemV1 : public QTableWidgetItem
{
public:
    AbulEduFileManagerTableWidgetItemV1();
    AbulEduFileManagerTableWidgetItemV1(const QString&);
    virtual ~AbulEduFileManagerTableWidgetItemV1();

    bool operator <(const QTableWidgetItem &other) const
    {
        //Compare des dates
        if(other.text().contains(QRegExp("../../.... ..:.."))) {
            QDateTime dateA = QDateTime::fromString(text(), "dd'/'MM'/'yyyy hh':'mm");
            QDateTime dateB = QDateTime::fromString(other.text(), "dd'/'MM'/'yyyy hh':'mm");
            return dateA < dateB;
        }
        if(other.text().contains(QRegExp("....-..-.. ..:..:.."))) {
            QDateTime dateA = QDateTime::fromString(text(), "yyyy'-'MM'-'dd hh':'mm':'ss");
            QDateTime dateB = QDateTime::fromString(other.text(), "yyyy'-'MM'-'dd hh':'mm':'ss");
            return dateA < dateB;
        }
        //Compare des entiers
        if(text() == QString::number(text().toInt())) {
            return text().toInt() < other.text().toInt();
        }
        //et par défaut, compare du texte sans tenir compte de la casse
        return QString::compare(text(),other.text(),Qt::CaseInsensitive);
    }
};

#endif // ABEFILEMANAGERTABLEWIDGETITEM_H
