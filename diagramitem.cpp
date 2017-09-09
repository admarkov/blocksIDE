#include "diagramitem.h"
#include <QPainterPath>

DiagramItem::DiagramItem(DiagramType type, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    _diagramType = type;
    switch(diagramType()) {
        case StartEnd: {
            QPainterPath path;
            path.moveTo(-60, -40);
            path.lineTo(40, -40);
            path.arcTo(40,-40,40,40,90,-90);
            path.lineTo(80, 20);
            path.arcTo(40,0,40,40,0,-90);
            path.lineTo(-60,40);
            path.arcTo(-80,0,40,40,-90,-90);
            path.lineTo(-80,-20);
            path.arcTo(-80,-40,40,40,-180,-90);
            _polygon = path.toFillPolygon();
            break;
    }
        case Conditional:
            _polygon << QPointF(0, 40) << QPointF(80, 0) << QPointF(0, -40) << QPointF(-80, 0) << QPointF(0, 40);
            break;
        case Step:
            _polygon << QPointF(-80, 40) << QPointF(80, 40) << QPointF(80, -40) << QPointF(-80, -40) << QPointF(-80, 40);
             break;
        default:
            _polygon << QPointF(-80, 40) << QPointF(40, 40) << QPointF(80, -40) << QPointF(-40, -40) << QPointF(-80, 40);
            break;
    }

    setPolygon(_polygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
