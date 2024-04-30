/**
  * Bouton plat et transparent, developpé initialement pour le projet
  * Imageo (le-terrier d'AbulEdu) et repris dans les classes de base
  * d'AbulÉdu (merci Thomas !)
  *
  * @see https://redmine.ryxeo.com/projects/leterrier-imageo
  * @author 2011 Thomas Grosjean-Pasut <thomas.grosjeanpasut@ryxeo.com>
  * @author 2011-2012 Eric Seigne <eric.seigne@ryxeo.com>
  * @author 2011 Jean-Louis Frucot <frucot.jeanlouis@free.fr>
  *
  * @see The GNU Public License (GPL)
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

#include "abuleduflatboutonv1.h"

#ifndef DEBUG_ABULEDUFLATBOUTONV1
    #include <abuledudisableloggerv1.h>
#endif

AbulEduFlatBoutonV1::AbulEduFlatBoutonV1(QWidget* parent) :
    QPushButton(parent)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS   = new QMap<QString, QString>();
    m_icones        = new QMap<QString, QString>();
    m_paddingLeft   = 0;
    m_paddingRight  = 0;
    m_paddingTop    = 0;
    m_paddingBottom = 0;
    m_marginLeft    = 0;
    m_marginRight   = 0;
    m_marginTop     = 0;
    m_marginBottom  = 0;
    m_borderRadius  = 0;
    m_fontSize      = -1;
    m_backgroundAlignement = QString();
    m_timer         = new QTimer();

    setCursor(Qt::PointingHandCursor);

    //Dans le cas des tablettes on fait en sorte que le bouton ne soit pas re-cliquable immédiatement
#ifdef __ABULEDUTABLETTEV1__MODE__
    setDisableTimeout(500);
#else
    setDisableTimeout(0);
#endif

    //Je ne vois pas pourquoi ? ->
    setFocusPolicy(Qt::NoFocus);

    // Couleurs par défaut
    setCouleursTexte(QColor(Qt::black), QColor(Qt::blue), QColor(Qt::blue), QColor(Qt::gray));
    setCouleursFond(QColor(Qt::transparent), QColor(Qt::transparent), QColor(Qt::transparent), QColor(Qt::transparent));

    // Pas d'icones par défaut
    setIcones("","","");

    connect(this, SIGNAL(clicked()), this, SLOT(slotClicked()), Qt::UniqueConnection);

    /* Tres important que ça soit en dernier sinon les setIcones et autres passent le flag à true et ca force la mise a jour css alors qu'on ne
       le veut pas forcement si on a mis un css dans l'ui */
    m_newCSSAvailable = false;

    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();
}

AbulEduFlatBoutonV1::~AbulEduFlatBoutonV1()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    delete m_couleursCSS;
    delete m_icones;
    delete m_timer;
}


void AbulEduFlatBoutonV1::setCouleursTexte(const QColor &normale, const QColor &survol, const QColor &pressed, const QColor &disabled)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    if(normale.isValid())
    {
        setCouleurTexteNormale(normale);
    }
    if(survol.isValid())
    {
        setCouleurTexteSurvol(survol);
    }
    if(pressed.isValid())
    {
        setCouleurTextePressed(pressed);
    }
    if(disabled.isValid())
    {
        setCouleurTexteDisabled(disabled);
    }

    m_newCSSAvailable = true;
    updateCSS();
}

void AbulEduFlatBoutonV1::setCouleurTexteNormale(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS->insert("couleurTexteNormale", toRGBA(couleur));
    m_newCSSAvailable = true;
    updateCSS();
}

QColor AbulEduFlatBoutonV1::getCouleurTexteNormale()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return fromRGBA(m_couleursCSS->value("couleurTexteNormale"));
}

void AbulEduFlatBoutonV1::setCouleurTexteSurvol(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS->insert("couleurTexteSurvol", toRGBA(couleur));
    m_newCSSAvailable = true;
    updateCSS();
}

