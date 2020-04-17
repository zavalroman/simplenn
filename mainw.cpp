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

    targets = new double[10]{};

    gettimeofday(&start, NULL);
    mSec = 0;
}

MainW::~MainW()
{
    delete ui;
}

void MainW::slotOutputs(double *outputs)
{
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

    int samples = 60000;
    //QList<double*> inputs;
    double **inputs = new double*[samples];
    for (int i = 0; i < samples; i++) {
        //inputs << new double[784];
        inputs[i] = new double[784];
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

            int digit = digits[imgIndex];
            targets[digit] = 1;

            //long sav_mSec = mSec;
            //gettimeofday(&end, NULL);
            //seconds = end.tv_sec - start.tv_sec;
            //useconds = end.tv_usec - start.tv_usec;
            //mSec = ((seconds) * 1000 + useconds / 1000.0 ) + 0.5;

            //qDebug() << "begin" << mSec - sav_mSec;

            double *outputs = nn->feedForward(inputs[imgIndex]);

            //sav_mSec = mSec;
            //gettimeofday(&end, NULL);
            //seconds = end.tv_sec - start.tv_sec;
            //useconds = end.tv_usec - start.tv_usec;
            //mSec = ((seconds) * 1000 + useconds / 1000.0 ) + 0.5;

            //qDebug() << "feed" << mSec - sav_mSec;

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

            //sav_mSec = mSec;
            //gettimeofday(&end, NULL);
            //seconds = end.tv_sec - start.tv_sec;
            //useconds = end.tv_usec - start.tv_usec;
            //mSec = ((seconds) * 1000 + useconds / 1000.0 ) + 0.5;

            //qDebug() << "back" << mSec - sav_mSec;
        }
        long sav_mSec = mSec;
        gettimeofday(&end, NULL);
        seconds = end.tv_sec - start.tv_sec;
        useconds = end.tv_usec - start.tv_usec;
        mSec = ((seconds) * 1000 + useconds / 1000.0 ) + 0.5;

        //qDebug() << "back" << mSec - sav_mSec;

        qDebug() << right << errorSum << mSec - sav_mSec;
    }

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

    timer->start(10);
}


void MainW::on_start_clicked()
{
    RunDigits();
}
