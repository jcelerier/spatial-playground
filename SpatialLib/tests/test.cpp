#include <iostream>
#include <src/Area.hpp>
#include <QGraphicsItem>
#include <QPainterPath>
#include <QPainter>
#include <QApplication>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
class PainterPathDevice
{
    public:
        QVector<QRectF> rects;
        void clear()
        {
            rects.clear();
        }

        template<typename... Args>
        void add(Args&&... args) // A rect
        {
            rects.push_back({std::forward<Args>(args)...});
        }
};

class AreaView : public QGraphicsItem
{
    public:
        QVector<QRectF> rects;

        QRectF boundingRect() const
        {
            return {0, 0, 800, 600};
        }

        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem* ,
                   QWidget*)
        {
            QColor col = Qt::green;
            painter->setPen(col.darker());
            painter->setBrush(col);

            painter->drawRects(rects);
        }
};

using namespace GiNaC;
int main(int argc, char** argv)
{
    QApplication a(argc, argv);
    QMainWindow win;
    QGraphicsScene* s = new QGraphicsScene;

    auto v = new QGraphicsView(s);


    symbol x("x");
    symbol y("y");
    space sp({x, y});

    {
        symbol xv("xv");
        symbol yv("yv");
        symbol x0("x0");
        symbol y0("y0");
        symbol r("r");
        area a1{pow((xv - x0),2) + pow((yv - y0),2) <= pow(r,2),
                {xv, yv},
                {{x0, numeric(400)}, {y0, numeric(400)}, {r, 100}}};

        square_renderer<QPointF, PainterPathDevice> renderer;
        renderer.size = {800, 600};
        renderer.render(a1, sp);

        auto areaview = new AreaView;
        s->addItem(areaview);
        areaview->rects = renderer.render_device.rects;
    }

    {
        symbol xv("xv");
        area a2{
            xv <= 50,
            area::variable_lst{xv},
            area::parameter_map{}};

        square_renderer<QPointF, PainterPathDevice> renderer;
        renderer.size = {800, 600};
        renderer.render(a2, sp);

        auto areaview = new AreaView;
        s->addItem(areaview);
        areaview->rects = renderer.render_device.rects;

    }


    v->setMinimumSize(800, 600);
    v->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    win.setCentralWidget(v);
    win.show();

    v->update();
    return a.exec();
}
