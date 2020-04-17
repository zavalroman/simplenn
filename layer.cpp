#include "layer.h"
#include <QDebug>

Layer::Layer(int size, int nextSize)
{
    this->size = size;
    weightSize = size * nextSize;
    neurons = new double[size];
    biases = new double[size];
    weights = new double*[size];
    for(int i=0; i<size; i++) {
        weights[i] = new double[nextSize];
    }
    //for(int i=0; i<size; i++) {
    //    weights.append(QVector<double>(nextSize));
    //}
}

