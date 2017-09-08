#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"

class DiagramScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit DiagramScene(QObject *parent = 0);

private:
    void addDiagramItem(DiagramItem *item);
public slots:
    void addStartEndItem();
    void addConditionalItem();
    void addStepItem();
    void addIOItem();
};

#endif // DIAGRAMSCENE_H
