#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class Scene : public QGraphicsScene
{ // https://evileg.com/en/post/103/
    Q_OBJECT
public:
    explicit Scene(QObject *parent = nullptr);
    ~Scene();

signals:
    // The signal of transmit the coordinates of the mouse position
    void signalTargetCoordinate(QPointF point);
    void signalMousePressed(Qt::MouseButton);
    void signalMouseReleased();

public slots:

private:
    // The function, which made tracking the mouse
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // BSCENE_H