QColor AbulEduFlatBoutonV1::getCouleurTexteSurvol()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return fromRGBA(m_couleursCSS->value("couleurTexteSurvol"));
}

void AbulEduFlatBoutonV1::setCouleurTextePressed(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS->insert("couleurTextePressed", toRGBA(couleur));
    m_newCSSAvailable = true;
    updateCSS();
}

QColor AbulEduFlatBoutonV1::getCouleurTextePressed()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return fromRGBA(m_couleursCSS->value("couleurTextePressed"));
}

void AbulEduFlatBoutonV1::setCouleurTexteDisabled(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS->insert("couleurTexteDisabled", toRGBA(couleur));
    m_newCSSAvailable = true;
    updateCSS();
}

QColor AbulEduFlatBoutonV1::getCouleurTexteDisabled()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return fromRGBA(m_couleursCSS->value("couleurTexteDisabled"));
}

void AbulEduFlatBoutonV1::setCouleursFond(const QColor &normale, const QColor &survol, const QColor &pressed, const QColor &disabled)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    if(normale.isValid())
    {
        setCouleurFondNormale(normale);
    }
    if(survol.isValid())
    {
        setCouleurFondSurvol(survol);
    }
    if(pressed.isValid())
    {
        setCouleurFondPressed(pressed);
    }
    if(disabled.isValid())
    {
        setCouleurFondDisabled(disabled);
    }
}

void AbulEduFlatBoutonV1::setCouleurFondNormale(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS->insert("couleurFondNormale", toRGBA(couleur));
    m_newCSSAvailable = true;
    updateCSS();
}

QColor AbulEduFlatBoutonV1::getCouleurFondNormale()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return fromRGBA(m_couleursCSS->value("couleurFondNormale"));
}

void AbulEduFlatBoutonV1::setCouleurFondSurvol(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS->insert("couleurFondSurvol", toRGBA(couleur));
    m_newCSSAvailable = true;
    updateCSS();
}

QColor AbulEduFlatBoutonV1::getCouleurFondSurvol()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return fromRGBA(m_couleursCSS->value("couleurFondSurvol"));
}

void AbulEduFlatBoutonV1::setCouleurFondPressed(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS->insert("couleurFondPressed", toRGBA(couleur));
    m_newCSSAvailable = true;
    updateCSS();
}

QColor AbulEduFlatBoutonV1::getCouleurFondPressed()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return fromRGBA(m_couleursCSS->value("couleurFondPressed"));
}

void AbulEduFlatBoutonV1::setCouleurFondDisabled(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_couleursCSS->insert("couleurFondDisabled", toRGBA(couleur));
    m_newCSSAvailable = true;
    updateCSS();
}

QColor AbulEduFlatBoutonV1::getCouleurFondDisabled()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return fromRGBA(m_couleursCSS->value("couleurFondDisabled"));
}

void AbulEduFlatBoutonV1::setIcones(const QString &normale, const QString &survol, const QString &pressed, const QString &disabled)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName()
                         << "normale :" << normale << "survol : " << survol
                         << "pressed :" << pressed << "disabled : " << disabled;

    if(!normale.isEmpty() && QFile(normale).exists())
        m_icones->insert("iconeNormale", normale);
    else
        m_icones->insert("iconeNormale", "");

    if(!survol.isEmpty() && QFile(survol).exists())
        m_icones->insert("iconeSurvol", survol);
    else
        m_icones->insert("iconeSurvol", "");

    if(!pressed.isEmpty() && QFile(pressed).exists())
        m_icones->insert("iconePressed", pressed);
    else
        m_icones->insert("iconePressed", "");

    if(!disabled.isEmpty() && QFile(disabled).exists())
        m_icones->insert("iconeDisabled", disabled);
    else
        m_icones->insert("iconeDisabled", "");

    m_newCSSAvailable = true;
    updateCSS();
}

