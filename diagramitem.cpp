#include "diagramitem.h"
#include <QPainterPath>
#include <QDebug>
#include <algorithm>
#include "arrow.h"
#include "mainwindow.h"

DiagramItem::DiagramItem(DiagramType type, int num, QGraphicsItem *parent) : QObject(), QGraphicsPolygonItem(parent)
{
    outArrow1 = outArrow2 = nullptr;
    _diagramType = type;
    setBrush(QBrush(Qt::white));
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
    textItem = new QGraphicsTextItem(this);
    number = num;
    redraw();

}

DiagramItem::~DiagramItem() {
    delete textItem;
    for (Arrow* arrow : inArrows) {
        delete arrow;
    }
    inArrows.erase(inArrows.begin(), inArrows.end());
    if (outArrow1!=nullptr) {
        delete outArrow1;
    }
    if (outArrow2!=nullptr) {
        delete outArrow2;
    }
}

void DiagramItem::setText(QString s) {
    textItem->setPlainText(s);
    textItem->setPos(-(textItem->boundingRect().width()/2), -(textItem->boundingRect().height()/2));
}

QString DiagramItem::Text() {
    return textItem->toPlainText();
}

void DiagramItem::redraw() {
    int hw = 80;
    int hh = 40;
    int newlines = Text().count('\n');
    hh+=newlines*8;
    if (diagramType()==IO) {
        int maxlinelen=0;
        int newlinepos=0;
        for (int i=0; i<Text().length(); i++) {
                maxlinelen=std::max(maxlinelen, i-newlinepos);
                if (Text()[i]=='\n')
                    newlinepos=i;
            }
        hw+=std::max(0,maxlinelen-12)*5;
    }
    else if (diagramType()==Step) {
        int maxlinelen=0;
        int newlinepos=0;
        for (int i=0; i<Text().length(); i++) {
                maxlinelen=std::max(maxlinelen, i-newlinepos);
                if (Text()[i]=='\n')
                    newlinepos=i;
            }
        hw+=std::max(0,maxlinelen-14)*3;
    }
    else {
        hw+=std::max(0,Text().length()-14)*3;
    }

    height = hh;
    width = hw;

    _polygon.clear();
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
            _polygon << QPointF(0, hh) << QPointF(hw, 0) << QPointF(0, -hh) << QPointF(-hw, 0) << QPointF(0, hh);
            break;
        case Step:
            _polygon << QPointF(-hw, hh) << QPointF(hw, hh) << QPointF(hw, -hh) << QPointF(-hw, -hh) << QPointF(-hw, hh);
             break;
        default:
            _polygon << QPointF(-hw, hh) << QPointF(hw/2, hh) << QPointF(hw, -hh) << QPointF(-hw/2, -hh) << QPointF(-hw, hh);
            break;
    }
    setPolygon(_polygon);
}

QVariant DiagramItem::itemChange(GraphicsItemChange change, const QVariant &value)
 {
     if (change == ItemPositionChange && scene()) {
         emit positionChanged(this, value.toPointF());
         if (outArrow1!=nullptr)
             outArrow1->updatePosition();
         if (outArrow2!=nullptr)
             outArrow2->updatePosition();
         for (Arrow* inArrow : inArrows) {
             inArrow->updatePosition();
         }
     }
     return QGraphicsItem::itemChange(change, value);
}

void DiagramItem::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked(this);
}
