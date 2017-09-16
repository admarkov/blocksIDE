#include "arrow.h"
#include <QPainter>
#include <QPen>
#include <cmath>
#include <QDebug>
using namespace std;

ArrowText::ArrowText(Arrow *arr,QGraphicsItem *parent)
    : QGraphicsTextItem(parent), arrow(arr)
{

}

void ArrowText::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    arrow->mousePressEvent(event);
}

Arrow::Arrow(DiagramItem *start, DiagramItem *end, QGraphicsItem *parent)
    : QObject(), QGraphicsLineItem(parent)
{
    StartItem = start;
    EndItem = end;
    setPen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    textItem = new ArrowText(this, this);
//    textItem->setPlainText("true");
    updatePosition();
}

Arrow::~Arrow() {
    QList<Arrow*>::iterator toErase;
    for (auto it = EndItem->inArrows.begin(); it!=EndItem->inArrows.end(); it++)
        if ((*it)==this)
            toErase = it;
    EndItem->inArrows.erase(toErase, toErase+1);
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
    qreal X = line.angle();
    //qDebug()<<(startPoint.x()+endPoint.x())/2.<<(startPoint.y()+endPoint.y())/2.;
    //qDebug()<<atan((startPoint.y()-endPoint.y())/(startPoint.x()-endPoint.x()));
    //qDebug()<<line.angle();
    textItem->setPos((startPoint.x()+endPoint.x())/2., (startPoint.y()+endPoint.y())/2.);
    qreal angle = line.angle();
    if (angle<=90 || angle>=270)
        textItem->setRotation(-angle);
    else
        textItem->setRotation(-180-angle);
    textItem->setPos((line.x1()+line.x2())/2., (line.y1()+line.y2())/2.);
}

QPainterPath Arrow::shape() const
{
    QPainterPath path = QGraphicsLineItem::shape();
    path.addPolygon(arrowHead);
    return path;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget ) {
    updatePosition();
    qreal arrowSize = 8;
    painter->setPen(pen());
    painter->setBrush(Qt::black);

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