void AbulEduFlatBoutonV1::setIconeNormale(const QString &normale)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    if(QFile(normale).exists()) {
        m_icones->insert("iconeNormale", normale);
    }
    else {
        ABULEDU_LOG_DEBUG() << "le fichier n'existe pas :" << normale;
        m_icones->insert("iconeNormale", "");
    }
    m_newCSSAvailable = true;
    updateCSS();
}

QString AbulEduFlatBoutonV1::getIconeNormale()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_icones->value("iconeNormale");
}

void AbulEduFlatBoutonV1::setIconeSurvol(const QString &survol)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    if(QFile(survol).exists())
        m_icones->insert("iconeSurvol", survol);
    else
        m_icones->insert("iconeSurvol", "");
    m_newCSSAvailable = true;
    updateCSS();
}

QString AbulEduFlatBoutonV1::getIconeSurvol()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_icones->value("iconeSurvol");
}

void AbulEduFlatBoutonV1::setIconePressed(const QString &pressed)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    if(QFile(pressed).exists())
        m_icones->insert("iconePressed", pressed);
    else
        m_icones->insert("iconePressed", "");
    m_newCSSAvailable = true;
    updateCSS();
}

QString AbulEduFlatBoutonV1::getIconePressed()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_icones->value("iconePressed");
}

void AbulEduFlatBoutonV1::setIconeDisabled(const QString &disabled)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    if(QFile(disabled).exists())
        m_icones->insert("iconeDisabled", disabled);
    else
        m_icones->insert("iconeDisabled", "");
    m_newCSSAvailable = true;
    updateCSS();
}

QString AbulEduFlatBoutonV1::getIconeDisabled()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_icones->value("iconeDisabled");
}

QColor AbulEduFlatBoutonV1::fromRGBA(QString couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    QColor c;
    if(couleur.startsWith("rgba(")) {
        couleur.remove("rgba(");
        couleur.remove(")");
        c.setRgb(couleur.split(",").at(0).toInt(),
                 couleur.split(",").at(1).toInt(),
                 couleur.split(",").at(2).toInt(),
                 couleur.split(",").at(3).toInt());
    }
    return c;
}

QString AbulEduFlatBoutonV1::toRGBA(const QColor &couleur)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    QString s="";
    if(couleur.isValid())
    {
        s.append("rgba(");
        s.append(QString::number(couleur.red()));
        s.append(",");
        s.append(QString::number(couleur.green()));
        s.append(",");
        s.append(QString::number(couleur.blue()));
        s.append(",");
        s.append(QString::number(couleur.alpha()));
        s.append(")");
    }
    else
    {
        s="(0,0,0,0)";
    }
    return s;
}

