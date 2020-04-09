#include "mainw.h"
#include "ui_mainw.h"

MainW::MainW(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainW)
{
    ui->setupUi(this);

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

    timer->start(300);
}

MainW::~MainW()
{
    delete ui;
}

