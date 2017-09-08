#include "diagramitem.h"

DiagramItem::DiagramItem(DiagramType type, QGraphicsItem *parent)
    : QGraphicsPolygonItem(parent)
{
    _diagramType = type;
    switch(diagramType()) {
        case Conditional:
            _polygon << QPointF(-100, 0) << QPointF(0, 100) << QPointF(100, 0) << QPointF(0, -100) << QPointF(-100, 0);
            break;
    }
    setPolygon(_polygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}
