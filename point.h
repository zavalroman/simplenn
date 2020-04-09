#ifndef POINT_H
#define POINT_H

#include <QObject>

class Point
{
public:
    explicit Point(int x = 0, int y = 0, Qt::MouseButton type = Qt::LeftButton);

    int x;
    int y;
    int type;
signals:

};

#endif // POINT_H
