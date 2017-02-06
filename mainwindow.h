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
    void add_recipe_to_ingredient(QStringList newIngredients, Recipe *recipe);
    void delete_recipe_from_ingredient(QStringList deletedIngredients, Recipe *recipe);
    bool add_category(QString const category);
    QStringList get_categoryList();

private slots:
    void on_add_pushButton_clicked();
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
    QHash<QString, Recipe *> cookbook_;
    QHash<QString, Ingredient *> ingredients_;
    QStringList categoryList_;
    QString dish_;
    QString fileName_;
    QString *filter_;
    QIcon * sortOrder_des_icon_;
    QIcon * sortOrder_asc_icon_;
    QIcon * filter_activated_icon_;
    QIcon * filter_deactivated_icon_;
    enum SortOrder {
        AscendingOrder,
        DescendingOrder
    } sortorder_;
    enum action {
        item_new,
        item_edit,
        item_delete,
        document_new,
        document_save,
        document_load
    };

    void edit_recipe(Recipe *recipe = 0);
    void control_toolbar(action actionPerformed);
    bool save_cookbook();
    void sort_cookbook();
    void clear_filter();
    void apply_filter();
};

#endif // MAINWINDOW_H
