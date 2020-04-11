#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QPointF>
#include <QTimer>
#include <cmath>

#include <QDebug>

#include "neuralnetwork.h"
#include "point.h"

class Graphics : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Graphics(QObject *parent = nullptr, int width = 1280, int height = 720);
    ~Graphics();

signals:

public slots:
    void repaint();
    void slotMousePos(QPointF);
    void slotMousePressed(Qt::MouseButton);

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private slots:

private:
    int w, h;

    QPointF pos;

    double (*sigmoid )(double) = [](double x)->double { return 1 / (1 + exp(-x)); };
    double (*dsigmoid)(double) = [](double y)->double { return y * (1 - y); };

    QList<Point> points;

    NeuralNetwork *nn;

    QImage *pimg;

    void Learning();
};

#endif // GRAPHICS_H
