#include "digits.h"

#include <QDebug>
#include <QRandomGenerator>

Digits::Digits(QObject *parent, NeuralNetwork* nn) :
    QObject(parent), QGraphicsItem()
{
    pos = QPointF(0,0);
    this->nn = nn;
    pimg = new QImage(w, h, QImage::Format_RGB32);

    for (int i = 0; i < 28; i++) {
        colors << QVector<double>(28);
        colors.back().fill(0);
    }
    //qDebug() << "Digs constructor ok";
}

Digits::~Digits()
{
    //
}

void Digits::repaint()
{
    Recognition();
    //nn->showWeight();
    this->update();
}

QRectF Digits::boundingRect() const
{
    return QRectF(0, 0, 1280, 720);
}

void Digits::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(QRect(0, 0, w*scale, h*scale), *pimg);

    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Digits::Recognition()
{
    //qDebug() << "recognition start";
    double *inputs = new double[784];
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (mousePressed != 0) {
                //qDebug() << mx << my;
                double dist = (i - mx) * (i - mx) + (j - my) * (j - my);
                //qDebug() << dist;
                if (dist < 1) dist = 1;
                dist *= dist;
                if (mousePressed == 1) {
                    //qDebug() << "adyn" << 0.1 / dist;
                    colors[i][j] += 0.1 / dist;
                } else {
                    //qDebug() << "dva" << 0.1 / dist;
                    colors[i][j] -= 0.1 / dist;
                }
                //qDebug() << colors[i][j];
                if (colors[i][j] > 1) colors[i][j] = 1;
                if (colors[i][j] < 0) colors[i][j] = 0;
            }
            int color = (int)(colors[i][j] * 255);
            color = (color << 16) | (color << 8) | color;
            pimg->setPixel(i, j, color);
            //qDebug() << i + j * w;
            inputs[i + j * w] = colors[i][j];
        }
    }
    //qDebug() << "curson processed";
    //qDebug() << inputs[300] << inputs[301] << inputs[302] << inputs[303] << inputs[304] << inputs[305] <<
    //            inputs[306] << inputs[307] << inputs[308] << inputs[309] << inputs[310] << inputs[311];
    outputs = nn->feedForward(inputs);
    //qDebug() << "recognized";
    delete [] inputs;

    int maxDigit = 0;
    double maxDigitWeight = -1;
    for (int i = 0; i < 10; i++) {
        if(outputs[i] > maxDigitWeight) {
            maxDigitWeight = outputs[i];
            maxDigit = i;
        }
    }
    //qDebug() << "recognition start";
    emit signalOutputs(outputs);
}

void Digits::slotMousePos(QPointF point)
{
    mx = point.x() / scale;
    my = point.y() / scale;
}

void Digits::slotMousePressed(Qt::MouseButton button)
{
    //qDebug() << mx << my;
    mousePressed = 1;
    if(button==Qt::RightButton) mousePressed = 2;
}

void Digits::slotMouseReleased()
{
    mousePressed = 0;
}

