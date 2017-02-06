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
    Recipe(QStringList ingredients, QString titel = "", QString guide = "", QString category = "Hauptgericht");
    Recipe &operator=(const Recipe &other);
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
    QString titel;
    QStringList ingredients;
    QString guide;
    QString category;
    float cost;
    int stars;
};

#endif // RECIPE_H
