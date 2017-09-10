#include "mainwindow.h"
#include "diagramscene.h"
#include "diagramitem.h"

#include <QStatusBar>

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

    scene = new DiagramScene();
    scene->setObjectName(QStringLiteral("scene"));
    scene->setSceneRect(0, 0, w-10, h-47);

    view = new QGraphicsView(centralWidget);
    view->setObjectName(QStringLiteral("view"));
    view->setGeometry(QRect(0, 0, w, h-45));
    view->setScene(scene);

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
    menuFile_open = new QAction(this);
    menuFile_open->setObjectName(QStringLiteral("menuFile_open"));
    menuFile->addAction(menuFile_open);
    menuFile_save = new QAction(this);
    menuFile_save->setObjectName(QStringLiteral("menuFile_save"));
    menuFile->addAction(menuFile_save);
    menuFile_saveas = new QAction(this);
    menuFile_saveas->setObjectName(QStringLiteral("menuFile_saveas"));
    menuFile->addAction(menuFile_saveas);

    menuEdit->addMenu(menuBlocks);
    menuEdit_addConnection = new QAction(this);
    menuEdit_addConnection->setObjectName(QStringLiteral("menuEdit_addConnection"));
    menuEdit->addAction(menuEdit_addConnection);
    menuEdit_delItem = new QAction(this);
    menuEdit_delItem->setObjectName(QStringLiteral("menuEdit_delItem"));
    menuEdit->addAction(menuEdit_delItem);
    menuEdit_delConnection = new QAction(this);
    menuEdit_delConnection->setObjectName(QStringLiteral("menuEdit_delConnection"));
    menuEdit->addAction(menuEdit_delConnection);
    menuEdit_editText= new QAction(this);
    menuEdit_editText->setObjectName(QStringLiteral("menuEdit_editText"));
    menuEdit->addAction(menuEdit_editText);

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

    retranslateUi();

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("Diagram IDE"));

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


MainWindow::~MainWindow()
{

}
