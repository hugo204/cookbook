#include "ingredient.h"

Ingredient::Ingredient(QObject *parent) : QObject(parent)
{

}

void Ingredient::add_recipe(Recipe *recipe)
{
    if(!recipeList.contains(recipe)) {
        recipeList.insert(recipeList.count(), recipe);
    }
}

void Ingredient::remove_recipe(Recipe *recipe)
{
    recipeList.removeAll(recipe);
}

float Ingredient::getCost() const
{
    return cost;
}

void Ingredient::setCost(float value)
{
    cost = value;
}

void Ingredient::setCost(QString value)
{
    value.replace(",",".");
    cost = value.simplified().toFloat();
}

int Ingredient::getCalories() const
{
    return calories;
}

void Ingredient::setCalories(int value)
{
    calories = value;
}

int Ingredient::recipeCount()
{
    return recipeList.size();
}
