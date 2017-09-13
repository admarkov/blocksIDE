#include "diagramscene.h"
#include "mainwindow.h"
#include "arrow.h"
#include <QDebug>

DiagramScene::DiagramScene(QObject *parent)
    : QGraphicsScene(parent)
{
    status = SceneStatus::Normal;
    w = parent;
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
        selectStatus(Normal);
    }
    if (status==EditingText) {
        editing = item;
        ((MainWindow*)w)->lineEditor->setEnabled(true);
        ((MainWindow*)w)->lineEditor->setText(editing->Text().replace("\n", ";"));
    }
    if (status==AddArrowStart) {
        if (item->outArrow1==nullptr || item->diagramType()==DiagramItem::Conditional && item->outArrow2==nullptr) {
            arrowStart = item;
            selectStatus(AddArrowEnd);
        }
    }
    if (status==AddArrowEnd) {
        if (item->inArrow==nullptr && item!=arrowStart) {
            arrowEnd = item;
            addSelectedArrow();
            selectStatus(Normal);
        }
    }
}

void DiagramScene::selectStatus(SceneStatus newStatus) {
    MainWindow *W = (MainWindow*)(w);
    if (newStatus == Normal) {
        W->menuFile->setEnabled(true);
        W->menuEdit->setEnabled(true);
        W->statusBar->clearMessage();
        W->lineEditor->hide();
        W->editorbtn->hide();
        //updateRunnable();
    }
    else {
        W->menuFile->setEnabled(false);
        W->menuEdit->setEnabled(false);
        W->menuRun->setEnabled(false);
        if (newStatus==DeletingItem) {
            W->statusBar->showMessage("Нажмите на узел для удаления. Для отмены нажмите Esc.");
        }
        if (newStatus==EditingText) {
            W->statusBar->showMessage("Нажите на узел для редактирования текста. Используйте ; для разделения строк.");
            W->lineEditor->setText("");
            W->lineEditor->show();
            W->editorbtn->show();
            W->lineEditor->setEnabled(false);
            editing = nullptr;
        }
        if (newStatus==AddArrowStart) {
            W->statusBar->showMessage("Нажмите на исходящий узел. Для отмены нажмите Esc.");
        }
        if (newStatus==AddArrowEnd) {
            W->statusBar->showMessage("Нажмите на входящий узел. Для отмены нажмите Esc.");
        }
    }
    status = newStatus;
}

void DiagramScene::onTextEdited(QString text) {
    if (editing!=nullptr) {
        if (editing->diagramType()==DiagramItem::StartEnd || editing->diagramType()==DiagramItem::Conditional) {
            ((MainWindow*)w)->lineEditor->setText(text.replace(";",""));
            editing->setText(text.replace(";", ""));
        }
        if (editing->diagramType()==DiagramItem::IO) {
            int newlinepos=0;
            for (int i=0; i<text.length(); i++)
                if (text[i]==';')
                    newlinepos=i;
            int linelen = text.length()-newlinepos;
            if (linelen>40)
                text.remove(text.length()-1, 1);
            ((MainWindow*)w)->lineEditor->setText(text);
            editing->setText(text);
        }
        editing->setText(text.replace(";", "\n"));
        editing->redraw();
    }
}

void DiagramScene::addArrow() {
    selectStatus(AddArrowStart);
}

void DiagramScene::addSelectedArrow() {
    Arrow *arrow = new Arrow(arrowStart, arrowEnd);
    if (arrowStart->outArrow1!=nullptr)
        arrowStart->outArrow2 = arrow;
    else
        arrowStart->outArrow1 = arrow;
    arrowEnd->inArrow = arrow;
    addItem(arrow);
}
