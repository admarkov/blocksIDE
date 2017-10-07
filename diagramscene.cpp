#include "diagramscene.h"
#include "mainwindow.h"
#include "arrow.h"
#include <QDebug>
#include <QtGui>
#include <QInputDialog>
#include <QMessageBox>

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

int DiagramScene::calcnumber() {
    int n = 0;
    bool free=true;
    do {
        n++;
        free = true;
        for (QGraphicsItem *it : items()) {
            if (dynamic_cast<DiagramItem*>(it)) {
                DiagramItem *item = dynamic_cast<DiagramItem*>(it);
                if (item->number==n)
                    free = false;
            }
        }
    } while (!free);
    return n;
}

void DiagramScene::addStartEndItem() {
    addDiagramItem(new DiagramItem(DiagramItem::StartEnd, calcnumber()));
    status = Normal;
}

void DiagramScene::addConditionalItem() {
    addDiagramItem(new DiagramItem(DiagramItem::Conditional, calcnumber()));
    status = Normal;
}

void DiagramScene::addStepItem() {
    addDiagramItem(new DiagramItem(DiagramItem::Step, calcnumber()));
    status = Normal;
}

void DiagramScene::addIOItem() {
    addDiagramItem(new DiagramItem(DiagramItem::IO, calcnumber()));
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
        editingItem = item;
        editingArrow = nullptr;
        ((MainWindow*)w)->lineEditor->setEnabled(true);
        ((MainWindow*)w)->lineEditor->setText(editingItem->Text().replace("\n", ";"));
    }
    if (status==AddArrowStart) {
        if (item->outArrow1==nullptr || item->diagramType()==DiagramItem::Conditional && item->outArrow2==nullptr) {
            arrowStart = item;
            selectStatus(AddArrowEnd);
        }
    }
    if (status==AddArrowEnd) {
        if (item!=arrowStart) {
            arrowEnd = item;
            addSelectedArrow();
            selectStatus(Normal);
        }
    }
}

