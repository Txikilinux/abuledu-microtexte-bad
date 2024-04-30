/**
  * Classe AbulEduFlatBoutonV1Plugin
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

#include "abuleduflatboutonv1-plugin.h"

#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>

#include <QtCore/qplugin.h>
#include <QtDeclarative/QDeclarativeView>

static const char toolTipC[] = "AbulEdu Flat Bouton V1";

QT_BEGIN_NAMESPACE

AbulEduFlatBoutonV1Plugin::AbulEduFlatBoutonV1Plugin(QObject *parent)
    : QObject(parent)
{
    m_initialized = false;
}

void AbulEduFlatBoutonV1Plugin::initialize(QDesignerFormEditorInterface * /* core */)
{
    if (m_initialized)
        return;

    // Add extension registrations, etc. here
    m_initialized = true;
}

bool AbulEduFlatBoutonV1Plugin::isInitialized() const
{
    return m_initialized;
}

QWidget *AbulEduFlatBoutonV1Plugin::createWidget(QWidget *parent)
{
    return new AbulEduFlatBoutonV1(parent);
}

QString AbulEduFlatBoutonV1Plugin::name() const
{
    return QLatin1String("AbulEduFlatBoutonV1");
}

QString AbulEduFlatBoutonV1Plugin::group() const
{
    return QLatin1String("AbulEdu");
}

QIcon AbulEduFlatBoutonV1Plugin::icon() const
{
    return QIcon();
}

QString AbulEduFlatBoutonV1Plugin::toolTip() const
{
    return QLatin1String("");
}

QString AbulEduFlatBoutonV1Plugin::whatsThis() const
{
    return QLatin1String("");
}

bool AbulEduFlatBoutonV1Plugin::isContainer() const
{
    return false;
}

QString AbulEduFlatBoutonV1Plugin::domXml() const
{
    return QLatin1String("\
    <ui language=\"c++\">\
        <widget class=\"AbulEduFlatBoutonV1\" name=\"btnAbeFlat\">\
        </widget>\
    </ui>");
}

QString AbulEduFlatBoutonV1Plugin::includeFile() const
{
    return QLatin1String("abuleduflatboutonv1.h");
}

QT_END_NAMESPACE

//Q_EXPORT_PLUGIN2(AbulEduFlatBoutonV1Plugin, AbulEduFlatBoutonV1Plugin)
