#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMouseEvent>
#include <QDebug>
#include <QVector>
#include <QGraphicsItem>
#include "DrawingView.hpp"
#include <QGraphicsPixmapItem>
#include <QStateMachine>
#include <QState>
#include <QEventTransition>

bool drawing{};
std::mt19937 generator;
std::uniform_int_distribution<int> distribution{1,255};

#include <QSoundEffect>
#include <QMediaPlayer>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    QMainWindow w;
    QGraphicsScene* s = new QGraphicsScene;

    auto bg = new QGraphicsPixmapItem(QPixmap{"LaRochelle.png"});
    s->addItem(bg);

    auto v = new MyView(s);

    v->setMinimumSize(1398, 876);
    v->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    v->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    v->setDrawing(true);

    w.setCentralWidget(v);

    QStateMachine* sm = new QStateMachine;
    QState* drawState = new QState;
    drawState->assignProperty(v, "drawing", true);
    QState* moveState = new QState;
    moveState->assignProperty(v, "drawing", false);

    QEventTransition* drawToMove = new QEventTransition(v, QKeyEvent::KeyRelease);
    drawToMove->setTargetState(moveState);
    drawState->addTransition(drawToMove);

    QEventTransition* moveToDraw = new QEventTransition(v, QKeyEvent::KeyPress);
    moveToDraw->setTargetState(drawState);
    moveState->addTransition(moveToDraw);

    sm->addState(drawState);
    sm->addState(moveState);
    sm->setInitialState(moveState);
    sm->start();

    QSoundEffect effect;
    effect.setSource(QUrl::fromLocalFile("/home/jcelerier/travail/SpatialTest2-build/audio/mer/laMer.wav"));
    effect.setLoopCount(QSoundEffect::Infinite);
    effect.setVolume(0.50f);
    effect.play();

    // Comment définir des textures sonores ?
    // Travail avec des limites floues ?
    // Quel est le jeu de paramètre qu'il serait intéressant d'exporter pour contrôle dans i-score?
    w.show();

    return app.exec();
}
