#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <diagramitem.h>
#include <QPainter>

class Arrow : public QGraphicsLineItem
{
public:

    Arrow(DiagramItem *start, DiagramItem *end, QGraphicsItem *parent = 0);
    ~Arrow();
    DiagramItem *StartItem, *EndItem;

    void updatePosition();

    QPainterPath shape() const override ;
    QRectF boundingRect() const override ;

protected:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0) override;

private:

    QPolygonF arrowHead;

};

#endif // ARROW_H
