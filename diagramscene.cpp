#include "diagramscene.h"

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

void DiagramScene::addDiagramItem(DiagramItem *item) {
    item->setPos(300,200);
    addItem(item);
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
