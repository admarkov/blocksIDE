#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
#include "arrow.h"
#include <QLabel>
#include <QStatusBar>

class DiagramScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit DiagramScene(QObject *parent = 0);
    enum SceneStatus {Normal, DeletingItem, EditingText, AddArrowStart, AddArrowEnd, DeletingArrow} status;
    void selectStatus(SceneStatus newStatus);
    DiagramItem *editingItem;
    Arrow *editingArrow;

private:
    void addDiagramItem(DiagramItem *item);
    QObject *w;
    DiagramItem *arrowStart, *arrowEnd;
    void addSelectedArrow();
    int calcnumber();

public slots:
    void addStartEndItem();
    void addConditionalItem();
    void addStepItem();
    void addIOItem();
    void itemPositionChanged(DiagramItem *item = nullptr, QPointF newPos = QPointF(0,0));
    void itemClicked(DiagramItem *item);
    void onTextEdited(QString text);
    void addArrow();
    void arrowClicked(Arrow *arrow);
};

#endif // DIAGRAMSCENE_H
