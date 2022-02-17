//
// Created by damian on 19/01/2022.
//

#ifndef FILEGRUNT_MAINWINDOW_H
#define FILEGRUNT_MAINWINDOW_H

#include "ui_MainWindow.h"
#include <QMainWindow>
#include <QtWidgets>
#include <QFileDialog>
#include <QStringList>
#include <QStringListModel>
#include <filesystem>
#include <string>
#include <list>
#include <opencv2/opencv.hpp>

extern "C"{
#include <sqlite3.h>
}

using std::string;
using std::filesystem::recursive_directory_iterator;
using std::list;

QT_BEGIN_NAMESPACE
namespace Ui {
    class MainWindow;
    class QAction;
}

namespace cv {
    class imread;
}

QT_END_NAMESPACE

struct duplicate_data{
    string path;
    string name;
    string size;
    string duplicate_path;
};

class MainWindow : public QMainWindow
{
Q_OBJECT

protected:
    void closeEvent(QCloseEvent *) override;

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
    static int callback(void*,int,char**,char**);
    static int getFilesCallback(void*,int,char**,char**);
    static int getDuplicateFileNamesCallback(void*,int,char**,char**);
    static int duplicateToArrayCallback(void*,int,char**,char**);

private:
    Ui::MainWindow *ui;
    void createActions();
    QAction *actionExit;
    QStringListModel *model;
    QStringList List;
    //static sqlite3* db = nullptr;
    sqlite3* db = nullptr;
    std::list<duplicate_data> duplicate_list;
    std::list<duplicate_data> * dup_list = &duplicate_list;
    std::list<duplicate_data> duplicate_images;

private slots:
    void selectSource();
    void processSource();
    void compareImages();

};

#endif //FILEGRUNT_MAINWINDOW_H
