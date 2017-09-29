#include "mainwindow.h"
#include "diagramscene.h"
#include "diagramitem.h"
#include "arrow.h"
#include <QDebug>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QHeaderView>
#include <map>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi();
}

void MainWindow::setupUi()
{

    this->setFixedSize(w, h);

    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    scene = new DiagramScene(this);
    scene->setSceneRect(0, 0, w-10, h-47);

    view = new QGraphicsView(centralWidget);
    view->setGeometry(QRect(0, 0, w, h-45));
    view->setScene(scene);

    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(sceneChanged()));
    unsavedChanges = false;

    menuBar = new QMenuBar(this);
    menuBar->setGeometry(QRect(0, 0, w, 25));
    menuFile = new QMenu(menuBar);
    menuBar->addAction(menuFile->menuAction());
    menuEdit = new QMenu(menuBar);
    menuBar->addAction(menuEdit->menuAction());
    menuBlocks = new QMenu(menuEdit);
    menuRun = new QMenu(menuBar);
    menuRun->setEnabled(false);
    menuBar->addAction(menuRun->menuAction());
    setMenuBar(menuBar);

    menuFile_new = new QAction(this);
    menuFile->addAction(menuFile_new);
    connect(menuFile_new, SIGNAL(triggered(bool)), this, SLOT(newDiagram()));
    menuFile_open = new QAction(this);
    menuFile->addAction(menuFile_open);
    connect(menuFile_open, SIGNAL(triggered(bool)),this, SLOT(openDiagram()));
    menuFile_save = new QAction(this);
    menuFile->addAction(menuFile_save);
    connect(menuFile_save, SIGNAL(triggered(bool)), this, SLOT(saveDiagram()));
    menuFile_saveas = new QAction(this);
    menuFile->addAction(menuFile_saveas);
    connect(menuFile_saveas, SIGNAL(triggered(bool)), this, SLOT(saveDiagramAs()));

    menuEdit->addMenu(menuBlocks);
    menuEdit_addConnection = new QAction(this);
    menuEdit->addAction(menuEdit_addConnection);
    connect(menuEdit_addConnection, SIGNAL(triggered(bool)), scene, SLOT(addArrow()));
    menuEdit_delItem = new QAction(this);
    connect(menuEdit_delItem, SIGNAL(triggered()), this, SLOT(deleteItemAction()));
    menuEdit->addAction(menuEdit_delItem);
    menuEdit_delConnection = new QAction(this);
    menuEdit->addAction(menuEdit_delConnection);
    connect(menuEdit_delConnection, SIGNAL(triggered()), this, SLOT(deleteArrowAction()));
    menuEdit_editText= new QAction(this);
    menuEdit->addAction(menuEdit_editText);
    connect(menuEdit_editText, SIGNAL(triggered()), this, SLOT(editTextAction()));

    menuBlocks_begend = new QAction(this);
    menuBlocks->addAction(menuBlocks_begend);
    connect(menuBlocks_begend, SIGNAL(triggered()), scene, SLOT(addStartEndItem()));
    menuBlocks_action = new QAction(this);
    menuBlocks->addAction(menuBlocks_action);
    connect(menuBlocks_action, SIGNAL(triggered()), scene, SLOT(addStepItem()));
    menuBlocks_if = new QAction(this);
    menuBlocks->addAction(menuBlocks_if);
    connect(menuBlocks_if, SIGNAL(triggered()), scene, SLOT(addConditionalItem()));
    menuBlocks_io = new QAction(this);
    menuBlocks->addAction(menuBlocks_io);
    connect(menuBlocks_io, SIGNAL(triggered()), scene, SLOT(addIOItem()));

    menuRun_manual = new QAction(this);
    menuRun->addAction(menuRun_manual);
    connect(menuRun_manual, SIGNAL(triggered(bool)), this, SLOT(runManual()));
    menuRun_auto = new QAction(this);
    menuRun->addAction(menuRun_auto);
    connect(menuRun_auto, SIGNAL(triggered(bool)), this, SLOT(runAuto()));
    menuRun->addSeparator();
    menuRun_stop = new QAction(this);
    menuRun->addAction(menuRun_stop);
    menuRun_stop->setEnabled(false);
    connect(menuRun_stop, SIGNAL(triggered(bool)), this, SLOT(stopRunning()));

    statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar_message = new QLabel;
    statusBar_message->hide();
    statusBar->addWidget(statusBar_message);

    lineEditor = new QLineEdit(this);
    lineEditor->setGeometry(0,25,w-100,30);
    lineEditor->setFont(QFont("Verdana", 12));
    connect(lineEditor, SIGNAL(textEdited(QString)), scene, SLOT(onTextEdited(QString)));

    editorbtn = new QPushButton(this);
    editorbtn->setGeometry(w-100,25,100,30);
    editorbtn->setText("Завершить");
    connect(editorbtn, SIGNAL(clicked(bool)), this, SLOT(resetSceneStatus()));

    lineEditor->hide();
    editorbtn->hide();

    varTable = new QTableWidget(this);
    varTable->setGeometry(w-280, 25, 280, h-46);
    varTable->setColumnCount(2);
    varTable->setColumnWidth(1, 140);
    QStringList tableHeader;
    tableHeader<<"Переменная"<<"Значение";
    varTable->setHorizontalHeaderLabels(tableHeader);
    varTable->verticalHeader()->hide();
    varTable->setSelectionMode(QAbstractItemView::NoSelection);
    varTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    varTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    varTable->hide();

    File = nullptr;

    //qDebug()<<QFileDialog::getSaveFileName(0, "Сохранить диаграму", "", "*.diag");

    retranslateUi();

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("Diagram IDE | Untitled"));

    menuFile_new->setText(tr("Новая"));
    menuFile_open->setText(tr("Открыть"));
    menuFile_save->setText(tr("Сохранить"));
    menuFile_saveas->setText(tr("Сохранить как"));
    menuEdit_addConnection->setText(tr("Добавить связь"));
    menuEdit_delItem->setText(tr("Удалить узел"));
    menuEdit_delConnection->setText(tr("Удалить связь"));
    menuEdit_editText->setText(tr("Редактировать текст"));
    menuBlocks_begend->setText(tr("Начало/конец"));
    menuBlocks_action->setText(tr("Вычисление"));
    menuBlocks_if->setText(tr("Выбор"));
    menuBlocks_io->setText(tr("Ввод/вывод"));
    menuRun_manual->setText(tr("Пошаговый"));
    menuRun_auto->setText(tr("Непрерывный"));
    menuRun_stop->setText(tr("Стоп"));
    menuFile->setTitle(tr("Файл"));
    menuEdit->setTitle(tr("Редактировать"));
    menuBlocks->setTitle(tr("Добавить узел"));
    menuRun->setTitle(tr("Запуск"));
}

