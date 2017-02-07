#ifndef INGREDIENT_H
#define INGREDIENT_H

#include <QObject>
#include "recipe.h"

class Ingredient : public QObject
{
    Q_OBJECT
public:
    explicit Ingredient(QObject *parent = 0);
    void add_recipe(Recipe * const recipe);
    void remove_recipe(Recipe * const recipe);
    float getCost() const;
    void setCost(const float value);
    void setCost(QString value);
    int getCalories() const;
    void setCalories(const int value);
    int recipeCount();

signals:

public slots:

private:
    float cost_;
    int calories_;
    QList<Recipe *> recipeList_;
};

#endif // INGREDIENT_H
