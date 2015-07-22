#pragma once
#include <QtWidgets>
#include <QSoundEffect>
extern bool drawing;
extern std::mt19937 generator;
extern std::uniform_int_distribution<int> distribution;

inline QPolygonF smooth(const QPolygonF& poly, int smoothing)
{
    QPolygonF prev = poly;
    QPolygonF poly2;
    auto middle = [] (QPointF first, QPointF second)
    { return QPointF{(first.x() + second.x()) / 2.,
                     (first.y() + second.y()) / 2.}; };

    for(; smoothing --> 0;)
    {
        QPointF first = prev[0];
        poly2.push_back(first);
        for(int i = 0; i < prev.size() - 1; i++)
        {
            auto second = prev.at(i+1);

            poly2.push_back(middle(first, second));
            poly2.push_back(second);
            first = second;
        }

        // Last one
        poly2.push_back(middle(prev.last(), prev.first()));

        for(int i = 2; (i+1) < poly2.size(); i += 2)
        {
            auto prev = poly2[i-1];
            auto next = poly2[i+1];
            poly2[i] = middle(prev, next);
        }
        poly2[0] = middle(poly2.last(), poly2[1]);

        prev = poly2;
        poly2.clear();

    }
    return prev;
}

class TempShape : public QGraphicsItem
{
    public:
        TempShape(QPointF start, QPointF end, QGraphicsItem* parent):
            QGraphicsItem{parent},
            start{start},
            end{end}
        {
        }

    public:
        QRectF boundingRect() const override
        { return {0, 0, 1398, 876}; }

        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem* option,
                   QWidget* widget) override
        {
            painter->setPen(QPen(Qt::white, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->drawLine(start, end);
        }

    private:
        QPointF start;
        QPointF end;
};


class MyShape : public QGraphicsItem
{
        int hue = distribution(generator);
        QColor pencolor = QColor::fromHsv(hue, 200, 150);
        QColor brushcolor = QColor::fromHsv(hue, 100, 200, 150);
    public:
        MyShape(QVector<QPointF> vector, QGraphicsItem* parent):
            QGraphicsItem{parent},
            m_polygon{vector}
        {
            this->setAcceptedMouseButtons(Qt::LeftButton);

            m_effect.setSource(QUrl::fromLocalFile("/home/jcelerier/travail/SpatialTest2-build/audio/chien/leChien.wav"));
            m_effect.setLoopCount(1);
            m_effect.setVolume(0.35f);

            int smooth = 5;
            for(; smooth --> 0;)
            {
                for(int i = 0; i < m_polygon.size() - 2; i += 2)
                {
                    m_polygon.remove(i);
                }
            }
        }

    public:
        QRectF boundingRect() const override
        { return {0, 0, 1398, 876}; }

        QPainterPath shape() const override
        {
            QPainterPath path;
            path.addPolygon(m_polygon);
            return path;
        }

        void paint(QPainter* painter,
                   const QStyleOptionGraphicsItem* option,
                   QWidget* widget) override
        {
            painter->setPen(QPen(pencolor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter->setBrush(QBrush(brushcolor));

            auto smoothpoly = smooth(m_polygon, 5);
            painter->drawPolygon(smoothpoly);

            /*
            painter->setPen(QPen(Qt::red, 5));
            for(auto pt : smoothpoly)
                painter->drawEllipse(pt, 2, 2);
            */
            /*
            QPainterPath p;
            p.moveTo(m_polygon[0]);
            for(int i = 0; i < m_polygon.size() - 1; i++)
            {
                QRectF rect(m_polygon[i], m_polygon[i+1]);
                p.cubicTo(rect.bottomLeft(), rect.topRight(), m_polygon[i+1]);
            }
            p.quadTo(m_polygon[m_polygon.size()-1], m_polygon[0]);
            painter->drawPath(p);
            */
        }

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent* ev) override
        {
            if(drawing) return;

            m_effect.play();
            QGraphicsItem::mousePressEvent(ev);
        }

    private:
        QPolygonF m_polygon;
        QSoundEffect m_effect;
};

class MyView : public QGraphicsView
{
        Q_OBJECT
        Q_PROPERTY(bool drawing READ drawing WRITE setDrawing NOTIFY drawingChanged)
    public:
        using QGraphicsView::QGraphicsView;

        bool drawing() const
        {
            return m_drawing;
        }

    public slots:
        void setDrawing(bool arg)
        {
            if (m_drawing == arg)
                return;

            m_drawing = arg;
            ::drawing = arg;
            emit drawingChanged(arg);

            if(m_drawing)
                this->setCursor(QCursor(Qt::ArrowCursor));
            else
                this->setCursor(QCursor(Qt::CrossCursor));
        }

    signals:
        void keyPressed();
        void keyReleased();

        void drawingChanged(bool arg);
        void currentPoint(QPointF);

    protected:
        virtual void mousePressEvent(QMouseEvent* m)
        {
            if(drawing())
            {
                // Create Vector
                m_vector.clear();
                m_vector << m->pos();

                m_prevPos = m->pos();
            }
            else
            {
                emit currentPoint(m->pos());
            }

            QGraphicsView::mousePressEvent(m);
        }

        virtual void mouseMoveEvent(QMouseEvent* m)
        {
            if(drawing())
            {
                // Append
                m_vector << m->pos();


                auto shape = new TempShape(m_prevPos, m->pos(), nullptr);
                shape->setZValue(1);
                this->scene()->addItem(shape);
                m_tempShapes.append(shape);
                m_prevPos = m->pos();
            }
            else
            {
                emit currentPoint(m->pos());
            }
        }
        virtual void mouseReleaseEvent(QMouseEvent *m)
        {
            if(drawing())
            {
                for(auto& shape : m_tempShapes)
                {
                    this->scene()->removeItem(shape);
                    delete shape;
                }
                m_tempShapes.clear();

                // Release and emit Vector
                auto shape = new MyShape(m_vector, nullptr);
                shape->setZValue(2);
                this->scene()->addItem(shape);
                m_shapes.append(shape);

                m_vector.clear();
            }
        }

        virtual void keyPressEvent(QKeyEvent* ev) override
        {
            emit keyPressed();
        }

        virtual void keyReleaseEvent(QKeyEvent* ev) override
        {
            emit keyReleased();
        }

    private:
        QVector<QPointF> m_vector;
        QList<TempShape*> m_tempShapes;
        QList<MyShape*> m_shapes;
        bool m_drawing{};
        QPointF m_prevPos;
};
