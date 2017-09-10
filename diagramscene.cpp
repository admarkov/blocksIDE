#include "diagramscene.h"
#include <QDebug>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    status = SceneStatus::Normal;
}

void DiagramScene::addDiagramItem(DiagramItem *item) {
    item->setPos(100,60);
    addItem(item);
    connect(item, SIGNAL(positionChanged(DiagramItem*,QPointF)), this, SLOT(itemPositionChanged(DiagramItem*,QPointF)));
    connect(item, SIGNAL(clicked(DiagramItem*)), this, SLOT(itemClicked(DiagramItem*)));
}

void DiagramScene::addStartEndItem() {
    addDiagramItem(new DiagramItem(DiagramItem::StartEnd));
    status = Normal;
}

void DiagramScene::addConditionalItem() {
    addDiagramItem(new DiagramItem(DiagramItem::Conditional));
    status = Normal;
}

void DiagramScene::addStepItem() {
    addDiagramItem(new DiagramItem(DiagramItem::Step));
    status = Normal;
}

void DiagramScene::addIOItem() {
    addDiagramItem(new DiagramItem(DiagramItem::IO));
    status = Normal;
}

void DiagramScene::itemPositionChanged(DiagramItem *item, QPointF newPos) {
    Q_UNUSED(item);
    int     top=0,
            bottom=603,
            left=0,
            right=1190;
    bool    changedX = false,
            changedY = false;

    for (auto  it : items()) {
        if (it->x() < left + 80) left = it->x(), changedX = true;
        if (it->x() > right - 80) right = it->x(), changedX = true;
        if (it->y() < top + 40) top = it->y(), changedY = true;
        if (it->y() > bottom -40) bottom = it->y(), changedY = true;
    }
    int     newX = 0,
            newY = top,
            newW = right - left,
            newH = bottom - top;
    if (changedX) {
        newX = left - 100;
        newW = right - left + 200;
        newH -= 20;
    }
    if (changedY) {
        newY = top - 60;
        newH = bottom - top + 140;
    }
    setSceneRect(newX, newY, newW, newH);
}

void DiagramScene::itemClicked(DiagramItem *item) {
    if (status==DeletingItem) {
        removeItem(item);
        delete item;
    }
    status = Normal;
}
