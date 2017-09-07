#ifndef DIAGRAMITEM_H
#define DIAGRAMITEM_H

#include <QGraphicsScene>
#include <QGraphicsPolygonItem>

class DiagramItem : public QGraphicsPolygonItem
{
public:

    enum DiagramType {Step, Conditional, StartEnd, Io};

    DiagramItem(DiagramType type, QGraphicsItem *parent = 0);

    DiagramType diagramType() const { return _diagramType; }
    QPolygonF polygon() const { return _polygon; }

private:
    DiagramType _diagramType;
    QPolygonF _polygon;
};

#endif // DIAGRAMITEM_H
