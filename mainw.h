#ifndef MAINW_H
#define MAINW_H

#include <QMainWindow>

#include "scene.h"
#include "graphics.h"

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
};
#endif // MAINW_H
