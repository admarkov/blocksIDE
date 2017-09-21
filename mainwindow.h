#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsView>
#include <QMenuBar>
#include <QMenu>
#include <QtWidgets/QWidget>
#include <QApplication>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QString>
#include <QFile>
#include <QTableWidget>
#include "diagramscene.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QStatusBar *statusBar;

    void showMessage(QString s) {
        statusBar_message->setText(s);
        statusBar_message->show();
    }

    void hideMessage() {
        statusBar_message->hide();
    }

    const int h = 650;
    const int w = 1200;

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

    QLabel *statusBar_message;

    DiagramScene *scene;
    QGraphicsView *view;

    QLineEdit *lineEditor;
    QPushButton *editorbtn;

    QTableWidget *varTable;

    void setupUi();

    void retranslateUi();

    void keyPressEvent(QKeyEvent *event);

    QFile *File;
    bool unsavedChanges;

protected:
    void closeEvent(QCloseEvent *e);

public slots:

    void deleteItemAction();
    void editTextAction();
    void resetSceneStatus();
    void deleteArrowAction();
    void sceneChanged();
    void saveDiagram();
    void saveDiagramAs();
    void newDiagram();
    void openDiagram();
    void runManual();
    void runAuto();
    void stopRunning();

};

#endif // MAINWINDOW_H
