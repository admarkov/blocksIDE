#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
#include <QLabel>
#include <QStatusBar>

class DiagramScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit DiagramScene(QObject *parent = 0);
    enum SceneStatus {Normal, DeletingItem} status;
    void selectStatus(SceneStatus newStatus);

private:
    void addDiagramItem(DiagramItem *item);
    QObject *w;

public slots:
    void addStartEndItem();
    void addConditionalItem();
    void addStepItem();
    void addIOItem();
    void itemPositionChanged(DiagramItem *item, QPointF newPos);
    void itemClicked(DiagramItem *item);
};

#endif // DIAGRAMSCENE_H
