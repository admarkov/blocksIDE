#include "diagramitem.h"
#include <QPainterPath>
#include "mainwindow.h"

DiagramItem::DiagramItem(DiagramType type, QGraphicsItem *parent) : QObject(), QGraphicsPolygonItem(parent)
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

    setBrush(QBrush(Qt::white));
    setPolygon(_polygon);
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    textItem = new QGraphicsTextItem(this);
}

DiagramItem::~DiagramItem() {
    delete textItem;
}

void DiagramItem::setText(QString s) {
    textItem->setPlainText(s);
    textItem->setPos(-(textItem->boundingRect().width()/2), -(textItem->boundingRect().height()/2));
}

QString DiagramItem::Text() {
    return textItem->toPlainText();
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemPositionChange && scene()) {
         emit positionChanged(this, value.toPointF());
     }
     return QGraphicsItem::itemChange(change, value);
}

void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked(this);
}
