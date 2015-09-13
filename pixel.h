#ifndef PIXEL_H
#define PIXEL_H

#include <QObject>
#include <QPoint>
#include <QColor>
#include <QVector2D>

class Pixel : public QObject {
    Q_OBJECT

    static const int range = 10, radius = 100;

    QVector2D origin, p, v;
    QColor c;

public:
    Pixel(int x, int y, QColor color);

    QPoint pos();
    QColor color();

    void update();

public slots:
    void mouseMove(const QVector2D &m);
};

#endif // PIXEL_H
