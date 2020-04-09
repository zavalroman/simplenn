#include "neuralnetwork.h"

#include <QRandomGenerator>
#include <QDebug>

NeuralNetwork::NeuralNetwork(double learningRate, double (*activation)(double),
                             double (*derivative)(double), QList<int> *sizes)
{
    this->learningRate = learningRate;
    this->activation = activation;
    this->derivative = derivative;

    for(int i=0; i<sizes->length(); i++) {
        int nextSize = 0;
        if(i<sizes->length()-1) nextSize = sizes->at(i+1);
        layers << Layer(sizes->at(i), nextSize);
        for(int j=0; j<sizes->at(i); j++) {
            layers[i].biases[j] = QRandomGenerator::global()->bounded(1.0) * 2.0 - 1.0;
            for(int k=0; k<nextSize; k++) {
                layers[i].weights[j][k] = QRandomGenerator::global()->bounded(1.0) * 2.0 - 1.0;
            }
        }
    }
}

double *NeuralNetwork::feedForward(double *inputs)
{
    layers[0].neurons = inputs;
    for(int i=1; i<layers.length(); i++) {
        Layer *l = &layers[i - 1];
        Layer *l1 = &layers[i];
        for(int j=0; j<l1->size; j++) {
            l1->neurons[j] = 0;
            for(int k=0; k<l->size; k++) {
                l1->neurons[j] += l->neurons[k] * l->weights[k][j];
            }
            l1->neurons[j] += l1->biases[j];
            l1->neurons[j] = activation(l1->neurons[j]);
        }
    }
    delete [] inputs;
    return layers[layers.length()-1].neurons;
}

void NeuralNetwork::backpropagation(double *targets)
{
    double *errors = new double[layers.back().size]; // DELETED ON 72
    for(int i=0; i<layers.back().size; i++) {
        errors[i] = targets[i] - layers[layers.length()-1].neurons[i];
    }
    for(int k=layers.length()-2; k>=0; k--) {
        Layer *l = &layers[k];
        Layer *l1 = &layers[k+1];
        double *errorsNext = new double[l->size];   // DELETED ON 93
        double *gradients = new double[l1->size];   // DELETED ON 91
        for(int i=0; i<l1->size; i++) {
            gradients[i] = errors[i] * derivative(layers[k+1].neurons[i]);
            gradients[i] *= learningRate;
        }
        QList<QList<double>> deltas;
        for(int i=0; i<l1->size; i++) {
            deltas << QList<double>();
            for(int j=0; j<l->size; j++) {
                deltas[i] << gradients[i] * l->neurons[j];
            }
        }
        for(int i=0; i<l->size; i++) {
            errorsNext[i] = 0;
            for(int j=0; j<l1->size; j++) {
                errorsNext[i] += l->weights[i][j] * errors[j];
            }
        }
        delete [] errors;
        errors = errorsNext;
        //qDebug() << "+++++++++++++++++++++++++++++++++++";
        //qDebug() << "errors" << errors[0] << errors[1] << errors[3] << errors[4] << errors[5];
        //qDebug() << "errorsNext" << errorsNext[0] << errorsNext[1] << errorsNext[3] << errorsNext[4] << errorsNext[5];

        QList<QVector<double>> weightsNew;
        for(int j=0; j<l->weights.length(); j++) {
             weightsNew << QVector<double>(l->weights[0].length());
        }
        for(int i=0; i<l1->size; i++) {        
            for(int j=0; j<l->size; j++) {
                weightsNew[j][i] = l->weights[j][i] + deltas[i][j];
            }
        }
        l->weights = weightsNew;
        for(int i=0; i<l1->size; i++) {
            l1->biases[i] += gradients[i];
        }
        delete [] gradients;
    }
    delete [] errors;
    delete [] targets;

}
