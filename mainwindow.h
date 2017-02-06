#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QInputDialog>
#include <QListWidget>
#include <QFileDialog>

#include "recipe_input.h"
#include "recipe.h"
#include "ingredient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    bool titel_exists(QString, Recipe *recipe = 0);
    void add_ingredient(QStringList newIngredients, Recipe *recipe);
    void delete_ingredient(QStringList deletedIngredients, Recipe *recipe);
    bool add_category(QString const category);
    QStringList get_categoryList();

private slots:
    void on_add_pushButton_clicked();
    //void process_input_from_dish_input_form(int);
    void on_delete_pushButton_clicked();
    bool on_actionOpen_triggered();
    bool on_actionSave_triggered();
    void on_edit_pushButton_clicked();
    void on_pushButton_clicked();
    void on_actionNew_triggered();
    void on_listWidget_doubleClicked(const QModelIndex &index);
    void on_actionFind_triggered();
    bool on_actionSave_as_triggered();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    QHash<QString, Recipe *> cookbook;
    QHash<QString, Ingredient *> ingredients;
    QStringList categoryList;
    QString dish;
    QString fileName;
    QString *filter;
    int cost;
    QIcon * sortOrder_des_icon;
    QIcon * sortOrder_asc_icon;
    QIcon * filter_activated_icon;
    QIcon * filter_deactivated_icon;
    enum SortOrder {
        AscendingOrder,
        DescendingOrder
    } sortorder;
    enum action {
        item_new,
        item_edit,
        item_delete,
        document_new,
        document_save,
        document_load
    };

    void insert_edited_recipe(Recipe_input *recipe_input, Recipe *recipe);
    void controlToolbar(action actionPerformed);
    bool saveCookbook();
    void sortCookbook();
    void clearFilter();
    void applyFilter();
};

#endif // MAINWINDOW_H
