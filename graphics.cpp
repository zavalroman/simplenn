#include "graphics.h"

//#include <QDebug>
#include <QRandomGenerator>
#include <QtGlobal>

Graphics::Graphics(QObject *parent, int w, int h) :
    QObject(parent), QGraphicsItem()
{
    this->w = w;
    this->h = h;

    pos = QPointF(0,0);

    QList<int>* sizes = new QList<int>();
    *sizes << 2 << 5 << 5 << 2;
    nn = new NeuralNetwork(0.01, sigmoid, dsigmoid, sizes);

    pimg = new QImage(w/8, h/8, QImage::Format_RGB32);
}

Graphics::~Graphics()
{
    //
}

void Graphics::repaint()
{
    Learning();
    //nn->showWeight();
    this->update();
}

QRectF Graphics::boundingRect() const
{
    return QRectF(0, 0, 1280, 720);
}

void Graphics::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->drawImage(QRect(0, 0, w, h), *pimg);

    for(Point p : points) {
        painter->setPen(QPen(Qt::white, 5));
        painter->drawEllipse(p.x-3, p.y-3, 26, 26);
        if(p.type==Qt::LeftButton) {
            painter->setPen(QPen(Qt::green, 5));
        } else {
            painter->setPen(QPen(Qt::blue, 5));
        }
        painter->drawEllipse(p.x, p.y, 20, 20);
    }
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

void Graphics::Learning()
{
    if(points.size()>0) {
        for(int k=0; k<10000; k++) {
            Point p = points[QRandomGenerator::global()->generateDouble()*points.size()];
            double nx = (double) p.x / w - 0.5;
            double ny = (double) p.y / h - 0.5;
            nn->feedForward(new double[2]{nx, ny});
            double *targets = new double[2]{0, 0}; // make global
            if(p.type==Qt::LeftButton) targets[0] = 1;
            else targets[1] = 1;
            nn->backpropagation(targets);
            delete [] targets; // make global
        }
    }
    for(int i=0; i<w/8; i++) {
        for(int j=0; j<h/8; j++) {
            double nx = (double) i / w * 8 - 0.5;
            double ny = (double) j / h * 8 - 0.5;
            double *inputs = new double[2]{nx, ny};
            double *outputs = nn->feedForward(inputs);
            delete [] inputs;
            double green = qMax((double)0, qMin((double)1, outputs[0]-outputs[1]+0.5));
            double blue  = 1 - green;
            //double green = outputs[0];
            //double blue = outputs[1];
            green = 0.3 + green * 0.5;
            blue  = 0.5 + blue  * 0.5;
            uint color = (100 << 16) | ((int)(green*255) << 8) | (int)(blue*255);
            pimg->setPixel(i, j, color);
        }
    }
}

void Graphics::slotMousePos(QPointF point)
{
    pos = point;
}

void Graphics::slotMousePressed(Qt::MouseButton button)
{
    //points << Point(pos.x()-16, pos.y()-38, button);
    points << Point(pos.x(), pos.y(), button);
}

