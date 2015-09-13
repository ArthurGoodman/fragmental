#include "pixel.h"
#include <QtWidgets>
#include "worker.h"
#include "widget.h"

Pixel::Pixel(int x, int y, QColor color)
    : origin(QPoint(x, y)), p(origin), c(color) {
}

QPoint Pixel::pos() {
    return p.toPoint();
}

QColor Pixel::color() {
    return c;
}

void Pixel::update() {
    p += v;
    v += QVector2D(origin.x() - p.x(), origin.y() - p.y()) / 25;
    v /= 1.5;
}

void Pixel::mouseMove(const QVector2D &m) {
    QVector2D delta = m - Widget::getLastPos();
    float d = QVector2D(m).distanceToPoint(p);

    if (d < radius + qrand() % 51 - 25)
        v += QVector2D(qrand() % (range * 2 + 1) - range, qrand() % (range * 2 + 1) - range) * d / radius + delta / 10;
}
