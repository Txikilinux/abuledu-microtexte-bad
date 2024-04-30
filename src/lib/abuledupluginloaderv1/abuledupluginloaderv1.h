/** @class AbulEduPluginLoaderV1
  * @file  abuledupluginloader.h
  *
  * @mainpage   AbulEduPluginLoaderV1
  *             Cette classe gère l'instanciation des différents plugins nécessaires aux logiciels AbulÉdu. \n
  *             Afin d'être unique (singleton), elle hérite de la classe générique AbulEduSingletonV1. \n
  *             AbulEduSingletonV1 est le contrôleur d'instanciation unique. \n
  *             AbulEduPluginLoaderV1 utilise également la généricité afin de charger des plugins quelque soit leur type. \n
  *             Ceci dans le but d'avoir qu'un seul chargeur de plugins pour toutes les librairies. \n
  *
  * @todo   Définir une méthodologie d'instanciation et d'arborescence pour les plugins AbulEdu. \n
  *
  * @author Icham Sirat <icham.sirat@ryxeo.com>
  * @date 2013 09 24
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduPluginLoaderV1
  * @see The GNU Public License (GPL)
  *
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

#ifndef ABULEDUPLUGINLOADERV1_H
#define ABULEDUPLUGINLOADERV1_H

#include <QPluginLoader>
#include <QDir>
#include <QObject>
#include "abuledusingletonv1.h"
#include "abuleduloggerv1.h"

using namespace AbulEduSingleton;

class AbulEduPluginLoaderV1 : public AbulEduSingletonV1<AbulEduPluginLoaderV1> {

    /* Déclaration d'une classe amie, pour avoir accès au constructeur privé */
    friend class AbulEduSingletonV1<AbulEduPluginLoaderV1>;

public :

    /* Interface public */
    /** Méthode accessible de l'extérieur qui charge les plugins
      * @return TRUE si plugins chargés, FALSE sinon
      */
    bool loadPlugins(const QString &pluginsLocation);

    /** Méthode qui renvoie la liste des plugins disponibles
      * @attention Aucun CONTROLE sur la liste, veuillez vous assurer AVANT que la méthode loadPlugins revoie bien TRUE
      * @return QList<QObject*>&, une référence sur une liste contenant des objets<"Plugins">
      */
    const QList<QObject*>& getPluginsAvailable(){
        return _listPlugins;
    }

private:

    /** Constructeur */
    AbulEduPluginLoaderV1(){}

    /** Destructeur */
    ~AbulEduPluginLoaderV1(){}

    /** Méthode qui demande un argument template T pour ne pas créer un chargeur par type de plugin.
      * @param const QString& fileName, le nom du fichier à essayer de charger (i.e. sous linux le chemin vers "fichier.so")
      * @return T*, une instance de plugin.
      */
    template<typename T> T* pluginByName(const QString& fileName) {
        QPluginLoader loader(fileName);         /* chargement du plugin */
        QObject *plugin = loader.instance();    /* instanciation */
        return qobject_cast<T*>(plugin);        /* plugin casté en retour */
    }

    /** Méthode avec un argument template pour récupérer tous les plugins existants dans un dossier.
      * @param const QString& dir, le chemin où sont contenu les fichiers plugins
      * @return QList<T*>, une liste d'objets plugins.
      *
      * @brief la liste ne contient que les plugins qui ont réussi à être instanciés.
      */
    template<typename T> QList<T*> pluginByDir(const QString& dir) {
        QList<T*> ls;
        QDir plugDir(dir);
        /* récupération de la liste des fichiers */
        foreach(QString file, plugDir.entryList(QDir::Files)) {
            if(T* plugin = AbulEduPluginLoaderV1::pluginByName<T>(plugDir.absoluteFilePath(file))) /* vérification */
                ls.push_back(plugin);
        }

        return ls;
    }

    /** @variable _listPlugins, une liste représentant tous les plugins trouvés*/
    QList<QObject*> _listPlugins;
};

#endif // ABULEDUPLUGINLOADER_H
