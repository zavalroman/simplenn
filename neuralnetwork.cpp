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
        //qDebug() << "NEXT" << nextSize;
        layers << Layer(sizes->at(i), nextSize);
        for(int j=0; j<sizes->at(i); j++) {
            layers[i].biases[j] = QRandomGenerator::global()->generateDouble() * 2.0 - 1.0;
            for(int k=0; k<nextSize; k++) {
                layers[i].weights[j][k] = QRandomGenerator::global()->generateDouble() * 2.0 - 1.0;
                //qDebug() << j << k << layers[i].weightSize << j * sizes->at(i) + k;
                //layers[i].weights[j * nextSize + k] = QRandomGenerator::global()->generateDouble() * 2.0 - 1.0;
                //qDebug() << "ok";
            }
        }
    }
}

double *NeuralNetwork::feedForward(double *inputs)
{
    for(int i=0; i<layers[0].size; i++) {
        layers[0].neurons[i] = inputs[i];
    }
    for(int i=1; i<layers.length(); i++) {
        Layer *l = &layers[i-1];
        Layer *l1 = &layers[i];
        for(int j=0; j<l1->size; j++) {
            l1->neurons[j] = 0;
            for(int k=0; k<l->size; k++) {
                //l1->neurons[j] += l->neurons[k] * l->weights[k][j];
                //l1->neurons[j] += l->neurons[k] * l->weights[k * l1->size + j];
                l1->neurons[j] += l->neurons[k] * l->weights[k][j];
            }
            l1->neurons[j] += l1->biases[j];
            l1->neurons[j] = activation(l1->neurons[j]);
        }
    }
    return layers[layers.length()-1].neurons;
}

void NeuralNetwork::backpropagation(double *targets)
{
    double *errors = new double[layers.back().size];
    for(int i=0; i<layers.back().size; i++) {
        errors[i] = targets[i] - layers.back().neurons[i];
    }
    for(int k=layers.length()-2; k>=0; k--) {
        Layer *l = &layers[k];
        Layer *l1 = &layers[k+1];
        double *errorsNext = new double[l->size];
        //double errorsNext[l->size];
        //double *gradients = new double[l1->size];
        double gradients[l1->size];
        for(int i=0; i<l1->size; i++) {
            gradients[i] = errors[i] * derivative(l1->neurons[i]);
            gradients[i] *= learningRate;
        }
        //QVector<QVector<double>> deltas(l1->size);
        //QList<QList<double>> deltas;
        //double *deltas = new double[l1->size * l->size];
        double **deltas = new double*[l1->size];
        //double deltas[l1->size * l->size];
        for(int i=0; i<l1->size; i++) {
            //deltas << QList<double>();
            //deltas[i].resize(l->size);
            deltas[i] = new double[l->size];
            for(int j=0; j<l->size; j++) {
                deltas[i][j] = gradients[i] * l->neurons[j];
                //deltas[i] << gradients[i] * l->neurons[j];
                //deltas[i * l->size + j] = gradients[i] * l->neurons[j];  //1-3 ms
            }
        }
        for(int i=0; i<l->size; i++) {
            errorsNext[i] = 0;
            for(int j=0; j<l1->size; j++) {
                errorsNext[i] += l->weights[i][j] * errors[j];
                //errorsNext[i] += l->weights[i * l1->size + j] * errors[j];
            }
        }
        delete [] errors;
        errors = errorsNext;

        //QList<QVector<double>> weightsNew;
        //for(int j=0; j<l->weights.length(); j++) {
        //     weightsNew << QVector<double>(l->weights[0].length());
        //}
        ///double *weightsNew = new double[l->weightSize];
        for(int i=0; i<l1->size; i++) {
            for(int j=0; j<l->size; j++) {
                //weightsNew[j][i] = l->weights[j][i] + deltas[i * l->size + j];// deltas[i][j];
                //int we_id = 0;//j * l1->size + i;
                //l->weights[j * l1->size + i] += deltas[i * l->size + j];
                //l->weights[j][i] += deltas[i * l->size + j];
                l->weights[j][i] += deltas[i][j];
            }
        }
        ///delete [] l->weights;
        ///l->weights = weightsNew;
        for(int i=0; i<l1->size; i++) {
            l1->biases[i] += gradients[i];
        }
        //delete [] gradients;
        for (int i=0; i<l1->size; i++) {
            delete deltas[i];
        }
        delete [] deltas;

    }
    delete [] errors;
}

