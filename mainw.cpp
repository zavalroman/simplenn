#include "mainw.h"
#include "ui_mainw.h"

#include <QDir>
#include <QDebug>
#include <QRandomGenerator>

MainW::MainW(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainW)
{
    ui->setupUi(this);

    //RunDots();
    //RunDigits();

}

MainW::~MainW()
{
    delete ui;
}

void MainW::slotOutputs(double *outputs)
{
    //qDebug() << "outputs start";
    ui->bar_0->setValue(outputs[0]*100);
    ui->bar_1->setValue(outputs[1]*100);
    ui->bar_2->setValue(outputs[2]*100);
    ui->bar_3->setValue(outputs[3]*100);
    ui->bar_4->setValue(outputs[4]*100);
    ui->bar_5->setValue(outputs[5]*100);
    ui->bar_6->setValue(outputs[6]*100);
    ui->bar_7->setValue(outputs[7]*100);
    ui->bar_8->setValue(outputs[8]*100);
    ui->bar_9->setValue(outputs[9]*100);
    //qDebug() << "outputs stop";
}

void MainW::RunDots()
{
    this->setFixedSize(w, h);

    timer = new QTimer;
    scene = new Scene;
    graphics = new Graphics;
    scene->addItem(graphics);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setMouseTracking(true);

    connect(scene, &Scene::signalTargetCoordinate, graphics, &Graphics::slotMousePos    );
    connect(scene, &Scene::signalMousePressed,     graphics, &Graphics::slotMousePressed);
    connect(timer, &QTimer::timeout,               graphics, &Graphics::repaint         );

    timer->start(50);
}

void MainW::RunDigits()
{
    QList<int>* sizes = new QList<int>();
    *sizes << 784 << 512 << 128 << 32 << 10;
    nn = new NeuralNetwork(0.001, sigmoid, dsigmoid, sizes);

    QDir train("train");
    QStringList files = train.entryList(QStringList() << "*.png", QDir::Files);
    QList<QImage> images;
    QList<int> digits;
    for (QString f : files) {
        images << QImage();
        images.back().load("train/"+f);
        digits << f.split(".")[0].right(1).toInt();
    }
    qDebug() << "IMAGES LOADED";
    int samples = 60000;
    QList<double*> inputs;
    for (int i = 0; i < samples; i++) {
        inputs << new double[784];
        for (int x = 0;  x < 28; ++x) {
            for (int y = 0; y < 28; ++y) {
                inputs[i][x + y * 28] = (images[i].pixel(x, y) & 0xff) / 255.0;
            }
        }
    }
    qDebug() << "IMAGES PARSED";
    for (int i = 0; i < 10; ++i) {
        int right = 0;
        double errorSum = 0;
        for (int j = 0; j < 100; ++j) {
            int imgIndex = QRandomGenerator::global()->generateDouble() * samples;
            double *targets = new double[10];
            //qDebug() << "one";
            targets[0] = 0.0;
            targets[1] = 0.0;
            targets[2] = 0.0;
            targets[3] = 0.0;
            targets[4] = 0.0;
            targets[5] = 0.0;
            targets[6] = 0.0;
            targets[7] = 0.0;
            targets[8] = 0.0;
            targets[9] = 0.0;
            //qDebug() << "index" << imgIndex;
            int digit = digits[imgIndex];
            //qDebug() << "2.1";
            targets[digit] = 1;
            //qDebug() << "2.2" << inputs[imgIndex][300] << inputs[imgIndex][301] << inputs[imgIndex][302] << inputs[imgIndex][303] << inputs[imgIndex][304] << inputs[imgIndex][305] <<
            //            inputs[imgIndex][306] << inputs[imgIndex][307] << inputs[imgIndex][308] << inputs[imgIndex][309] << inputs[imgIndex][310] << inputs[imgIndex][311];
            double *outputs = nn->feedForward(inputs[imgIndex]);
            //qDebug() << "2.3";
            //qDebug() << outputs[0] << outputs[1] << outputs[2] << outputs[3] << outputs[4] << outputs[5] << outputs[6] << outputs[7] << outputs[8] << outputs[9];
            int maxDigit = 0;
            double maxDigitWeight = -1;
            for (int k = 0; k < 10; ++k) {
                if(outputs[k] > maxDigitWeight) {
                    maxDigitWeight = outputs[k];
                    maxDigit = k;
                }
            }
            //qDebug() << "three";
            if(digit == maxDigit) right++;
            for (int k = 0; k < 10; ++k) {
                errorSum += (targets[k] - outputs[k]) * (targets[k] - outputs[k]);
            }
            nn->backpropagation(targets);
            //qDebug() << "five";
        }
        qDebug() << right << errorSum;
    }
    qDebug() << "LEARNED";

    timer = new QTimer;
    scene = new Scene;
    digs = new Digits(this, nn);
    scene->addItem(digs);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);
    ui->graphicsView->setMouseTracking(true);

    connect(scene, &Scene::signalTargetCoordinate, digs, &Digits::slotMousePos      );
    connect(scene, &Scene::signalMousePressed,     digs, &Digits::slotMousePressed  );
    connect(scene, &Scene::signalMouseReleased,    digs, &Digits::slotMouseReleased );
    connect(digs,  &Digits::signalOutputs,         this, &MainW::slotOutputs        );
    connect(timer, &QTimer::timeout,               digs, &Digits::repaint           );

    timer->start(50);

}


void MainW::on_start_clicked()
{
    RunDigits();
}