void MainWindow::deleteItemAction() {
    scene->selectStatus(DiagramScene::DeletingItem);
}

void MainWindow::editTextAction() {
    scene->selectStatus(DiagramScene::EditingText);
}

void MainWindow::resetSceneStatus() {
    scene->selectStatus(DiagramScene::Normal);
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
    if (event->key()==Qt::Key_Escape && scene->status!=DiagramScene::RunningManual && scene->status!=DiagramScene::RunningAuto) {
        scene->selectStatus(DiagramScene::Normal);
    }
    if (event->key()==Qt::Key_Space && scene->status==DiagramScene::RunningManual) {
        scene->runDFS();
    }
}

void MainWindow::deleteArrowAction() {
    scene->selectStatus(DiagramScene::DeletingArrow);
}

void MainWindow::saveDiagram() {
    if (File==nullptr) {
        saveDiagramAs();
        return;
    }
    File->open(QIODevice::WriteOnly);
    QTextStream file(File);
    file<<scene->items().size()/2<<"\n";
    for (QGraphicsItem *item : scene->items()) {
        if (dynamic_cast<DiagramItem*>(item)) {
            DiagramItem *it = dynamic_cast<DiagramItem*>(item);
            file<<"item\n";
            file<<it->number<<"\n";
            if (it->diagramType()==DiagramItem::StartEnd)
                file<<"StartEnd\n";
            if (it->diagramType()==DiagramItem::Conditional)
                file<<"Conditional\n";
            if (it->diagramType()==DiagramItem::IO)
                file<<"IO\n";
            if (it->diagramType()==DiagramItem::Step)
                file<<"Step\n";
            file<<it->pos().x()<<" "<<it->pos().y()<<"\n";
            file<<(it->Text().isEmpty()?"$NO_TEXT$":it->Text())<<"\n";
        }
        else if (dynamic_cast<Arrow*>(item)) {
            Arrow *ar = dynamic_cast<Arrow*>(item);
            file<<"arrow\n";
            file<<ar->StartItem->number<<" "<<ar->EndItem->number<<"\n";
            file<<(ar->Text().isEmpty()?"$NO_TEXT$":ar->Text())<<"\n";
        }
    }
    File->close();
}

void MainWindow::saveDiagramAs() {
    if (File!=nullptr) {
        delete File;
        File = nullptr;
    }
    QString path = QFileDialog::getSaveFileName(0, "Сохранить диаграмму", "", "*.diag");
    if (!path.count(".diag"))
        path+=".diag";
    if (path!=".diag") {
        File = new QFile(path);
        setWindowTitle("Diagram IDE | " + File->fileName());
        saveDiagram();
    }
}

void MainWindow::newDiagram() {
   /* if (unsavedChanges) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QString::fromUtf8("Предупреждение"),
                              QString::fromUtf8("Имеются несохраненные изменения. Сохранить?"),
                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            saveDiagram();
        }
        else if (reply == QMessageBox::Cancel) {
            return;
        }
    }*/
    if (File!=nullptr)
        delete File;
    File = nullptr;
    setWindowTitle("Diagram IDE | Untitled");
    for (QGraphicsItem *item : scene->items()) {
        scene->removeItem(item);
    }
    scene->setSceneRect(0,0,w,h-47);
}

