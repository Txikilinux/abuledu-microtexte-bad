/** @class AbulEduSingletonV1
  * @file  abuledusingletonv1.h
  *
  * @mainpage   AbulEduSingletonV1 \n
  *             Cette classe gère l'instanciation unique (singleton) d'objets.\n
  *             Elle est le contrôleur d'instanciation unique.\n
  *             AbulEduSingletonV1 utilise également la généricité afin d'être dérivable pour tout type.\n
  *             Afin de pouvoir mettre en place ce modèle de conception, la classe héritée doit être déclarée en classe "friend"\n
  *
  *@code // Petit exemple de dérivation
  *
  * // De par la généricité, il est obligatoire de spécifier le type entre <...> pour l'héritage
  * class MonSingleton : public AbulEduSingletonV1<MonSingleton>{
  *
  *     // Déclaration d'une classe amie, pour avoir accès au constructeur privé
  *     friend class AbulEduSingletonV1<MonSingleton>;
  *
  *     private:
  *         MonSingleton(){}
  *         ~MonSingleton(){}
  *         int _value;
  *
  *     public:
  *         // Méthodes get & set seulement là pour illustrer le concept
  *         void setValue(int value){ _value = value;}
  *         int getValue(){ return _value;}
  * };
  *
  *     // Pour instancier un objet (le constructeur est privé):
  *     MonSingleton *obj1;
  *     obj1 = MonSingleton::getInstance();
  * @endcode
  *
  * @author Icham Sirat <icham.sirat@ryxeo.com>
  * @date 2013 09 24
  *
  * @see https://redmine.ryxeo.com/projects/ryxeo/wiki/AbulEduSingletonV1
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

#ifndef ABULEDUSINGLETONV1_H
#define ABULEDUSINGLETONV1_H

#include <QObject>

namespace AbulEduSingleton {

template <typename T>
class AbulEduSingletonV1
{

protected:
    /* Constructeur et Destructeur en protected pour réimplémentation dans les classes filles */
    /** Constructeur de la classe */
    AbulEduSingletonV1(){}

    /** Destructeur de la classe */
    ~AbulEduSingletonV1(){}

public:
    /* Interface publique */
    /** Méthode statique qui contrôle l'instanciation unique de l'objet
      * @return T *, un pointeur sur l'instance
      */
    static T *getInstance()
    {
        if(NULL == _singleton){
            // qDebug() << __FUNCTION__ << "Creation Singleton";
            _singleton = new T;
        }
        else{
            // qDebug() << __FUNCTION__ << "Singleton deja existant";
        }
        return (static_cast<T*> (_singleton));
    }

    /** Méthode statique qui contrôle la destruction de l'objet */
    static void kill()
    {
        if(NULL != _singleton)
        {
            delete _singleton;
            _singleton = NULL;
        }
    }

private:

    /** @variable _singleton, représente l'instance unique */
    static T *_singleton;
};

/** @variable _singleton */
template <typename T>
T *AbulEduSingletonV1<T>::_singleton = NULL;

}
#endif // ABULEDUSINGLETONV1_H
