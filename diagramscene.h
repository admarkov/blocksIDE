#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
#include "arrow.h"
#include <QLabel>
#include <QStatusBar>
#include <map>

class DiagramScene : public QGraphicsScene
{

    Q_OBJECT

public:
    explicit DiagramScene(QObject *parent = 0);
    enum SceneStatus {Normal, DeletingItem, EditingText, AddArrowStart, AddArrowEnd, DeletingArrow, RunningManual, RunningAuto} status;
    void selectStatus(SceneStatus newStatus);
    DiagramItem *editingItem;
    Arrow *editingArrow;
    bool check();
    void run();

private:
    bool check_dfs(DiagramItem *item);
    void addDiagramItem(DiagramItem *item);
    QObject *w;
    DiagramItem *arrowStart, *arrowEnd;
    void addSelectedArrow();
    int calcnumber();
    std::map<DiagramItem*, bool> used;

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
