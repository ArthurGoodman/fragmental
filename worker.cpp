#include "worker.h"
#include <QtWidgets>
#include "widget.h"
#include "pixel.h"

Worker::Worker(Widget *w)
    : w(w) {
    createTexture();
    createPixels();
}

Worker::~Worker() {
    foreach (Pixel *pixel, pixels)
        delete pixel;
}

void Worker::resize(QSize size) {
    this->size = size;
}

QPoint Worker::getOffset() {
    return offset;
}

void Worker::createTexture() {
    QSettings s("settings.ini", QSettings::IniFormat);

    QFont font = QFont(s.value("font", "Helvetica").toString(), s.value("pointSize", 75).toInt());
    QColor color = s.value("color", QColor(Qt::white)).toString();
    QString text = s.value("text", "Hello, World!").toString();

    s.setValue("font", font.family());
    s.setValue("pointSize", font.pointSize());
    s.setValue("color", QVariant(color).toString());
    s.setValue("text", text);

    texture = QImage(QFontMetrics(font).boundingRect(text).size() + QSize(20, 20), QImage::Format_ARGB32);
    texture.fill(Qt::darkGray);

    QPainter p(&texture);
    p.setFont(font);
    p.setPen(color);
    p.drawText(texture.rect(), Qt::AlignCenter, text);

    w->setMinimumSize(texture.size());
}

void Worker::createPixels() {
    for (int x = 0; x < texture.width(); ++x)
        for (int y = 0; y < texture.height(); ++y)
            if (texture.pixel(x, y) != QColor(Qt::darkGray).rgb()) {
                pixels << new Pixel(x, y, texture.pixel(x, y));
                connect(w, SIGNAL(mouseMove(QVector2D)), pixels.last(), SLOT(mouseMove(QVector2D)));
            }
}

void Worker::timerEvent(QTimerEvent *) {
    if (size != buffer.size()) {
        buffer = QImage(size, QImage::Format_ARGB32);
        offset = buffer.rect().center() - texture.rect().center();
    }

    buffer.fill(Qt::darkGray);

    if (buffer.isNull())
        return;

    QPainter p(&buffer);

    QPoint point;
    foreach (Pixel *pixel, pixels) {
        pixel->update();
        point = pixel->pos() + offset;
        if (!buffer.valid(point))
            continue;
        p.setPen(pixel->color());
        p.drawPoint(point);
    }

    emit renderedImage(buffer);
}

void Worker::start() {
    startTimer(16);
}
