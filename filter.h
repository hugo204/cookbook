#ifndef FILTER_H
#define FILTER_H

#include <QDialog>

namespace Ui {
class Filter;
}

class Filter : public QDialog
{
    Q_OBJECT

public:
    explicit Filter(QWidget *parent = 0, QString * userInput = 0);
    ~Filter();

private slots:
    void on_buttonBox_accepted();
    void on_pushButton_clicked();

private:
    Ui::Filter *ui;
    QString *input_;
};

#endif // FILTER_H
