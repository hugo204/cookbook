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
    void on_toolButton_3_clicked();
    void on_toolButton_2_clicked();
    void on_buttonBox_accepted();
    void on_toolButton_clicked();
    void on_lineEdit_textChanged(const QString &arg1);
    void on_toolButton_4_clicked();

private:
    Ui::Recipe_input *ui;
    QStringListModel *model_;
    Recipe *recipe_;
};

#endif // RECIPE_OINPUT_H
