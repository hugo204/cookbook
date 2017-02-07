#include "filter.h"
#include "ui_filter.h"

#include "mainwindow.h"

Filter::Filter(QWidget *parent, QString * userInput) :
    QDialog(parent),
    ui(new Ui::Filter)
{
    ui->setupUi(this);
    MainWindow *mainwindow = qobject_cast<MainWindow*>(parent);
    foreach(QString category, mainwindow->cookbook_.get_categoryList()) {
        ui->comboBox->addItem(category);
    }
    input_ =  userInput;
}

Filter::~Filter()
{
    delete ui;
}

void Filter::on_buttonBox_accepted()
{
    *input_ = ui->comboBox->currentText();
}

void Filter::on_pushButton_clicked()
{
    *input_ = "";
    this->reject();
}
