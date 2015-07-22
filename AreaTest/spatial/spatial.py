
import math

from PyQt5.QtCore import (qAbs, QLineF, QPointF, QRectF, qrand, qsrand, Qt,
        QTime, QTimer)
from PyQt5.QtGui import (QBrush, QColor, QPainter, QPainterPath, QPixmap,
        QPolygonF)
from PyQt5.QtWidgets import (QApplication, QGraphicsItem, QGraphicsScene,
        QGraphicsView, QGraphicsWidget)


if __name__ == '__main__':

    import sys

    MouseCount = 7

    app = QApplication(sys.argv)

    scene = QGraphicsScene()
    scene.setSceneRect(-300, -300, 600, 600)
    scene.setItemIndexMethod(QGraphicsScene.NoIndex)


    view = QGraphicsView(scene)
    view.setRenderHint(QPainter.Antialiasing)
    view.setViewportUpdateMode(QGraphicsView.BoundingRectViewportUpdate)
    view.setDragMode(QGraphicsView.ScrollHandDrag)
    view.resize(400, 300)
    view.show()

    sys.exit(app.exec_())
