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
    enum SceneStatus {Normal, DeletingItem, EditingText, AddArrowStart, AddArrowEnd, DeletingArrow} status;
    void selectStatus(SceneStatus newStatus);
    DiagramItem *editing;

private:
    void addDiagramItem(DiagramItem *item);
    QObject *w;
    DiagramItem *arrowStart, *arrowEnd;
    void addSelectedArrow();

public slots:
    void addStartEndItem();
    void addConditionalItem();
    void addStepItem();
    void addIOItem();
    void itemPositionChanged(DiagramItem *item, QPointF newPos);
    void itemClicked(DiagramItem *item);
    void onTextEdited(QString text);
    void addArrow();
    void arrowClicked(Arrow *arrow);
};

#endif // DIAGRAMSCENE_H
