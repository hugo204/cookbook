#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sortOrder_des_icon_ = NULL;
    sortOrder_asc_icon_ = NULL;
    filter_activated_icon_ = NULL;
    filter_deactivated_icon_ = NULL;

    // enable drag and drop of list element
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    this->setWindowTitle(tr("New File"));
    ui->actionNew->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionFind->setEnabled(false);
    sortorder_ = SortOrder::AscendingOrder;
    categoryList_ << "Vorspeise" << "Hauptgericht" << "Suppe" << "Nachtisch" << "Brot" << "Kuchen";

    filter_ = new QString;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::control_toolbar(MainWindow::action actionPerformed)
{
    switch(actionPerformed) {
    case item_new:
        ui->actionNew->setEnabled(true);
        ui->actionSave->setEnabled(true);
        ui->actionFind->setEnabled(true);
        break;
    case item_edit:
        ui->actionSave->setEnabled(true);
        break;
    case item_delete:
        if(ui->listWidget->count() == 0) {
            ui->actionFind->setEnabled(false);
        }
        ui->actionSave->setEnabled(true);
        break;
    case document_new:
        ui->actionNew->setEnabled(false);
        ui->actionFind->setEnabled(false);
        ui->actionSave->setEnabled(false);
        break;
    case document_save:
         ui->actionSave->setEnabled(false);
        break;
    case document_load:
        ui->actionNew->setEnabled(true);
        ui->actionSave->setEnabled(false);
        if(ui->listWidget->count() > 0) {
            ui->actionFind->setEnabled(true);
        }
        break;
    }
}

bool MainWindow::titel_exists(QString newTitel, Recipe * recipe)
{
    if(cookbook_.contains(newTitel.simplified())) {
        if(!(cookbook_.value(newTitel) == recipe)){
            return true;
        }
    }
    return false;
}

void MainWindow::add_ingredient(QStringList newIngredients, Recipe * recipe) {
    QStringListIterator i(newIngredients);
    while(i.hasNext()) {
        if(i.peekNext().simplified() == "") {
            // move this to input check
            i.next();
            continue;
        }
        if(!ingredients_.contains(i.peekNext())) {
            Ingredient *ingredient = new Ingredient;
            ingredients_.insert(i.peekNext(), ingredient);
            ingredient->add_recipe(recipe);
        }
        else {
            ingredients_.value(i.peekNext())->add_recipe(recipe);
        }
        i.next();
    }
}

void MainWindow::delete_ingredient(QStringList deletedIngredients, Recipe * recipe ) {
    QStringListIterator i(deletedIngredients);
    while(i.hasNext()) {
        if(ingredients_.contains(i.peekNext())) {
            ingredients_.value(i.peekNext())->remove_recipe(recipe);
        }
        i.next();
    }
}

bool MainWindow::add_category(const QString newCategory)
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

QStringList MainWindow::get_categoryList()
{
    return categoryList_;
}

void MainWindow::on_add_pushButton_clicked()
{
    //window_return_value = dish_input_window->exec();
    Recipe *recipe = new Recipe;
    if(recipe) {
        Recipe_input *recipe_input = new Recipe_input(this, recipe);

        //MainWindow::insert_new_recipe(recipe_input, recipe);
        if(recipe_input->exec() == QDialog::Accepted) {
            cookbook_.insert(recipe->getTitel(), recipe);
            ui->listWidget->insertItem(0, recipe->getTitel());
            ui->listWidget->item(0)->setSelected(true);
            ui->listWidget->setCurrentItem(ui->listWidget->item(0));
            MainWindow::control_toolbar(MainWindow::item_new);
            MainWindow::apply_filter();
            MainWindow::sort_cookbook();
        }
        else {
            qDebug() << "rejected !";
            delete recipe;
        }
        delete recipe_input;
    }
}

