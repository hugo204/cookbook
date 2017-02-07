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
     bool titel_exists(QString, Recipe *recipe = 0);
     bool add_category(QString const category);
     void clear_categoryList();
     QStringList get_categoryList();
     bool save(QString fileName);
     bool open(QString fileName);
private:
     QStringList categoryList_;
};

#endif // COOKBOOK_H
