#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QImage>

class Widget;
class Pixel;

class Worker : public QObject {
    Q_OBJECT

    QPoint offset;

    Widget *w;

    QImage texture, buffer;
    QList<Pixel *> pixels;

    QSize size;

public:
    Worker(Widget *w);
    ~Worker();

    void resize(QSize size);

    QPoint getOffset();

private:
    void createTexture();
    void createPixels();

protected:
    void timerEvent(QTimerEvent *);

public slots:
    void start();

signals:
    void renderedImage(const QImage &buffer);
};

#endif // WORKER_H
