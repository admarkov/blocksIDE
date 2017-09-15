#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QPaintEvent>
#include <QObject>
#include <QGraphicsTextItem>
#include <QList>

class Arrow;

class DiagramItem :  public QObject, public QGraphicsPolygonItem
{

    Q_OBJECT

public:

    enum DiagramType {Step, Conditional, StartEnd, IO};

    DiagramItem(DiagramType type, QGraphicsItem *parent = 0);
    ~DiagramItem();

    DiagramType diagramType() const { return _diagramType; }
    QPolygonF polygon() const { return _polygon; }

    void setText(QString s);
    QString Text();
    void redraw();

    int width, height;

    QList<Arrow*> inArrows;
    Arrow *outArrow1;
    Arrow *outArrow2;

private:

    QGraphicsTextItem *textItem;
    DiagramType _diagramType;
    QPolygonF _polygon;
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void positionChanged(DiagramItem *item, QPointF newPos);
    void clicked(DiagramItem *item);

};

#endif // DIAGRAMITEM_H
