#include "arrow.h"

#include <QPainter>
#include <QPen>
#include <cmath>
#include <QDebug>
using namespace std;

Arrow::Arrow(DiagramItem *start, DiagramItem *end, QGraphicsItem *parent)
    : QObject(), QGraphicsLineItem(parent)
{
    StartItem = start;
    EndItem = end;
    setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
}

Arrow::~Arrow() {
    EndItem->inArrow = nullptr;
    if (StartItem->outArrow1==this)
        StartItem->outArrow1 = nullptr;
    if (StartItem->outArrow2==this)
        StartItem->outArrow2 = nullptr;
}

QRectF Arrow::boundingRect() const
{
    qreal extra = (pen().width() + 20) / 2.0;

    return QRectF(line().p1(), QSizeF(line().p2().x() - line().p1().x(),
                                      line().p2().y() - line().p1().y()))
        .normalized()
        .adjusted(-extra, -extra, extra, extra);
}

void Arrow::updatePosition()
{
    QPointF startPoint, endPoint = QPointF(0,-(EndItem->height));
    if (StartItem->diagramType()==DiagramItem::Conditional) {
        if (StartItem->outArrow1 == this)
            startPoint = QPointF(-(StartItem->width), 0);
        else
            startPoint = QPointF(StartItem->width, 0);
    }
    else {
        startPoint = QPointF(0, StartItem->height);
    }
    QLineF line(mapFromItem(StartItem, startPoint), mapFromItem(EndItem, endPoint));
    setLine(line);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
    qreal arrowSize = 8;
    painter->setPen(pen());
    painter->setBrush(Qt::black);

    updatePosition();

    double Pi = M_PI;

    double angle = std::acos(line().dx() / line().length());
    if (line().dy() >= 0)
        angle = (Pi * 2) - angle;

    QPointF arrowP1 = line().p2() - QPointF(sin(angle + Pi / 3) * arrowSize, cos(angle + Pi / 3) * arrowSize);
    QPointF arrowP2 = line().p2() - QPointF(sin(angle + Pi - Pi / 3) * arrowSize, cos(angle + Pi - Pi / 3) * arrowSize);

    arrowHead.clear();
    arrowHead << line().p2() << arrowP1 << arrowP2;

    painter->drawLine(line());
    painter->drawPolygon(arrowHead);

}

void Arrow::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    emit clicked(this);
}
