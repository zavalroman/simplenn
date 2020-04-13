#include "digits.h"

//#include <QDebug>
#include <QRandomGenerator>
#include <QtGlobal>

Digits::Digits(QObject *parent, NeuralNetwork* nn) :
    QObject(parent), QGraphicsItem()
{
    pos = QPointF(0,0);
    this->nn = nn;
    pimg = new QImage(w, h, QImage::Format_RGB32);

    for (int i = 0; i < 28; i++) {
        colors << QVector<double>(28);
    }
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
    double inputs[784];
    for (int i = 0; i < w; i++) {
        for (int j = 0; j < h; j++) {
            if (mousePressed != 0) {
                double dist = (i - mx) * (i - mx) + (j - my) * (j - my);
                if (dist < 1) dist = 1;
                dist *= dist;
                if (mousePressed == 1) colors[i][j] += 0.1 / dist;
                else colors[i][j] -= 0.1 /dist;
                if (colors[i][j] > 1) colors[i][j] = 1;
                if (colors[i][j] < 0) colors[i][j] = 0;
            }
            int color = (int)(colors[i][j] * 255);
            color = (color << 16) | (color << 8) | color;
            pimg->setPixel(i, j, color);
            inputs[i + j * w] = colors[i][j];
        }
    }
    outputs = nn->feedForward(inputs);
    int maxDigit = 0;
    double maxDigitWeight = -1;
    for (int i = 0; i < 10; i++) {
        if(outputs[i] > maxDigitWeight) {
            maxDigitWeight = outputs[i];
            maxDigit = i;
        }
    }
}

void Digits::slotMousePos(QPointF point)
{
    pos = point;
}

void Digits::slotMousePressed(Qt::MouseButton button)
{
    //points << Point(pos.x()-16, pos.y()-38, button);
    //points << Point(pos.x(), pos.y(), button);
}

