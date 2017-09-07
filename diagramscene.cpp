#include "diagramscene.h"

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{

}

void DiagramScene::addDiagramItem(DiagramItem *item) {
    item->setPos(300,200);
    addItem(item);
}

void DiagramScene::addConditionalItem() {
    addDiagramItem(new DiagramItem(DiagramItem::Conditional));
}