struct fileArrow {
    int from, to;
    QString text;
    fileArrow(int f, int t, QString txt) : from(f), to(t), text(txt) { }
};

void MainWindow::openDiagram() {
    QString path = QFileDialog::getOpenFileName(0, "Открыть диаграмму", "", "*.diag");
    if (QFile::exists(path)) {
        newDiagram();
        File = new QFile(path);
        setWindowTitle("Diagram IDE | " + File->fileName());
        File->open(QIODevice::ReadOnly);
        QTextStream file(File);
        QString itemType;
        QList<fileArrow> arrows;
        std::map<int, DiagramItem*> numberToItem;
        QString line;
        while (!file.atEnd()) {
            itemType = file.readLine();
            if (itemType == "arrow") {
                int to, from;
                QString text;
                line = file.readLine();
                from = line.split(" ").at(1).toInt();
                to = line.split(" ").at(0).toInt();
                text = file.readLine();
                arrows.append(fileArrow(to, from, text));
            }
            else if (itemType == "item") {
                int n;
                qreal x, y;
                QString type, text;
                n = file.readLine().toInt();
                type = file.readLine();
                line = file.readLine();
                x = line.split(" ").at(0).toDouble();
                y = line.split(" ").at(1).toDouble();
                text = file.readLine();
                DiagramItem *item;
                if (type=="StartEnd")
                    item = new DiagramItem(DiagramItem::StartEnd,n);
                if (type=="Conditional")
                    item = new DiagramItem(DiagramItem::Conditional,n);
                if (type=="IO")
                    item = new DiagramItem(DiagramItem::IO,n);
                if (type=="Step")
                    item = new DiagramItem(DiagramItem::Step,n);
                item->setPos(x, y);
                if (text!="$NO_TEXT$")
                    item->setText(text);
                item->setPos(x,y);
                scene->addItem(item);
                connect(item, SIGNAL(positionChanged(DiagramItem*,QPointF)), scene, SLOT(itemPositionChanged(DiagramItem*,QPointF)));
                connect(item, SIGNAL(clicked(DiagramItem*)), scene, SLOT(itemClicked(DiagramItem*)));
                numberToItem[n] = item;
            }
        }
        File->close();
        for (fileArrow farrow : arrows) {
            DiagramItem *arrowStart = numberToItem[farrow.from];
            DiagramItem *arrowEnd = numberToItem[farrow.to];
            Arrow *arrow = new Arrow(arrowStart, arrowEnd);
            if (arrowStart->outArrow1!=nullptr)
                arrowStart->outArrow2 = arrow;
            else
                arrowStart->outArrow1 = arrow;
            arrowEnd->inArrows.append(arrow);
            scene->addItem(arrow);
            connect(arrow, SIGNAL(clicked(Arrow*)), scene, SLOT(arrowClicked(Arrow*)));
            if (farrow.text!="$NO_TEXT$")
                arrow->setText(farrow.text);
        }
        scene->itemPositionChanged();
        scene->selectStatus(DiagramScene::Normal);
    }
}

void MainWindow::sceneChanged() {

}

void MainWindow::closeEvent(QCloseEvent *e) {
   /* if (unsavedChanges) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, QString::fromUtf8("Предупреждение"),
                              QString::fromUtf8("Имеются несохраненные изменения. Сохранить перед завершением?"),
                              QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (reply == QMessageBox::Yes) {
            saveDiagram();
            e->accept();
        }
        else if (reply == QMessageBox::No) {
            e->accept();
        }
        else {
            e->ignore();
        }
    }*/
    e->accept();
}

void MainWindow::runAuto() {
    scene->selectStatus(DiagramScene::RunningAuto);
}

void MainWindow::runManual() {
    scene->selectStatus(DiagramScene::RunningManual);
}

void MainWindow::stopRunning() {
    scene->selectStatus(DiagramScene::Normal);
}

void MainWindow::updatevar(string varname, double varvalue) {
    QString var = QString::fromStdString(varname);
    QString val = QString::number(varvalue);
    bool isUpdated = false;
    for (int i=0; i<varTable->rowCount(); i++) {
        if (varTable->item(i,0)->text() == var) {
            varTable->item(i,1)->setText(val);
            isUpdated = true;
            break;
        }
    }
    if (!isUpdated) {
        varTable->insertRow(varTable->rowCount());
        qDebug()<<QString::number(varTable->rowCount());
        varTable->setItem(varTable->rowCount()-1, 0, new QTableWidgetItem);
        varTable->setItem(varTable->rowCount()-1, 1, new QTableWidgetItem);
        varTable->item(varTable->rowCount()-1, 0)->setText(var);
        varTable->item(varTable->rowCount()-1, 1)->setText(val);
    }
}

MainWindow::~MainWindow()
{

}