void AbulEduFlatBoutonV1::updateCSS()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    //Si le bouton n'est pas visible ça ne sert à rien de mettre à jour sa css
    if(!m_newCSSAvailable || !isVisible()) {
        return;
    }

    m_cssFlatBouton.clear();

    //    m_cssFlatBouton.append(QString("QPushButton > *{color:red;}"));
    // Etat normal du bouton------------------------------------------------------------------
    m_cssFlatBouton.append(QString("QPushButton{"));
    if(m_marginLeft > 0 || m_marginRight > 0 || m_marginTop > 0 || m_marginBottom > 0)
    {
        m_cssFlatBouton.append(QString("margin: "));
        m_cssFlatBouton.append(QString::number(m_marginTop)+"px ");
        m_cssFlatBouton.append(QString::number(m_marginRight)+"px ");
        m_cssFlatBouton.append(QString::number(m_marginBottom)+"px ");
        m_cssFlatBouton.append(QString::number(m_marginLeft)+"px ");
        m_cssFlatBouton.append(QString(";"));
    }

    if(m_borderRadius > 0)
    {
        m_cssFlatBouton.append("border:0px solid red; border-radius:"+QString::number(m_borderRadius)+"px;");
    }
    else
    {
        m_cssFlatBouton.append("border: none; ");
    }
    m_cssFlatBouton.append("color:"+m_couleursCSS->value("couleurTexteNormale")+";");

    m_cssFlatBouton.append(m_texteAlignement);

    if(m_fontSize > 0){
        m_cssFlatBouton.append("font-size:"+QString::number(m_fontSize)+"pt;");
    }

    //Ajout d'un déplacement horizontal si nécessaire
    if(m_paddingLeft > 0) {
        m_cssFlatBouton.append(QString("padding-left: %1px").arg(m_paddingLeft)+";");
    }

    if(m_paddingRight > 0) {
        m_cssFlatBouton.append(QString("padding-right: %1px").arg(m_paddingRight)+";");
    }

    if(m_paddingTop > 0) {
        m_cssFlatBouton.append(QString("padding-top: %1px").arg(m_paddingTop)+";");
    }

    if(m_paddingBottom > 0) {
        m_cssFlatBouton.append(QString("padding-bottom: %1px").arg(m_paddingBottom)+";");
    }


    m_cssFlatBouton.append(QString("background-repeat: no-repeat;"));
    // Ajout d'une couleur de fond
    m_cssFlatBouton.append(QString("background-color:"+m_couleursCSS->value("couleurFondNormale")+";"));

    // Ajout d'une icone s'il y en a une
    if(m_icones->value("iconeNormale").size() > 0)
    {
        //Si on place l'image en background ou en border-image ?
        if(m_backgroundAlignement.isEmpty()){
            m_cssFlatBouton.append(QString("border-image:url('"+m_icones->value("iconeNormale")+"');"));
            m_cssFlatBouton.append(QString("image-position: center;"));
        }
        else {
            m_cssFlatBouton.append(QString("background-image:url('"+m_icones->value("iconeNormale")+"');"));
            m_cssFlatBouton.append(QString("background-position: " + m_backgroundAlignement + ";"));
        }
    }
    m_cssFlatBouton.append(QString("}\n"));

    // Etat normal du survolé du bouton-------------------------------------------------------
    m_cssFlatBouton.append(QString("QPushButton:hover{color:"+m_couleursCSS->value("couleurTexteSurvol")+";"));
    // Ajout d'une couleur de fond
    m_cssFlatBouton.append(QString("background-color:"+m_couleursCSS->value("couleurFondSurvol")+";"));
    // Ajout d'une icone s'il y en a une
    if(m_icones->value("iconeSurvol").size() > 0)
    {
        if(m_backgroundAlignement.isEmpty()){
            m_cssFlatBouton.append(QString("border-image:url('"+m_icones->value("iconeSurvol")+"');"));
            m_cssFlatBouton.append(QString("image-position: center;"));
        }
        else {
            m_cssFlatBouton.append(QString("background-image:url('"+m_icones->value("iconeSurvol")+"');"));
            m_cssFlatBouton.append(QString("background-position: " + m_backgroundAlignement + ";"));
        }
    }
    m_cssFlatBouton.append(QString("image-position: center;}\n"));

    // Etat pressé du bouton----------------------------------------------------------------
    m_cssFlatBouton.append(QString("QPushButton:pressed{color:"+m_couleursCSS->value("couleurTextePressed")+";"));
    // Ajout d'une couleur de fond
    m_cssFlatBouton.append(QString("background-color:"+m_couleursCSS->value("couleurFondPressed")+";"));
    // Ajout d'une icone s'il y en a une
    if(m_icones->value("iconePressed").size() > 0)
    {
        if(m_backgroundAlignement.isEmpty()){
            m_cssFlatBouton.append(QString("border-image:url('"+m_icones->value("iconePressed")+"');"));
            m_cssFlatBouton.append(QString("image-position: center;"));
        }
        else {
            m_cssFlatBouton.append(QString("background-image:url('"+m_icones->value("iconePressed")+"');"));
            m_cssFlatBouton.append(QString("background-position: " + m_backgroundAlignement + ";"));
        }
    }
    m_cssFlatBouton.append(QString("image-position: center;}\n"));

    // Etat disabled du bouton------------------------------------------------------------------
    m_cssFlatBouton.append(QString("QPushButton:disabled{border: none; color:"+m_couleursCSS->value("couleurTexteDisabled")+";"));
    // Ajout d'une couleur de fond
    m_cssFlatBouton.append(QString("background-color:"+m_couleursCSS->value("couleurFondDisabled")+";"));
    // Ajout d'une icone s'il y en a une
    if(m_icones->value("iconeDisabled").size() > 0)
    {
        if(m_backgroundAlignement.isEmpty()){
            m_cssFlatBouton.append(QString("border-image:url('"+m_icones->value("iconeDisabled")+"');"));
            m_cssFlatBouton.append(QString("image-position: center;"));
        }
        else {
            m_cssFlatBouton.append(QString("background-image:url('"+m_icones->value("iconeDisabled")+"');"));
            m_cssFlatBouton.append(QString("background-position: " + m_backgroundAlignement + ";"));
        }
    }
    else
    {
        m_cssFlatBouton.append(QString("image:none")+";"); // Plus d'icône
    }
    m_cssFlatBouton.append(QString("image-position: center;}\n"));

    m_cssFlatBouton.append(QString("QToolTip {background: #f5ffbb; color: black; border-color: #000; border-width: 1px; border-style:solid; }\n"));

    ABULEDU_LOG_DEBUG() << m_cssFlatBouton;
    m_cssFlatBouton.append("QPushButton:focus {border: none;outline: none;}");

    setStyleSheet(m_cssFlatBouton);
    m_newCSSAvailable = false;
}

