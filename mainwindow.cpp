#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "filter.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    sort_descending_icon_ = NULL;
    sort_ascending_icon_ = NULL;
    filter_activated_icon_ = NULL;
    filter_deactivated_icon_ = NULL;

    // enable drag and drop of list element
    ui->listWidget->setDragDropMode(QAbstractItemView::InternalMove);
    this->setWindowTitle(tr("New File"));
    ui->action_new->setEnabled(false);
    ui->action_save->setEnabled(false);
    ui->action_find->setEnabled(false);
    sortorder_ = SortOrder::AscendingOrder;
    cookbook_.add_category("Vorspeise");
    cookbook_.add_category("Hauptgericht");
    cookbook_.add_category("Suppe");
    cookbook_.add_category("Nachtisch");
    cookbook_.add_category("Brot");
    cookbook_.add_category("Kuchen");
    filter_ = new QString;
    ui->toolBar_2->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::control_toolbar(MainWindow::action action_performed)
{
    switch(action_performed) {
    case item_new:
        ui->action_new->setEnabled(true);
        ui->action_save->setEnabled(true);
        ui->action_find->setEnabled(true);
        break;
    case item_edit:
        ui->action_save->setEnabled(true);
        break;
    case item_delete:
        if(ui->listWidget->count() == 0) {
            ui->action_find->setEnabled(false);
        }
        ui->action_save->setEnabled(true);
        break;
    case document_new:
        ui->action_new->setEnabled(false);
        ui->action_find->setEnabled(false);
        ui->action_save->setEnabled(false);
        break;
    case document_save:
         ui->action_save->setEnabled(false);
        break;
    case document_load:
        ui->action_new->setEnabled(true);
        ui->action_save->setEnabled(false);
        if(ui->listWidget->count() > 0) {
            ui->action_find->setEnabled(true);
        }
        break;
    }
}

void MainWindow::add_recipe_to_ingredient(QStringList newIngredients, Recipe * recipe) {
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

void MainWindow::delete_recipe_from_ingredient(QStringList deletedIngredients, Recipe * recipe ) {
    QStringListIterator i(deletedIngredients);
    while(i.hasNext()) {
        if(ingredients_.contains(i.peekNext())) {
            ingredients_.value(i.peekNext())->remove_recipe(recipe);
        }
        i.next();
    }
}

void MainWindow::on_pushButton_add_clicked()
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
            MainWindow::sort_listWidget();
        }
        else {
            delete recipe;
        }
        delete recipe_input;
    }
}

void MainWindow::on_pushButton_delete_clicked()
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
        MainWindow::delete_recipe_from_ingredient(cookbook_.take(current_item->text())->getIngredients(), cookbook_.take(current_item->text()));
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

void MainWindow::on_pushButton_edit_clicked()
{
    //edit_item
    edit_recipe();
}

void MainWindow::on_listWidget_doubleClicked(const QModelIndex &index)
{
    //edit_item
    edit_recipe();
}

void MainWindow::edit_recipe(Recipe * recipe) {
    QListWidgetItem *current_item = ui->listWidget->currentItem();
    if(current_item == NULL) {
        return;
    }
    if(recipe == NULL) {
        recipe = cookbook_.value(current_item->text());
    }
    Recipe_input *recipe_input = new Recipe_input(this, recipe);
    Recipe *recipe_copy = new Recipe;
    *recipe_copy = *recipe;
    if(recipe_input->exec() == QDialog::Accepted) {
        cookbook_.take(recipe_copy->getTitel());
        cookbook_.insert(recipe->getTitel(), recipe);
        current_item->setText(recipe->getTitel());
        MainWindow::control_toolbar(MainWindow::item_edit);
        MainWindow::apply_filter();
        MainWindow::sort_listWidget();
    }
    else {
        recipe = recipe_copy;
    }
    ui->listWidget->setFocus();
    delete recipe_input;
    delete recipe_copy;
}

void MainWindow::on_pushButton_calendar_clicked()
{
    //show
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

void MainWindow::on_action_new_triggered()
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

    QHashIterator<QString, Ingredient *> j(ingredients_);
    while(j.hasNext()) {
        j.next();
        delete j.value();
    }
    ingredients_.clear();
    cookbook_.clear_categoryList();
    cookbook_.add_category("Vorspeise");
    cookbook_.add_category("Hauptgericht");
    cookbook_.add_category("Suppe");
    cookbook_.add_category("Nachtisch");
    cookbook_.add_category("Brot");
    cookbook_.add_category("Kuchen");
    fileName_.clear();
    MainWindow::clear_filter();
    MainWindow::control_toolbar(MainWindow::document_new);
    ui->statusBar->showMessage(tr("new document"), 10000);
}

