#include "diagramitem.h"
#include <QPainterPath>

DiagramItem::DiagramItem(DiagramType type, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    _diagramType = type;
    switch(diagramType()) {
        case StartEnd: {
            QPainterPath path;
            path.moveTo(200, 50);
            path.arcTo(150, 0, 50, 50, 0, 90);
            path.arcTo(50, 0, 50, 50, 90, 90);
            path.arcTo(50, 50, 50, 50, 180, 90);
            path.arcTo(150, 50, 50, 50, 270, 90);
            path.lineTo(200, 25);
            _polygon = path.toFillPolygon();
            break;
    }
        case Conditional:
            _polygon << QPointF(-70, 0) << QPointF(0, 40) << QPointF(70, 0) << QPointF(0, -40) << QPointF(-70, 0);
            break;
        case Step:
            _polygon << QPointF(-100, -100) << QPointF(100, -100) << QPointF(100, 100) << QPointF(-100, 100) << QPointF(-100, -100);
             break;
        default:
            _polygon << QPointF(-120, -80) << QPointF(-70, 80) << QPointF(120, 80) << QPointF(70, -80) << QPointF(-120, -80);
            break;
    }

    setPolygon(_polygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
