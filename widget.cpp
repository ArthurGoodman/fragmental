#include "widget.h"
#include <QtWidgets>
#include "worker.h"

QPoint Widget::lastPos;
Worker *Widget::w;

Widget::Widget() {
    qsrand(QTime::currentTime().msec());
    setMouseTracking(true);

    w = new Worker(this);
    thread = new QThread;
    connect(thread, SIGNAL(started()), w, SLOT(start()));
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    connect(thread, SIGNAL(finished()), w, SLOT(deleteLater()));
    connect(w, SIGNAL(renderedImage(QImage)), SLOT(updatePixmap(QImage)));
    w->moveToThread(thread);
    thread->start();

    installEventFilter(this);
    //startTimer(16);
}

Widget::~Widget() {
    thread->deleteLater();
    w->deleteLater();
}

QVector2D Widget::getLastPos() {
    return QVector2D(lastPos - w->getOffset());
}

bool Widget::eventFilter(QObject *o, QEvent *e) {
    if (o == this && (e->type() == QEvent::Enter || e->type() == QEvent::Leave)) {
        if (e->type() == QEvent::Enter)
            timer = startTimer(16);
        else
            killTimer(timer);
    }

    return false;
}

void Widget::timerEvent(QTimerEvent *) {
    emit mouseMove(QVector2D(pos - w->getOffset()));
}

void Widget::resizeEvent(QResizeEvent *) {
    w->resize(size());
}

void Widget::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
    case Qt::Key_Escape:
        isFullScreen() ? showNormal() : (void)close();
        break;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *e) {
    lastPos = pos;
    pos = e->pos();
}

void Widget::paintEvent(QPaintEvent *) {
    QPainter p(this);
    p.drawPixmap(0, 0, pixmap);
}

void Widget::updatePixmap(const QImage &buffer) {
    pixmap = QPixmap::fromImage(buffer);
    repaint();
}
