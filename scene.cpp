#include "scene.h"

Scene::Scene(QObject *parent) :
    QGraphicsScene()
{
    Q_UNUSED(parent);
}

Scene::~Scene()
{

}

void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalTargetCoordinate(event->scenePos());
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalMousePressed(event->button());
}

void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    emit signalMouseReleased();

    Q_UNUSED(event);
}
