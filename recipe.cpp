#include "recipe.h"

Recipe::Recipe(QStringList ingredients, QString titel, QString guide, QString category)
{
    this->titel = titel;
    this->guide = guide;
    setIngredients(ingredients);
    this->category = category;
}

Recipe::Recipe()
{
    this->category = "Hauptgericht";
}

Recipe &Recipe::operator=(const Recipe &other)
{
    titel = other.getTitel();
    ingredients = other.getIngredients();
    guide = other.getGuide();
    category = other.getCategory();
    cost = other.getCost();
    stars = other.getStars();
}

//Recipe::Recipe(Recipe other)
//{
//    titel = other.getTitel();
//    ingredients = other.getIngredients();
//    guide = other.getGuide();
//    category = other.getCategory();
//    cost = other.getCost();
//    stars = other.getStars();
//}

QString Recipe::getTitel() const
{
    return titel;
}

void Recipe::setTitel(const QString &value)
{
    titel = value.simplified();
}

QStringList Recipe::getIngredients() const
{
    return ingredients;
}

void Recipe::setIngredients(const QStringList &value)
{
    ingredients.clear();
    if(value.isEmpty()) {
        return;
    }
    foreach(QString item, value) {
        if(!item.simplified().isEmpty()){
            ingredients.append(item.simplified());
        }
    }
    // TODO: calculate price
}

QString Recipe::getGuide() const
{
    return guide;
}

void Recipe::setGuide(const QString &value)
{
    guide = value;
}

QString Recipe::getCategory() const
{
    return category;
}

void Recipe::setCategory(const QString &value)
{
    category = value;
}

float Recipe::getCost() const
{
    return cost;
}

int Recipe::getStars() const
{
    return stars;
}

void Recipe::setCost(float value)
{
    cost = value;
}

void Recipe::setCost(QString value)
{
    if(value.isEmpty()) {
        return;
    }
    value.replace(",",".");
    cost = value.simplified().toFloat();
}

void Recipe::setStars(int value)
{
    stars = value;
}
