#include <QApplication>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>

#include <functional>
#include <vector>

#include <random>
inline int32_t getNextId()
{
    using namespace std;
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_int_distribution<int32_t>
            dist(numeric_limits<int32_t>::min(),
                 numeric_limits<int32_t>::max());

    return dist(gen);
}

const int w = 1000;
const int h = 800;

inline double getRandDouble()
{
    using namespace std;
    static random_device rd;
    static mt19937 gen(rd());
    static uniform_real_distribution<double>
            dist(0, 1);

    return dist(gen);
}

class Fun
{
    public:
        template<typename TheFun>
        Fun(TheFun&& thefun):
            f{std::move(thefun)}
        {

        }

        std::function<bool(double,double)> f;
        bool operator()(double a, double b)
        {
            return f(a, b);
        }
};

class Space : public QGraphicsItem
{
    public:
        std::vector<Fun> functions;

    public:
        QRectF boundingRect() const
        {
            return {0, 0, w, h};
        }

        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem* ,
                   QWidget* )
        {
            QVector<QPointF> points;
            for(auto&& fun : functions)
            {
                QColor col = QColor::fromHslF(getRandDouble(),  1. - 0.6 * getRandDouble(), 0.6 + 0.2 * getRandDouble());

                for(double x = 0; x < w; x+=10)
                {
                    for(double y = 0; y < h; y+=10)
                    {
                        if(fun(x, y))
                        {
                            painter->setPen(col.darker());
                            painter->setBrush(col);
                            painter->drawRect(QRectF{x-5, y-5, 10, 10});

                            QColor shitsu = col.darker(120);
                            painter->setBrush(shitsu);
                            painter->setPen(shitsu);
                            painter->drawEllipse(QPointF(x, y), 2, 2);

                            painter->setPen(Qt::black);
                            painter->drawPoint(x, y);
                        }
                    }
                }
            }
        }
};



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QMainWindow win;
    QGraphicsScene* s = new QGraphicsScene;

    auto v = new QGraphicsView(s);

    auto space = new Space;
    s->addItem(space);


    //space->functions.push_back([] (double x, double y)
    //{ return getRandDouble() * x + getRandDouble() * y < getRandDouble() * x; } );

    //space->functions.push_back([] (double x, double y)
    //{ return getRandDouble() * x + getRandDouble() * y < getRandDouble() * x; } );

    space->functions.push_back([] (double x, double y)
    { return y >= 2*x + 50; } );

    QPolygonF poly{{{100, 100}, {250, 100}, {750, 500}, {400, 700}}};
    space->functions.push_back([=] (double x, double y)
    {
        return poly.containsPoint(QPointF{x, y}, Qt::WindingFill);
    } );

    space->functions.push_back([] (double x, double y)
    { return pow(x - 300, 2) + pow(y - 300, 2) <= pow(200, 2); } );
    space->functions.push_back([] (double x, double y)
    { return pow(x - 300, 2) + pow(y - 300, 2) <= pow(50, 2); } );


    v->setMinimumSize(w, h);
    v->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    //v->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);


    win.setCentralWidget(v);
    win.show();


    return a.exec();
}
