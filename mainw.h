#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>

#include "scene.h"
#include "graphics.h"
#include "neuralnetwork.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainW; }
QT_END_NAMESPACE

class MainW : public QMainWindow
{
    Q_OBJECT

public:
    MainW(QWidget *parent = nullptr);
    ~MainW();

private:
    Ui::MainW *ui;

    const int w = 1280;
    const int h = 720;

    QTimer *timer;
    Scene *scene;
    Graphics *graphics;

    NeuralNetwork *nn;

    double (*sigmoid )(double) = [](double x)->double { return 1 / (1 + exp(-x)); };
    double (*dsigmoid)(double) = [](double y)->double { return y * (1 - y); };

    void RunDots();
    void RunDigits();
};
#endif // MAINW_H
