#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>

#include "scene.h"
#include "dots.h"
#include "digits.h"
#include "neuralnetwork.h"

#include <sys/time.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainW; }
QT_END_NAMESPACE

class MainW : public QMainWindow
{
    Q_OBJECT

public:
    MainW(QWidget *parent = nullptr);
    ~MainW();

public slots:
    void slotOutputs(double*);

private slots:
    void on_runDigits_clicked();

    void on_runDots_clicked();

private:
    Ui::MainW *ui;

    const int w = 1280;
    const int h = 720;

    double *targets;

    QTimer *timer;
    Scene *scene;
    Dots *graphics;
    Digits *digs;

    NeuralNetwork *nn;

    double (*sigmoid )(double) = [](double x)->double { return 1 / (1 + exp(-x)); };
    double (*dsigmoid)(double) = [](double y)->double { return y * (1 - y); };

    void RunDots();
    void RunDigits();

    struct timeval start, end;
    long  mSec, seconds, useconds;
};
#endif // MAINW_H
