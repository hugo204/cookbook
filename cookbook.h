#ifndef COOKBOOK_H
#define COOKBOOK_H

#include <QtCore>
#include <QHashIterator>
#include "recipe.h"

class Cookbook : public QObject, public QHash<QString, Recipe *>
{
    Q_OBJECT;
public:    
    Cookbook();
     bool titel_exists(const QString newTitel, Recipe * const recipe = 0);
     bool add_category(const QString category);
     void clear_categoryList();
     QStringList get_categoryList();
     bool save(const QString fileName);
     bool open(const QString fileName);
private:
     QStringList categoryList_;
};

#endif // COOKBOOK_H