bool MainWindow::on_action_open_triggered()
{
    //open
    MainWindow::on_action_new_triggered();
    ui->statusBar->showMessage(tr(""), 1);
    this->setWindowTitle("");
    fileName_ = QFileDialog::getOpenFileName(this, tr("Open Cookbook"), QDir::homePath(), tr("Cookbook (*.json)"));
    if(fileName_.isEmpty()) {
        return false;
    }
    if(cookbook_.open(fileName_) == true) {
        MainWindow::clear_filter();
        QHashIterator<QString, Recipe *> i(cookbook_);
        while(i.hasNext()) {
            i.next();
            MainWindow::add_recipe_to_ingredient(i.value()->getIngredients(), i.value());
        }
        this->setWindowTitle(fileName_);
        ui->statusBar->showMessage(tr("document opened"), 10000);
        MainWindow::control_toolbar(MainWindow::document_load);
        MainWindow::sort_listWidget();
    }
    return true;
}

bool MainWindow::on_action_save_triggered()
{
    //save
    if(fileName_.isEmpty()) {
        fileName_ = QFileDialog::getSaveFileName(this,
                                                tr("Save Cookbook"), "",
                                                tr("Cookbook (*.json);;All Files (*)"));
        if(fileName_.isEmpty()) {
            return false;
        }
    }
    if(cookbook_.save(fileName_) == true) {
        MainWindow::control_toolbar(MainWindow::document_save);
        return true;
    }
    return false;
}

bool MainWindow::on_action_save_as_triggered()
{
    //save
    QString fileNameBackup = fileName_;
    fileName_.clear();
    if(fileName_.isEmpty()) {
        fileName_ = QFileDialog::getSaveFileName(this,
                                                tr("Save Cookbook"), "",
                                                tr("Cookbook (*.json);;All Files (*)"));
        if(!fileName_.isEmpty()) {
            if(cookbook_.save(fileName_) == false) {
                fileName_ = fileNameBackup;
                return false;
            }
            else {
                this->setWindowTitle(fileName_);
                MainWindow::control_toolbar(MainWindow::document_save);
                return true;
            }
        }
    }
    return false;
}

void MainWindow::on_action_find_triggered()
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
        MainWindow::edit_recipe(recipe);
    }
    else {

    }
}

void MainWindow::on_pushButton_sort_clicked()
{
    //sort_order
    if(sortorder_ == SortOrder::DescendingOrder)
    {
       sortorder_ = SortOrder::AscendingOrder;
       MainWindow::sort_listWidget();
       if(!sort_ascending_icon_) {
           sort_ascending_icon_ = new QIcon(":/icons/icons/thin-0572_down.png");
       }
       ui->pushButton_sort->setIcon(*sort_ascending_icon_);
    }
    else {
       sortorder_ = SortOrder::DescendingOrder;
       MainWindow::sort_listWidget();
       if(!sort_descending_icon_) {
           sort_descending_icon_ = new QIcon(":/icons/icons/thin-0573_up.png");
       }
       ui->pushButton_sort->setIcon(*sort_descending_icon_);
    }
}

void MainWindow::sort_listWidget()
{
    if(sortorder_ == SortOrder::DescendingOrder)
    {
        ui->listWidget->sortItems(Qt::DescendingOrder);
    }
    else {
        ui->listWidget->sortItems(Qt::AscendingOrder);
    }
}

void MainWindow::on_pushButton_filter_clicked()
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
        }
        MainWindow::sort_listWidget();
        if(!filter_activated_icon_) {
            filter_activated_icon_ = new QIcon(":/icons/icons/thin-0041_filter_funnel_active.png");
        }
        ui->pushButton_filter->setIcon(*filter_activated_icon_);
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
    MainWindow::sort_listWidget();
    if(!filter_deactivated_icon_) {
        filter_deactivated_icon_ = new QIcon(":/icons/icons/thin-0041_filter_funnel.png");
    }
    ui->pushButton_filter->setIcon(*filter_deactivated_icon_);
}
