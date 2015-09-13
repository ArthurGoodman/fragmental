#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class Worker;

class Widget : public QWidget {
    Q_OBJECT

    static QPoint lastPos;
    QPoint pos;

    static Worker *w;
    QThread *thread;

    QPixmap pixmap;

    int timer;

public:
    Widget();
    ~Widget();

    static QVector2D getLastPos();

protected:
    bool eventFilter(QObject *, QEvent *);
    void timerEvent(QTimerEvent *);
    void resizeEvent(QResizeEvent *);
    void keyPressEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

private slots:
    void updatePixmap(const QImage &buffer);

signals:
    void mouseMove(const QVector2D &m);
};

#endif // WIDGET_H
