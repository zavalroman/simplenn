#ifndef LAYER_H
#define LAYER_H

#include <QObject>

class Layer
{
public:
    Layer(int size = 1, int nextSize = 1);

    int size;
    double *neurons;
    double *biases;
    QList<QVector<double>> weights;

    void showWeight();

signals:
};

#endif // LAYER_H
