#ifndef DIGITS_H
#define DIGITS_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>

#include "neuralnetwork.h"

class Digits : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit Digits(QObject *parent = nullptr, NeuralNetwork *nn = nullptr);
    ~Digits();

signals:
    void signalOutputs(double*);

public slots:
    void repaint();
    void slotMousePos(QPointF);
    void slotMousePressed(Qt::MouseButton);
    void slotMouseReleased();

private:
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private slots:

private:
    const int w = 28, h = 28;
    const int scale = 32;

    double* outputs;

    int mousePressed = 0;
    int mx = 0;
    int my = 0;
    QList<QVector<double>> colors;

    QPointF pos;

    NeuralNetwork *nn;

    QImage *pimg;

    void Recognition();
};

#endif // DIGITS_H
