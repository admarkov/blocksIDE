#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi();
}

void MainWindow::setupUi()
{

    if (this->objectName().isEmpty())
        this->setObjectName(QStringLiteral("MainWindow"));

    this->resize(760, 480);

    centralWidget = new QWidget(this);
    centralWidget->setObjectName(QStringLiteral("centralWidget"));
    this->setCentralWidget(centralWidget);

    menuBar = new QMenuBar(this);
    menuBar->setObjectName(QStringLiteral("menuBar"));
    menuBar->setGeometry(QRect(0, 0, 760, 25));
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

    menuEdit_addItem = new QAction(this);
    menuEdit_addItem->setObjectName(QStringLiteral("menuEdit_addItem"));
    menuEdit->addAction(menuEdit_addItem);
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
    menuBlocks_action = new QAction(this);
    menuBlocks_action->setObjectName(QStringLiteral("menuBlocks_action"));
    menuBlocks->addAction(menuBlocks_action);
    menuBlocks_if = new QAction(this);
    menuBlocks_if->setObjectName(QStringLiteral("menuBlocks_if"));
    menuBlocks->addAction(menuBlocks_if);
    menuBlocks_io = new QAction(this);
    menuBlocks_io->setObjectName(QStringLiteral("menuBlocks_io"));
    menuBlocks->addAction(menuBlocks_io);

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

    graphicsView = new QGraphicsView(centralWidget);
    graphicsView->setObjectName(QStringLiteral("graphicsView"));
    graphicsView->setGeometry(QRect(0, 0, 760, 455));
    graphicsView->setSceneRect(QRectF(0, 0, 758, 445));
    graphicsView->setDragMode(QGraphicsView::NoDrag);

    retranslateUi();

    QMetaObject::connectSlotsByName(this);
}

void MainWindow::retranslateUi()
{
    setWindowTitle(QApplication::translate("MainWindow", "Diagram IDE", 0));
    menuFile_new->setText(QApplication::translate("MainWindow", "Новая", 0));;
    menuFile_open->setText(QApplication::translate("MainWindow", "Открыть", 0));;
    menuFile_save->setText(QApplication::translate("MainWindow", "Сохранить", 0));;
    menuFile_saveas->setText(QApplication::translate("MainWindow", "Сохранить как", 0));;
    menuEdit_addItem->setText(QApplication::translate("MainWindow", "Добавить узел", 0));;
    menuEdit_addConnection->setText(QApplication::translate("MainWindow", "Добавить связь", 0));;
    menuEdit_delItem->setText(QApplication::translate("MainWindow", "Удалить узел", 0));;
    menuEdit_delConnection->setText(QApplication::translate("MainWindow", "Удалить связь", 0));;
    menuEdit_editText->setText(QApplication::translate("MainWindow", "Редактировать текст", 0));;
    menuBlocks_begend->setText(QApplication::translate("MainWindow", "Начало/конец", 0));;
    menuBlocks_action->setText(QApplication::translate("MainWindow", "Вычисление", 0));;
    menuBlocks_if->setText(QApplication::translate("MainWindow", "Выбор", 0));;
    menuBlocks_io->setText(QApplication::translate("MainWindow", "Ввод/вывод", 0));;
    menuRun_manual->setText(QApplication::translate("MainWindow", "Пошаговый", 0));;
    menuRun_auto->setText(QApplication::translate("MainWindow", "Непрерывный", 0));;
    menuRun_stop->setText(QApplication::translate("MainWindow", "Стоп", 0));;

    menuFile->setTitle(QApplication::translate("MainWindow", "Файл", 0));
    menuEdit->setTitle(QApplication::translate("MainWindow", "Редактировать", 0));
    menuBlocks->setTitle(QApplication::translate("MainWindow", "Добавить узел", 0));
    menuRun->setTitle(QApplication::translate("MainWindow", "Запуск", 0));
}


MainWindow::~MainWindow()
{

}
