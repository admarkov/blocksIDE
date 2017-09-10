#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QPaintEvent>
#include <QObject>

class DiagramItem :  public QObject, public QGraphicsPolygonItem
{

    Q_OBJECT

public:

    enum DiagramType {Step, Conditional, StartEnd, IO};

    DiagramItem(DiagramType type, QGraphicsItem *parent = 0);

    DiagramType diagramType() const { return _diagramType; }
    QPolygonF polygon() const { return _polygon; }

private:

    DiagramType _diagramType;
    QPolygonF _polygon;

protected:
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

signals:
    void positionChanged(DiagramItem *item, QPointF newPos);

};

#endif // DIAGRAMITEM_H