void DiagramScene::arrowClicked(Arrow *arrow) {
    if (status==DeletingArrow) {
        delete arrow;
        selectStatus(Normal);
    }
    if (status==EditingText) {
        if (arrow->StartItem->diagramType()==DiagramItem::Conditional) {
            editingItem = nullptr;
            editingArrow = arrow;
            ((MainWindow*)w)->lineEditor->setEnabled(true);
            ((MainWindow*)w)->lineEditor->setText(editingArrow->textItem->toPlainText().replace("\n", ";"));
        }
        else {
            editingItem = nullptr;
            editingArrow = nullptr;
            ((MainWindow*)w)->lineEditor->setEnabled(false);
            ((MainWindow*)w)->lineEditor->setText("");
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
        W->varTable->hide();
        W->view->setGeometry(0,0, W->w, W->h-45);
        for (QGraphicsItem *it : items()) {
            DiagramItem* item = dynamic_cast<DiagramItem*>(it);
            if (item!=nullptr)
                item->setBrush(Qt::white);
        }
        if (check()) {
            W->menuRun->setEnabled(true);
            W->menuRun_auto->setEnabled(true);
            W->menuRun_manual->setEnabled(true);
            W->menuRun_stop->setEnabled(false);
        }
        else {
            W->menuRun->setEnabled(false);
        }
    }
    else {
        W->menuFile->setEnabled(false);
        W->menuEdit->setEnabled(false);
        W->menuRun->setEnabled(false);
        if (newStatus==DeletingItem) {
            W->statusBar->showMessage("Нажмите на узел для удаления. Для отмены нажмите Esc.");
        }
        if (newStatus==EditingText) {
            W->statusBar->showMessage("Нажите на узел или связь для редактирования текста. Используйте ; для разделения строк.");
            W->lineEditor->setText("");
            W->lineEditor->show();
            W->editorbtn->show();
            W->lineEditor->setEnabled(false);
            editingItem = nullptr;
            editingArrow = nullptr;
        }
        if (newStatus==AddArrowStart) {
            W->statusBar->showMessage("Нажмите на исходящий узел. Для отмены нажмите Esc.");
        }
        if (newStatus==AddArrowEnd) {
            W->statusBar->showMessage("Нажмите на входящий узел. Для отмены нажмите Esc.");
        }
        if (newStatus==RunningAuto || newStatus==RunningManual) {
            while (W->varTable->rowCount()>0)
                W->varTable->removeRow(0);
            W->varTable->show();
            W->menuRun->setEnabled(true);
            W->menuRun_auto->setEnabled(false);
            W->menuRun_manual->setEnabled(false);
            W->menuRun_stop->setEnabled(true);
            W->view->setGeometry(0,0, W->w-280, W->h-45);
            run();
        }
    }
    status = newStatus;
}

void DiagramScene::onTextEdited(QString text) {
    if (editingItem!=nullptr) {
        if (editingItem->diagramType()==DiagramItem::StartEnd || editingItem->diagramType()==DiagramItem::Conditional) {
            ((MainWindow*)w)->lineEditor->setText(text.replace(";",""));
            editingItem->setText(text.replace(";", ""));
        }
        if (editingItem->diagramType()==DiagramItem::IO) {
            int newlinepos=0;
            for (int i=0; i<text.length(); i++)
                if (text[i]==';')
                    newlinepos=i;
            int linelen = text.length()-newlinepos;
            if (linelen>40)
                text.remove(text.length()-1, 1);
            ((MainWindow*)w)->lineEditor->setText(text);
            editingItem->setText(text);
        }
        editingItem->setText(text.replace(";", "\n"));
        editingItem->redraw();
    }
    if (editingArrow!=nullptr) {
        ((MainWindow*)w)->lineEditor->setText(text.replace(";",""));
        editingArrow->textItem->setPlainText(text.replace(";", ""));
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
    arrowEnd->inArrows.append(arrow);
    addItem(arrow);
    connect(arrow, SIGNAL(clicked(Arrow*)), this, SLOT(arrowClicked(Arrow*)));
}

bool DiagramScene::check_dfs(DiagramItem *item) {
    if (used[item])
        return true;
    used[item] = true;
    switch (item->diagramType()) {

    case DiagramItem::StartEnd:
    {
        if (item->Text()=="begin") {
            if (item->inArrows.size()>0)
                return false;
        }
        else if (item->Text()=="end") {
            if (item->outArrow1!=nullptr || item->outArrow2!=nullptr) {
                return false;
            }
            return true;
        }
        else
            return false;
        break;
    }
    case DiagramItem::Step:
    {
        if (item->outArrow1==nullptr && item->outArrow2==nullptr)
            return false;

        QStringList lines = item->Text().split('\n');
        for (QString line : lines) {
            if (line.count('=')!=1)
                return false;
            string varname = line.split('=')[0].toStdString();
            for (int i=0; i<varname.length(); i++) {
                if (isspace(varname[i]))
                {
                    varname.erase(i,1);
                    i--;
                }
            }
            string linexpr = line.split('=')[1].toStdString();
            for (int i=0; i<linexpr.length(); i++) {
                if (isspace(linexpr[i]))
                {
                    linexpr.erase(i,1);
                    i--;
                }
            }
          try
          {
            if (!vardefined[varname]) {
                vardefined[varname] = true;
                parser.DefineVar(varname, &varvalue[varname]);
            }
            parser.SetExpr(linexpr);
            qDebug()<<parser.Eval();
          }
          catch (Parser::exception_type &e)
          {
                qDebug()<<QString::fromStdString(linexpr)<<QString::fromStdString(e.GetMsg());
            return false;
          }
        }
        break;
    }
    case DiagramItem::Conditional:
    {
        if (item->outArrow1==nullptr || item->outArrow2==nullptr)
            return false;
        if (item->outArrow1->Text()!="true" && item->outArrow2->Text()!="true" || item->outArrow1->Text()!="false" && item->outArrow2->Text()!="false")
            return false;
        string linexpr = item->Text().toStdString();
        for (int i=0; i<linexpr.length(); i++) {
            if (isspace(linexpr[i]))
            {
                linexpr.erase(i,1);
                i--;
            }
        }
        try
        {
          parser.SetExpr(linexpr);
          qDebug()<<parser.Eval();
        }
        catch (Parser::exception_type &e)
        {
              qDebug()<<QString::fromStdString(linexpr)<<QString::fromStdString(e.GetMsg());
          return false;
        }
        break;
    }
    case DiagramItem::IO:
    {
        if (item->outArrow1==nullptr && item->outArrow2==nullptr)
            return false;
        QStringList lines = item->Text().split('\n');
        for (QString line : lines) {
            if (line.indexOf("read")==0) {
                string var = line.toStdString();
                var.erase(0,5);
                if (!vardefined[var]) {
                    vardefined[var] = true;
                    parser.DefineVar(var, &varvalue[var]);
                }
            }
            else if (line.indexOf("print")==0){
                string var = line.toStdString();
                var.erase(0,6);
                if (!vardefined[var]) {
                    vardefined[var] = true;
                    parser.DefineVar(var, &varvalue[var]);
                }
            }
            else {
                return false;
            }
        }
        break;
    }

    }

    return ((item->outArrow1==nullptr?true:check_dfs(item->outArrow1->EndItem)) && (item->outArrow2==nullptr?true:check_dfs(item->outArrow2->EndItem)));
}

bool DiagramScene::check() {
    parser.ClearVar();
    vardefined.clear();
    varvalue.clear();
    DiagramItem *start = nullptr;
    for (QGraphicsItem *item : items()) {     //Находим начальный узел, заодно проверяем его единственность
        if (dynamic_cast<DiagramItem*>(item)) {
            DiagramItem *it = dynamic_cast<DiagramItem*>(item);
            if (it->diagramType() == DiagramItem::StartEnd && it->Text()=="begin") {
                if (start==nullptr)
                    start = it;
                else
                    return false;
            }
        }
    }
    used.clear();
    used[nullptr] = true;
    if (!check_dfs(start))
        return false;
    for (QGraphicsItem *it: items()) {
        DiagramItem *item;
        if (item = dynamic_cast<DiagramItem*>(it)) {
            if (!used[item])
                return false;
        }
    }
    return true;
}

void DiagramScene::runDFS() {
    DFSItem->setBrush(Qt::white);
    switch(DFSItem->diagramType()) {

    case DiagramItem::StartEnd:
    {
        if (DFSItem->Text()==QString("begin"))
            DFSItem = DFSItem->outArrow1->EndItem;
        else
            DFSItem = nullptr;
        break;
    }
    case DiagramItem::Step:
    {
        QStringList lines = DFSItem->Text().split('\n');
        for (QString line : lines) {
            string varname = line.split('=')[0].toStdString();
            for (int i=0; i<varname.length(); i++) {
                if (isspace(varname[i]))
                {
                    varname.erase(i,1);
                    i--;
                }
            }
            string linexpr = line.split('=')[1].toStdString();
            for (int i=0; i<linexpr.length(); i++) {
                if (isspace(linexpr[i]))
                {
                    linexpr.erase(i,1);
                    i--;
                }
            }
            if (!vardefined[varname]) {
                vardefined[varname] = true;
                parser.DefineVar(varname, &varvalue[varname]);
            }
            parser.SetExpr(linexpr);
            varvalue[varname] = parser.Eval();
            MainWindow *W = (MainWindow*)(w);
            W->updatevar(varname, varvalue[varname]);
        }
        DFSItem = DFSItem->outArrow1->EndItem;
        break;
    }
    case DiagramItem::Conditional:
    {
        parser.SetExpr(DFSItem->Text().toStdString());
        if (parser.Eval()) {
            if (DFSItem->outArrow1->Text()=="true")
                DFSItem = DFSItem->outArrow1->EndItem;
            else
                DFSItem = DFSItem->outArrow2->EndItem;
        }
        else {
            if (DFSItem->outArrow1->Text()=="false")
                DFSItem = DFSItem->outArrow1->EndItem;
            else
                DFSItem = DFSItem->outArrow2->EndItem;
        }
        break;
    }
    case DiagramItem::IO:
    {
        QStringList lines = DFSItem->Text().split('\n');
        for (QString line : lines) {
        if (line.indexOf("read")==0) {
            bool ok;
            string var = line.toStdString();
            var.erase(0,5);
            while (var.front()==' ')
                var.erase(0,1);
            while (var.back()==' ')
                var.erase(var.length()-1, 1);
            double d = QInputDialog::getDouble(dynamic_cast<QWidget*>(this),
                                      QString::fromUtf8("Введите значение переменной"),
                                      QString::fromUtf8((var+": ").c_str()),
                                      0.00, -100000, 100000, 6, &ok);
            if (ok){
                if (!vardefined[var]) {
                    vardefined[var] = true;
                    parser.DefineVar(var, &varvalue[var]);
                }
                varvalue[var] = d;
                MainWindow *W = (MainWindow*)(w);
                W->updatevar(var, d);
            }
            else {
                DFSItem = nullptr;
                selectStatus(Normal);
                return;
            }
        }
        else {
            string var = line.toStdString();
            var.erase(0,6);
            while (var.front()==' ')
                var.erase(0,1);
            while (var.back()==' ')
                var.erase(var.length()-1, 1);
            QMessageBox::information(dynamic_cast<QWidget*>(this),
                                   QString::fromUtf8("Значение переменной"),
                                   QString::fromStdString(var+": ")+QString::number(varvalue[var]));
        }
        }
        DFSItem = DFSItem->outArrow1->EndItem;
        break;
    }

    }

    if (DFSItem!=nullptr)
        DFSItem->setBrush(Qt::yellow);
    if (DFSItem == nullptr) {
        selectStatus(Normal);
        return;
    }
}

void DiagramScene::run() {
    parser.ClearVar();
    vardefined.clear();
    varvalue.clear();
    DiagramItem *start = nullptr;
    for (QGraphicsItem *item : items()) {
        if (dynamic_cast<DiagramItem*>(item)) {
            DiagramItem *it = dynamic_cast<DiagramItem*>(item);
            if (it->diagramType() == DiagramItem::StartEnd && it->Text()=="begin") {
                    start = it;
                    break;
            }
        }
    }
    used.clear();
    used[nullptr] = true;
    DFSItem = start;
    start->setBrush(Qt::yellow);
}