void MainWindow::on_delete_pushButton_clicked()
{
    //delete_item
    QListWidgetItem *current_item = ui->listWidget->currentItem();

    if(!current_item) {
        return;
    }

    QMessageBox::StandardButton reply;
    QString message = "Do you want to delete " + current_item->text() + "?";
    reply = QMessageBox::question(this, tr("QMessageBox::question()"),
                                  message,
                                  QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
    if (reply == QMessageBox::Yes) {
        qDebug() << "items removed: ";
        delete cookbook_.take(current_item->text());
        ui->listWidget->takeItem(ui->listWidget->row(current_item));
        ui->statusBar->showMessage(current_item->text() + " " + tr("deleted"), 10000);
        delete current_item;
        MainWindow::control_toolbar(MainWindow::item_delete);
    }
    else if (reply == QMessageBox::No){
    }
    else{
    }
}

void MainWindow::on_edit_pushButton_clicked()
{
    // edit item
    QListWidgetItem *current_item = ui->listWidget->currentItem();
    if(current_item == NULL) {
        return;
    }
    Recipe *recipe = cookbook_.value(current_item->text());
    Recipe_input *recipe_input = new Recipe_input(this, recipe);
    insert_edited_recipe(recipe_input, recipe);
    ui->listWidget->setFocus();
    delete recipe_input;
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    // double click on item
    QListWidgetItem *current_item = ui->listWidget->currentItem();
    if(current_item == NULL) {
        return;
    }
    Recipe *recipe = cookbook_.value(current_item->text());
    Recipe_input *recipe_input = new Recipe_input(this, recipe);
    insert_edited_recipe(recipe_input, recipe);
    ui->listWidget->setFocus();
    delete recipe_input;
}

void MainWindow::insert_edited_recipe(Recipe_input *recipe_input, Recipe *recipe) {
    QListWidgetItem *current_item = ui->listWidget->currentItem();
    if(current_item == NULL) {
        return;
    }
    Recipe *old_recipe = new Recipe;
    *old_recipe = *recipe;
    if(recipe_input->exec() == QDialog::Accepted) {
        cookbook_.take(current_item->text());
        cookbook_.insert(recipe->getTitel(), recipe);
        current_item->setText(recipe->getTitel());
        MainWindow::control_toolbar(MainWindow::item_edit);
        MainWindow::apply_filter();
        MainWindow::sort_cookbook();
    }
    else {
        recipe = old_recipe;
    }
    delete old_recipe;
}

void MainWindow::on_pushButton_clicked()
{
    // show
    QHashIterator<QString, Ingredient *> i(ingredients_);
    while(i.hasNext()) {
        i.next();
        qDebug() << i.key() << i.value()->recipeCount();
        if(i.value()->recipeCount() == 0) {
            qDebug() << "delete ingredient: " << i.key();
            delete i.value();
            ingredients_.remove(i.key());
        }
    }
}

void MainWindow::on_actionNew_triggered()
{
    //new_document
    ui->listWidget->clear();
    this->setWindowTitle("New File");

    QHashIterator<QString, Recipe *> i(cookbook_);
    while(i.hasNext()) {
        i.next();
        delete i.value();
    }
    cookbook_.clear();
    qDebug() << cookbook_.count();

    QHashIterator<QString, Ingredient *> j(ingredients_);
    while(j.hasNext()) {
        j.next();
        delete j.value();
    }
    ingredients_.clear();
    qDebug() << ingredients_.count();
    categoryList_.clear();
    categoryList_ << "Vorspeise" << "Hauptgericht" << "Suppe" << "Nachtisch" << "Brot" << "Kuchen";
    fileName_.clear();
    MainWindow::clear_filter();
    MainWindow::control_toolbar(MainWindow::document_new);
    ui->statusBar->showMessage(tr("new document"), 10000);
}

bool MainWindow::on_actionOpen_triggered()
{
    // open
    MainWindow::on_actionNew_triggered();
    ui->statusBar->showMessage(tr(""), 1);
    this->setWindowTitle("");
    fileName_ = QFileDialog::getOpenFileName(this, tr("Open Cookbook"), QDir::homePath(), tr("Cookbook (*.json)"));
    if(fileName_.isEmpty()) {
        return false;
    }
    qDebug() << fileName_;
    QFile openFile(fileName_);
    if(!openFile.open(QIODevice::ReadWrite)) {
        qDebug() << "Could not open file";
        return false;
    }
    QByteArray loadData = openFile.readAll();
    qDebug() << loadData;
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
        MainWindow::add_category(category);
        jsonIngredientsArray = recipe["ingredients"].toArray();
        ingredients.clear();
        foreach(QVariant ing, jsonIngredientsArray.toVariantList()) {
            ingredients << ing.toString();
        }
        Recipe * newRecipe = new Recipe(ingredients, titel, guide, category);
        cookbook_.insert(titel, newRecipe);
        ui->listWidget->insertItem(cookbook_.count(), titel);
        MainWindow::add_ingredient(newRecipe->getIngredients(), newRecipe);
    }
    this->setWindowTitle(fileName_);
    ui->statusBar->showMessage(tr("document opened"), 10000);
    MainWindow::control_toolbar(MainWindow::document_load);
    MainWindow::sort_cookbook();

    return true;
}

bool MainWindow::on_actionSave_triggered()
{
    //save
    return save_cookbook();
}

bool MainWindow::on_actionSave_as_triggered()
{
    //save
    QString fileNameBackup = fileName_;
    fileName_.clear();

    if(save_cookbook() == false) {
        fileName_ = fileNameBackup;
        return false;
    }
    else {
        return true;
    }
}