void AbulEduFlatBoutonV1::setCSSFlatBouton(const QString &css)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_cssFlatBouton = css; // Attention cette valeur sera écrasée par tout changement dans la feuille de style
    setStyleSheet(m_cssFlatBouton);
    m_newCSSAvailable = false;
}

void AbulEduFlatBoutonV1::setTexteAlignement(const Qt::Alignment &alignement)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_alignement = alignement;
    m_texteAlignement = QString("text-align: ");

    if(alignement.testFlag(Qt::AlignLeft)) {
        m_texteAlignement += QString("left");
    }
    if(alignement.testFlag(Qt::AlignRight)) {
        m_texteAlignement += QString("right");
    }
    if(alignement.testFlag(Qt::AlignCenter)) {
        m_texteAlignement += QString("center");
    }
    if(alignement.testFlag(Qt::AlignJustify)) {
        m_texteAlignement += QString("justify");
    }
    if(alignement.testFlag(Qt::AlignTop)) {
        m_texteAlignement += QString("top");
    }
    if(alignement.testFlag(Qt::AlignBottom)) {
        m_texteAlignement += QString("bottom");
    }
    m_texteAlignement += QString(";");
    /*
    case Qt::AlignVCenter:
        m_texteAlignement = QString("vertical-align: middle;");
        break;
    }
    */
    m_newCSSAvailable = true;
    updateCSS();
}

const Qt::Alignment &AbulEduFlatBoutonV1::getTexteAlignement()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_alignement;
}

void AbulEduFlatBoutonV1::abeFlatBoutonSetFontSize(int pointSize)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_fontSize = pointSize;
    m_newCSSAvailable = true;
    updateCSS();
}

void AbulEduFlatBoutonV1::setBackgroundAlignement(const QString &alignement)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_backgroundAlignement = alignement;
    m_newCSSAvailable = true;
    updateCSS();
}

const QString &AbulEduFlatBoutonV1::getBackgroundAlignement()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_backgroundAlignement;
}

void AbulEduFlatBoutonV1::abeFlatBoutonSetBackgroundAlignement(Qt::Alignment alignement)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    QString returnedAlignment = QString("left");
    /* Test de l'alignement horizontal qui est left par défaut */
    if(alignement.testFlag(Qt::AlignRight)){
        returnedAlignment = QString("right");
    }
    else if(alignement.testFlag(Qt::AlignHCenter)){
        returnedAlignment = QString("center");
    }

    /* Test de l'alignement vertical */
    if(alignement.testFlag(Qt::AlignTop)){
        returnedAlignment.append(" top");
    }
    else if(alignement.testFlag(Qt::AlignBottom)){
        returnedAlignment.append(" bottom");
    }
    if(alignement.testFlag(Qt::AlignVCenter)){
        returnedAlignment.append(" center");
    }
    m_abeFlatBoutonBackgroundAlignment = alignement;
    setBackgroundAlignement(returnedAlignment);
}

