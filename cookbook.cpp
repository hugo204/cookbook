#include "cookbook.h"
#include "recipe.h"

Cookbook::Cookbook() {
}

bool Cookbook::titel_exists(const QString newTitel, Recipe * const recipe)
{
    if(this->contains(newTitel.simplified())) {
        if(!(this->value(newTitel) == recipe)){
            return true;
        }
    }
    return false;
}

bool Cookbook::add_category(const QString newCategory)
{
    if(newCategory.isEmpty()) {
        return false;
    }
    if(!categoryList_.contains(newCategory)) {
        categoryList_ << newCategory;
        return true;
    }
    return false;
}

void Cookbook::clear_categoryList()
{
    categoryList_.clear();
}

QStringList Cookbook::get_categoryList()
{
    return categoryList_;
}

bool Cookbook::open(const QString fileName) {
    if(fileName.isEmpty()){
        return false;
    }
    QFile openFile(fileName);
    if(!openFile.open(QIODevice::ReadWrite)) {
        qDebug() << "Could not open file";
        return false;
    }
    QByteArray loadData = openFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(loadData));

    QString titel;
    QStringList ingredients;
    QString guide;
    QString category;
    QJsonObject recipe;
    QJsonArray jsonIngredientsArray;

    QJsonArray recipeArray = loadDoc.object()["cookbook"].toArray();
    for(int recipeIndex = 0; recipeIndex < recipeArray.size(); ++recipeIndex) {
        recipe = recipeArray[recipeIndex].toObject();
        titel = recipe["titel"].toString();
        guide = recipe["guide"].toString();
        category = recipe["category"].toString();
        this->add_category(category);
        jsonIngredientsArray = recipe["ingredients"].toArray();
        ingredients.clear();
        foreach(QVariant ing, jsonIngredientsArray.toVariantList()) {
            ingredients << ing.toString();
        }
        Recipe * newRecipe = new Recipe(ingredients, titel, guide, category);
        this->insert(titel, newRecipe);
    }
    return true;
}

bool Cookbook::save(QString const fileName)
{
    //save
    if(fileName.isEmpty()) {
        return false;
    }
    QFile saveFile(fileName);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject jsonCookBook;
    QJsonObject jsonRecipe;
    QJsonArray jsonRecipeArray;
    QJsonArray ingredients;

    QHash<QString, Recipe *>::iterator i;
    for(i = this->begin(); i != this->end(); ++i) {
        while(!ingredients.isEmpty()) {
            ingredients.removeFirst();
        }
        foreach(QString ingredient, i.value()->getIngredients()) {
            ingredients.insert(0, ingredient);
        }
        jsonRecipe["guide"] = i.value()->getGuide();
        jsonRecipe["ingredients"] = ingredients;
        jsonRecipe["category"] = i.value()->getCategory();
        jsonRecipe["titel"] = i.value()->getTitel();
        jsonRecipeArray.insert(0, jsonRecipe);
    }
    jsonCookBook["cookbook"] = jsonRecipeArray;
    QJsonDocument saveDoc(jsonCookBook);
    saveFile.write(saveDoc.toJson());
    return true;
}
