#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QObject>
#include "recipe.h"

class Ingredient : public QObject
{
    Q_OBJECT
public:
    explicit Ingredient(QObject *parent = 0);
    void add_recipe(Recipe * recipe);
    void remove_recipe(Recipe * recipe);
    float getCost() const;
    void setCost(float value);
    void setCost(QString value);
    int getCalories() const;
    void setCalories(int value);
    int recipeCount();

signals:

public slots:

private:
    float cost;
    int calories;
    QList<Recipe *> recipeList;
};

#endif // INGREDIENT_H
