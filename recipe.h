#ifndef RECIPE_H
#define RECIPE_H

#include <QObject>
#include <QString>
#include <QDebug>

class Recipe : public QObject
{
    Q_OBJECT;

public:
    Recipe();
    Recipe(QStringList ingredients_, QString titel_ = "", QString guide_ = "", QString category_ = "Hauptgericht");
    Recipe &operator=(const Recipe &other);
   ~Recipe();
//    Recipe(const Recipe &other);

    QString getTitel() const;
    void setTitel(const QString &value);

    QStringList getIngredients() const;
    void setIngredients(const QStringList &value);

    QString getGuide() const;
    void setGuide(const QString &value);

    QString getCategory() const;
    void setCategory(const QString &value);
    void setCategory(const int &value);

    float getCost() const;
    void setCost(float value);
    void setCost(QString value);

    int getStars() const;
    void setStars(int value);

private:
    QString titel_;
    QStringList ingredients_;
    QString guide_;
    QString category_;
    float cost_;
    int stars_;
};

#endif // RECIPE_H
