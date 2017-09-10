#include "diagramscene.h"
#include <QDebug>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

void DiagramScene::addDiagramItem(DiagramItem *item) {
    item->setPos(300,200);
    addItem(item);
    connect(item, SIGNAL(positionChanged(DiagramItem*,QPointF)), this, SLOT(itemPositionChanged(DiagramItem*,QPointF)));
}

void DiagramScene::addStartEndItem() {
    addDiagramItem(new DiagramItem(DiagramItem::StartEnd));
}

void DiagramScene::addConditionalItem() {
    addDiagramItem(new DiagramItem(DiagramItem::Conditional));
}

void DiagramScene::addStepItem() {
    addDiagramItem(new DiagramItem(DiagramItem::Step));
}

void DiagramScene::addIOItem() {
    addDiagramItem(new DiagramItem(DiagramItem::IO));
}

void DiagramScene::itemPositionChanged(DiagramItem *item, QPointF newPos) {
    Q_UNUSED(item);
    qDebug()<<newPos.x()<<" "<<newPos.y()<<"; ";
}
