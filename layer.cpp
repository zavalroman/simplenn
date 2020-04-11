#include "layer.h"
#include <QDebug>

Layer::Layer(int size, int nextSize)
{
    this->size = size;
    neurons = new double[size];
    biases = new double[size];
    for(int i=0; i<size; i++) {
        weights.append(QVector<double>(nextSize));
    }
}

void Layer::showWeight()
{
    for(QVector<double> v : weights) {
        qDebug() << v;
    }
}
