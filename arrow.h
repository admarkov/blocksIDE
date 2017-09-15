#ifndef ARROW_H
#define ARROW_H

#include <QGraphicsLineItem>
#include <diagramitem.h>
#include <QPainter>

class Arrow : public QObject, public QGraphicsLineItem
{

    Q_OBJECT

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

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    QPolygonF arrowHead;

signals:
    void clicked(Arrow *arrow);
};

#endif // ARROW_H
