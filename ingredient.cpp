#include "ingredient.h"

Ingredient::Ingredient(QObject *parent) : QObject(parent)
{

}

void Ingredient::add_recipe(Recipe *recipe)
{
    if(!recipeList_.contains(recipe)) {
        recipeList_.insert(recipeList_.count(), recipe);
    }
}

void Ingredient::remove_recipe(Recipe *recipe)
{
    recipeList_.removeAll(recipe);
}

float Ingredient::getCost() const
{
    return cost_;
}

void Ingredient::setCost(float value)
{
    cost_ = value;
}

void Ingredient::setCost(QString value)
{
    value.replace(",",".");
    cost_ = value.simplified().toFloat();
}

int Ingredient::getCalories() const
{
    return calories_;
}

void Ingredient::setCalories(int value)
{
    calories_ = value;
}

int Ingredient::recipeCount()
{
    return recipeList_.size();
}