Qt::Alignment AbulEduFlatBoutonV1::abeFlatBoutonGetBackgroundAlignement()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_abeFlatBoutonBackgroundAlignment;
}

void AbulEduFlatBoutonV1::setTextePaddingLeft(const qreal &padding)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_paddingLeft = qFloor(padding);
    m_newCSSAvailable = true;
    updateCSS();
}

qreal AbulEduFlatBoutonV1::getTextePaddingLeft()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_paddingLeft;
}

void AbulEduFlatBoutonV1::setTextePaddingRight(const qreal &padding)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_paddingRight = qFloor(padding);
    m_newCSSAvailable = true;
    updateCSS();
}

qreal AbulEduFlatBoutonV1::getTextePaddingRight()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_paddingRight;
}

void AbulEduFlatBoutonV1::setTextePadding(const qreal &left, const qreal &top, const qreal &right, const qreal &bottom)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_paddingLeft   = qFloor(left);
    m_paddingRight  = qFloor(right);
    m_paddingTop    = qFloor(top);
    m_paddingBottom = qFloor(bottom);
    m_newCSSAvailable = true;
    updateCSS();
}

void AbulEduFlatBoutonV1::mouseReleaseEvent(QMouseEvent *e)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    QPushButton::mouseReleaseEvent(e);
    if(m_timer){
        if(m_timer->interval() > 0) {
            m_timer->start();
#ifdef __ABULEDUTABLETTEV1__MODE__
#else
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
#endif
            this->setEnabled(false);
        }
    }
}

void AbulEduFlatBoutonV1::showEvent(QShowEvent *)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    if(m_newCSSAvailable) {
        updateCSS();
    }
}


void AbulEduFlatBoutonV1::setDisableTimeout(int msec)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_timer->setInterval(msec);
    connect(m_timer,SIGNAL(timeout()),this,SLOT(slotDisableTimeout()), Qt::UniqueConnection);
}

void AbulEduFlatBoutonV1::setAllMargins(int top, int right, int bottom, int left)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_marginTop = top;
    m_marginRight = right;
    m_marginBottom = bottom;
    m_marginLeft = left;
    m_newCSSAvailable = true;
    updateCSS();
}

void AbulEduFlatBoutonV1::setSameMargins(int margin)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_marginTop = margin;
    m_marginRight = margin;
    m_marginBottom = margin;
    m_marginLeft = margin;
    m_newCSSAvailable = true;
    updateCSS();
}

void AbulEduFlatBoutonV1::setBorderRadius(int radius)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    m_borderRadius = radius;
    m_newCSSAvailable = true;
    updateCSS();
}

int AbulEduFlatBoutonV1::getBorderRadius()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    return m_borderRadius;
}

void AbulEduFlatBoutonV1::enterEvent(QEvent *)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    emit signalSurvolIn();
}

void AbulEduFlatBoutonV1::leaveEvent(QEvent *)
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    emit signalSurvolOut();
}

void AbulEduFlatBoutonV1::slotDisableTimeout()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    this->setEnabled(true);
#ifdef __ABULEDUTABLETTEV1__MODE__
#else
    QApplication::restoreOverrideCursor();
#endif
    m_timer->stop();
}

void AbulEduFlatBoutonV1::slotClicked()
{
    ABULEDU_LOG_TRACE() <<  __PRETTY_FUNCTION__ << objectName();

    ///@todo: @warning: je ne vois pas trop pourqoi faire un signalSurvolIn sur le Click ...
    emit signalSurvolIn();
}
