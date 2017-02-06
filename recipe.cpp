#include "recipe.h"

Recipe::Recipe(QStringList ingredients, QString titel, QString guide, QString category)
{
    titel_ = titel;
    guide_ = guide;
    setIngredients(ingredients);
    category_ = category;
}

Recipe::Recipe()
{
    this->category_ = "Hauptgericht";
}

Recipe &Recipe::operator=(const Recipe &other)
{
    titel_ = other.getTitel();
    ingredients_ = other.getIngredients();
    guide_ = other.getGuide();
    category_ = other.getCategory();
    cost_ = other.getCost();
    stars_ = other.getStars();

    return *this;
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
    return titel_;
}

void Recipe::setTitel(const QString &value)
{
    titel_ = value.simplified();
}

QStringList Recipe::getIngredients() const
{
    return ingredients_;
}

void Recipe::setIngredients(const QStringList &value)
{
    ingredients_.clear();
    if(value.isEmpty()) {
        return;
    }
    foreach(QString item, value) {
        if(!item.simplified().isEmpty()){
            ingredients_.append(item.simplified());
        }
    }
    // TODO: calculate price
}

QString Recipe::getGuide() const
{
    return guide_;
}

void Recipe::setGuide(const QString &value)
{
    guide_ = value;
}

QString Recipe::getCategory() const
{
    return category_;
}

void Recipe::setCategory(const QString &value)
{
    category_ = value;
}

float Recipe::getCost() const
{
    return cost_;
}

int Recipe::getStars() const
{
    return stars_;
}

void Recipe::setCost(float value)
{
    cost_ = value;
}

void Recipe::setCost(QString value)
{
    if(value.isEmpty()) {
        return;
    }
    value.replace(",",".");
    cost_ = value.simplified().toFloat();
}

void Recipe::setStars(int value)
{
    stars_ = value;
}
