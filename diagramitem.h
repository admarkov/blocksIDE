#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsScene>
#include <QGraphicsPolygonItem>
#include <QPaintEvent>

class DiagramItem : public QGraphicsPolygonItem, public QObject
{
public:

    enum DiagramType {Step, Conditional, StartEnd, IO};

    DiagramItem(DiagramType type, QGraphicsItem *parent = 0);

    DiagramType diagramType() const { return _diagramType; }
    QPolygonF polygon() const { return _polygon; }

private:

    DiagramType _diagramType;
    QPolygonF _polygon;
};

#endif // DIAGRAMITEM_H
