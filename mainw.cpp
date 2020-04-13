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
    RunDigits();
}

MainW::~MainW()
{
    delete ui;
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
        images.back().load("C:/train/"+f);
        digits << f.split(".")[0].right(1).toInt();
    }

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

    for (int i = 0; i < 1000; ++i) {
        int right = 0;
        double errorSum = 0;
        for (int j = 0; j < 100; ++j) {
            int imgIndex = QRandomGenerator::global()->generateDouble() * samples;
            double *targets = new double[10]; // NEW !!
            int digit = digits[imgIndex];
            targets[digit] = 1;

            double *outputs = nn->feedForward(inputs[imgIndex]);
            int maxDigit = 0;
            double maxDigitWeight = -1;
            for (int k = 0; k < 10; ++k) {
                if(outputs[k] > maxDigitWeight) {
                    maxDigitWeight = outputs[k];
                    maxDigit = k;
                }
            }
            if(digit == maxDigit) right++;
            for (int k = 0; k < 10; ++k) {
                errorSum += (targets[k] - outputs[k]) * (targets[k] - outputs[k]);
            }
            nn->backpropagation(targets);
        }
        qDebug() << right << errorSum;
    }

}

