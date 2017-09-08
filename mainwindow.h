#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenuBar>
#include <QMenu>
#include <QtWidgets/QWidget>
#include <QApplication>
#include <QLabel>
#include "diagramscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    const int h = 600;
    const int w = 960;

    QWidget *centralWidget;

    QMenu *menuFile;
    QMenu *menuEdit;
    QMenu *menuBlocks;
    QMenu *menuRun;

    QAction *menuFile_new;
    QAction *menuFile_open;
    QAction *menuFile_save;
    QAction *menuFile_saveas;
    QAction *menuEdit_addConnection;
    QAction *menuEdit_delItem;
    QAction *menuEdit_delConnection;
    QAction *menuEdit_editText;
    QAction *menuBlocks_begend;
    QAction *menuBlocks_action;
    QAction *menuBlocks_if;
    QAction *menuBlocks_io;
    QAction *menuRun_manual;
    QAction *menuRun_auto;
    QAction *menuRun_stop;

    QMenuBar *menuBar;

    QStatusBar *statusBar;
    QLabel *statusBar_message;

    DiagramScene *scene;
    QGraphicsView *view;


    void setupUi();

    void retranslateUi();
};

#endif // MAINWINDOW_H
