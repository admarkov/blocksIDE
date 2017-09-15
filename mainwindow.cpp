#include "mainwindow.h"
#include "diagramscene.h"
#include "diagramitem.h"
#include "arrow.h"
#include <QDebug>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi();
}

void MainWindow::setupUi()
{

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("MainWindow"));

    this->setFixedSize(w, h);

    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    this->setCentralWidget(centralWidget);

    scene = new DiagramScene(this);
    scene->setObjectName(QStringLiteral("scene"));
    scene->setSceneRect(0, 0, w-10, h-47);

    view = new QGraphicsView(centralWidget);
    view->setObjectName(QStringLiteral("view"));
    view->setGeometry(QRect(0, 0, w, h-45));
    view->setScene(scene);

    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(sceneChanged()));
    unsavedChanges = false;

    menuBar = new QMenuBar(this);
    menuBar->setObjectName(QStringLiteral("menuBar"));
    menuBar->setGeometry(QRect(0, 0, w, 25));
    menuFile = new QMenu(menuBar);
    menuFile->setObjectName(QStringLiteral("menuFile"));
    menuBar->addAction(menuFile->menuAction());
    menuEdit = new QMenu(menuBar);
    menuEdit->setObjectName(QStringLiteral("menuEdit"));
    menuBar->addAction(menuEdit->menuAction());
    menuBlocks = new QMenu(menuEdit);
    menuBlocks->setObjectName(QStringLiteral("menuBlocks"));
    menuRun = new QMenu(menuBar);
    menuRun->setObjectName(QStringLiteral("menuRun"));
    menuRun->setEnabled(false);
    menuBar->addAction(menuRun->menuAction());
    setMenuBar(menuBar);

    menuFile_new = new QAction(this);
    menuFile_new->setObjectName(QStringLiteral("menuFile_new"));
    menuFile->addAction(menuFile_new);
    connect(menuFile_new, SIGNAL(triggered(bool)), this, SLOT(newDiagram()));
    menuFile_open = new QAction(this);
    menuFile_open->setObjectName(QStringLiteral("menuFile_open"));
    menuFile->addAction(menuFile_open);
    menuFile_save = new QAction(this);
    menuFile_save->setObjectName(QStringLiteral("menuFile_save"));
    menuFile->addAction(menuFile_save);
    connect(menuFile_save, SIGNAL(triggered(bool)), this, SLOT(saveDiagram()));
    menuFile_saveas = new QAction(this);
    menuFile_saveas->setObjectName(QStringLiteral("menuFile_saveas"));
    menuFile->addAction(menuFile_saveas);
    connect(menuFile_saveas, SIGNAL(triggered(bool)), this, SLOT(saveDiagramAs()));

    menuEdit->addMenu(menuBlocks);
    menuEdit_addConnection = new QAction(this);
    menuEdit_addConnection->setObjectName(QStringLiteral("menuEdit_addConnection"));
    menuEdit->addAction(menuEdit_addConnection);
    connect(menuEdit_addConnection, SIGNAL(triggered(bool)), scene, SLOT(addArrow()));
    menuEdit_delItem = new QAction(this);
    menuEdit_delItem->setObjectName(QStringLiteral("menuEdit_delItem"));
    connect(menuEdit_delItem, SIGNAL(triggered()), this, SLOT(deleteItemAction()));
    menuEdit->addAction(menuEdit_delItem);
    menuEdit_delConnection = new QAction(this);
    menuEdit_delConnection->setObjectName(QStringLiteral("menuEdit_delConnection"));
    menuEdit->addAction(menuEdit_delConnection);
    connect(menuEdit_delConnection, SIGNAL(triggered()), this, SLOT(deleteArrowAction()));
    menuEdit_editText= new QAction(this);
    menuEdit_editText->setObjectName(QStringLiteral("menuEdit_editText"));
    menuEdit->addAction(menuEdit_editText);
    connect(menuEdit_editText, SIGNAL(triggered()), this, SLOT(editTextAction()));

    menuBlocks_begend = new QAction(this);
    menuBlocks_begend->setObjectName(QStringLiteral("menuBlocks_begend"));
    menuBlocks->addAction(menuBlocks_begend);
    connect(menuBlocks_begend, SIGNAL(triggered()), scene, SLOT(addStartEndItem()));
    menuBlocks_action = new QAction(this);
    menuBlocks_action->setObjectName(QStringLiteral("menuBlocks_action"));
    menuBlocks->addAction(menuBlocks_action);
    connect(menuBlocks_action, SIGNAL(triggered()), scene, SLOT(addStepItem()));
    menuBlocks_if = new QAction(this);
    menuBlocks_if->setObjectName(QStringLiteral("menuBlocks_if"));
    menuBlocks->addAction(menuBlocks_if);
    connect(menuBlocks_if, SIGNAL(triggered()), scene, SLOT(addConditionalItem()));
    menuBlocks_io = new QAction(this);
    menuBlocks_io->setObjectName(QStringLiteral("menuBlocks_io"));
    menuBlocks->addAction(menuBlocks_io);
    connect(menuBlocks_io, SIGNAL(triggered()), scene, SLOT(addIOItem()));

    menuRun_manual = new QAction(this);
    menuRun_manual->setObjectName(QStringLiteral("menuRun_manual"));
    menuRun->addAction(menuRun_manual);
    menuRun_auto = new QAction(this);
    menuRun_auto->setObjectName(QStringLiteral("menuRun_auto"));
    menuRun->addAction(menuRun_auto);
    menuRun->addSeparator();
    menuRun_stop = new QAction(this);
    menuRun_stop->setObjectName(QStringLiteral("menuRun_stop"));
    menuRun->addAction(menuRun_stop);
    menuRun_stop->setEnabled(false);

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
    if (event->key()==Qt::Key_Escape) {
        scene->selectStatus(DiagramScene::Normal);
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
    unsavedChanges = false;
    if (windowTitle()[windowTitle().length()-1]=='*') {
        QString title = "";
        for (int i=0; i<windowTitle().length()-1; i++)
            title+=windowTitle()[i];
        setWindowTitle(title);
    }
    File->close();
}

void MainWindow::saveDiagramAs() {
    if (File!=nullptr) {
        delete File;
        File = nullptr;
    }
    QString path = QFileDialog::getSaveFileName(0, "Сохранить диаграму", "", "*.diag");
    if (!path.count(".diag"))
        path+=".diag";
    if (path!=".diag") {
        File = new QFile(path);
        setWindowTitle("Diagram IDE | " + File->fileName());
        saveDiagram();
    }
}

void MainWindow::newDiagram() {
    delete File;
    File = nullptr;
    setWindowTitle("Diagram IDE | Untitled");
    for (QGraphicsItem *item : scene->items()) {
        scene->removeItem(item);
    }
}

void MainWindow::sceneChanged() {
    unsavedChanges = true;
    if (windowTitle()[windowTitle().length()-1]!='*')
        setWindowTitle(windowTitle()+"*");
}

MainWindow::~MainWindow()
{

}
