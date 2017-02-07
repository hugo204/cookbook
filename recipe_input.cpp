#include "recipe_input.h"
#include "ui_recipe_input.h"
#include "mainwindow.h"

Recipe_input::Recipe_input(QWidget *parent, Recipe *recipeFromParent) : QDialog(parent), ui(new Ui::Recipe_input)
{
    ui->setupUi(this);
    //myParent = parent;
    MainWindow *mainwindow = qobject_cast<MainWindow*>(parent);
    recipe_ = recipeFromParent;
    ui->lineEdit_titel->setText(recipe_->getTitel());
    ui->plainTextEdit->insertPlainText(recipe_->getGuide());
    model_ = new QStringListModel(recipe_->getIngredients(), this);

    ui->listView->setModel(model_);
    ui->listView->setEditTriggers(QListView::DoubleClicked);
    ui->lineEdit_titel->setFocus();

    foreach(QString category, mainwindow->cookbook_.get_categoryList()) {
        ui->comboBox->addItem(category);
    }
    ui->comboBox->setCurrentText(recipe_->getCategory());
}

Recipe_input::~Recipe_input()
{
    delete ui;
    delete model_;
    // TODO: remove references to ingredient
}

void Recipe_input::on_toolButton_add_ingredient_clicked()
{
    // add
    int row = model_->rowCount();
    model_->insertRow(row);
    QModelIndex index = model_->index(row);
    ui->listView->setCurrentIndex(index);
    ui->listView->edit(index);
}

void Recipe_input::on_toolButton_edit_ingredient_clicked()
{
    // edit
    ui->listView->edit(ui->listView->currentIndex());
}

void Recipe_input::on_toolButton_delete_ingredient_clicked()
{
    // delete
    model_->removeRow(ui->listView->currentIndex().row());
}

void Recipe_input::on_buttonBox_accepted()
{
    MainWindow *mainwindow = qobject_cast<MainWindow*>(parent());
    // find the ingredients which were deleted by the user
    QStringList ingredientsToDelete;
    if(!recipe_->getIngredients().isEmpty()) {
        foreach(QString item, recipe_->getIngredients()) {
            if(!(model_->stringList().contains(item))) {
                ingredientsToDelete.append(item);
            }
        }
        mainwindow->delete_recipe_from_ingredient(ingredientsToDelete, recipe_);
    }
    // find the ingredients which were added by the user
    QStringList newIngredients;
    foreach(QString item, model_->stringList()) {
        if(!(recipe_->getIngredients().contains(item))) {
            newIngredients.append(item);
        }
    }
    mainwindow->add_recipe_to_ingredient(newIngredients, recipe_);
    recipe_->setGuide(ui->plainTextEdit->toPlainText());
    recipe_->setTitel(ui->lineEdit_titel->text());
    recipe_->setIngredients(model_->stringList());
    recipe_->setCategory(ui->comboBox->currentText());
    // update ingredients
}

void Recipe_input::on_lineEdit_titel_textChanged(const QString &arg1)
{
    MainWindow *mainwindow = qobject_cast<MainWindow*>(parent());
    if(mainwindow->cookbook_.titel_exists(arg1, recipe_)) {
        ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(false);
        ui->lineEdit_titel->setStyleSheet("QLineEdit{background: rgb(255,130,130);}");
    }
    else {
       ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(true);
       ui->lineEdit_titel->setStyleSheet("QLineEdit{background: white;}");
    }
}

void Recipe_input::on_toolButton_add_category_clicked()
{
    bool ok;
    QString text = QInputDialog::getText(this, tr("Kategorie hinzufügen"),
                                         tr("Kategorie:"), QLineEdit::Normal,
                                         "", &ok);
    if(ok && !text.isEmpty()) {
        text = text.simplified();
        MainWindow *mainwindow = qobject_cast<MainWindow*>(parent());
        if(mainwindow->cookbook_.add_category(text) == true) {
            ui->comboBox->addItem(text);
            ui->comboBox->setCurrentText(text);
        }
    }
}
