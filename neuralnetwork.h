#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include <QObject>
#include "layer.h"

class NeuralNetwork
{
public:
    explicit NeuralNetwork(double learningRate = 0.0f, double (*activation)(double) = nullptr,
                           double (*derivative)(double) = nullptr, QList<int>* sizes = nullptr);

    double* feedForward(double*);
    void backpropagation(double*);
    void showWeight();

private:
    double learningRate;
    double (*activation)(double);
    double (*derivative)(double);

    QList<Layer> layers;
};

#endif // NEURALNETWORK_H