bool MainWindow::save_cookbook()
{
    if(fileName_.isEmpty()) {
        fileName_ = QFileDialog::getSaveFileName(this,
                                                tr("Save Cookbook"), "",
                                                tr("Cookbook (*.json);;All Files (*)"));
        if(fileName_.isEmpty()) {
            return false;
        }
    }
    this->setWindowTitle(fileName_);
    QFile saveFile(fileName_);
    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }
    QJsonObject jsonCookBook;
    QJsonObject jsonRecipe;
    QJsonArray jsonRecipeArray;
    QJsonArray ingredients;

    QHashIterator<QString, Recipe *> i(cookbook_);
    while(i.hasNext()) {
        i.next();
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
    MainWindow::control_toolbar(MainWindow::document_save);
    return true;
}

void MainWindow::on_actionFind_triggered()
{
    //search //find
    if(cookbook_.count() < 1) {
        return;
    }
    bool ok;
    QList<QListWidgetItem *> itemList;

    QString text = QInputDialog::getText(this, tr("Search for recipe"),
                                         tr("Recipe:"), QLineEdit::Normal,
                                         "", &ok);
    if (ok && !text.isEmpty()) {
        if(!cookbook_.contains(text)) {
            QMessageBox::information(this, tr("Wrong input"), tr("Recipe \"") + text + tr("\" not found"));
            return;
        }

        itemList = ui->listWidget->findItems(text, Qt::MatchExactly);
        if(itemList.count() == 1) {
            ui->listWidget->setCurrentItem(itemList.first());
        }
        else {
            QMessageBox::critical(this, tr("Critical error"), tr("Critical error"));
            // TODO: Error handling
        }
        Recipe *recipe = cookbook_.value(text);
        Recipe_input *recipe_input = new Recipe_input(this, recipe);
        MainWindow::insert_edited_recipe(recipe_input, recipe);
        delete recipe_input;
    }
    else {

    }
    ui->listWidget->currentItem()->setSelected(true);
}

void MainWindow::on_pushButton_2_clicked()
{
    //sort_order
    if(sortorder_ == SortOrder::DescendingOrder)
    {
       sortorder_ = SortOrder::AscendingOrder;
       MainWindow::sort_cookbook();
       if(!sortOrder_asc_icon_) {
           sortOrder_asc_icon_ = new QIcon(":/icons/icons/thin-0572_down.png");
       }
       ui->pushButton_2->setIcon(*sortOrder_asc_icon_);
    }
    else {
       sortorder_ = SortOrder::DescendingOrder;
       MainWindow::sort_cookbook();
       if(!sortOrder_des_icon_) {
           sortOrder_des_icon_ = new QIcon(":/icons/icons/thin-0573_up.png");
       }
       ui->pushButton_2->setIcon(*sortOrder_des_icon_);
    }
}

void MainWindow::sort_cookbook()
{
    if(sortorder_ == SortOrder::DescendingOrder)
    {
        ui->listWidget->sortItems(Qt::DescendingOrder);
    }
    else {
        ui->listWidget->sortItems(Qt::AscendingOrder);
    }
}

void MainWindow::on_pushButton_3_clicked()
{
    //filter
    Filter *filter_input_window = new Filter(this, filter_);
    filter_input_window->exec();
    apply_filter();
    delete filter_input_window;
}

void MainWindow::apply_filter()
{
    if(!filter_->isEmpty()) {
        ui->listWidget->clear();
        QHashIterator<QString, Recipe *> i(cookbook_);
        while(i.hasNext()) {
            i.next();
            if(i.value()->getCategory() == *filter_) {
                ui->listWidget->insertItem(0, i.key());
            }
            MainWindow::sort_cookbook();
            if(!filter_activated_icon_) {
                filter_activated_icon_ = new QIcon(":/icons/icons/thin-0041_filter_funnel_active.png");
            }
            ui->pushButton_3->setIcon(*filter_activated_icon_);
        }
    }
    else {
        clear_filter();
    }
}

void MainWindow::clear_filter()
{
    *filter_ = "";
    ui->listWidget->clear();
    QHashIterator<QString, Recipe *> i(cookbook_);
    while(i.hasNext()) {
        i.next();
        ui->listWidget->insertItem(0, i.key());
    }
    MainWindow::sort_cookbook();
    if(!filter_deactivated_icon_) {
        filter_deactivated_icon_ = new QIcon(":/icons/icons/thin-0041_filter_funnel.png");
    }
    ui->pushButton_3->setIcon(*filter_deactivated_icon_);
}
