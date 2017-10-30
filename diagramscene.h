#ifndef DIAGRAMSCENE_H
#define DIAGRAMSCENE_H

#include <QGraphicsScene>
#include "diagramitem.h"
#include "arrow.h"
#include <QLabel>
#include <QStatusBar>
#include <map>
#include <string>
#include "muParser.h"
using namespace std;
using namespace mu;

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
    void runDFS();

private:
    bool check_dfs(DiagramItem *item);
    void addDiagramItem(DiagramItem *item);
    QObject *w;
    DiagramItem *arrowStart, *arrowEnd;
    void addSelectedArrow();
    int calcnumber();
    std::map<DiagramItem*, bool> used;
    Parser parser;
    map<string, double> varvalue;
    map<string, bool> vardefined;
    DiagramItem *DFSItem;
    void waitASecond();

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
