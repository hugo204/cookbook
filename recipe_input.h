#ifndef RECIPE_OINPUT_H
#define RECIPE_OINPUT_H

#include <QtCore>
#include <QtGui>
#include <QDialog>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QPushButton>

#include "recipe.h"

namespace Ui {
class Recipe_input;
}

class Recipe_input : public QDialog
{
    Q_OBJECT

public:
    explicit Recipe_input(QWidget *parent = 0, Recipe *recipeFromParent = 0);
    Recipe_input(const Recipe_input &other);
    Recipe_input &operator=(const Recipe_input &other);
    ~Recipe_input();

private slots:
    void on_toolButton_add_category_clicked();
    void on_buttonBox_accepted();
    void on_pushButton_add_ingredient_clicked();
    void on_pushButton_edit_ingredient_clicked();
    void on_pushButton_delete_ingredient_clicked();
    void on_lineEdit_titel_textChanged(const QString &arg1);


private:
    Ui::Recipe_input *ui;
    QStringListModel *model_;
    Recipe *recipe_;
};

#endif // RECIPE_OINPUT_H
