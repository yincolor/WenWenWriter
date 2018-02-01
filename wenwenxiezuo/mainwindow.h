#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<QPushButton>

#include <qstandarditemmodel.h>
#include <QStringList>
#include<QDateTime>
#include<QDebug>
#include<qdebug.h>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QStandardItemModel>
#include<QStandardItem>
#include<QLabel>
#include<QTextEdit>
#include<QMessageBox>
#include<QInputDialog>
#include"fileconsole.h"
#include"book.h"
#include"chapter.h"
#include"volume.h"
namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


    //展示在textEdit的Chapter,但并不是BookList的对象


    //获取json的dom 并加载到List上
    void set_book_list();
    //根据List 创建json document
    QJsonDocument getJsonDocumentByList();

    //打印BookList的树
    int out_list_tree();
    //加载list到treeview上，用于在开启应用、增删对象后刷新treeview
    int fresh_tree_view();
    //根据书籍名 卷名 章节名找到文件名
    QString getFileNameByList(QString bookName,QString volumeName,QString chapterName);




    //设置控件样式
    void set_qss(QString qssName);
private slots:


    void on_file_treeView_doubleClicked(const QModelIndex &index);

    void on_text_tabWidget_tabCloseRequested(int index);

    void on_newBook_pushButton_clicked();

    void on_newVolume_pushButton_clicked();

    void on_newChapter_pushButton_clicked();

    void on_pushButton_4_clicked();

    void on_saveAll_pushButton_clicked();

    void on_closeAll_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
