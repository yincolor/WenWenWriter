#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
 * 规定textEdit 的 objectName 统一为 "tab_textEdit"
 *    label的 objectName统一为 "tab_Label"
*/
#define TextEditObjName "tab_textEdit"
#define LabelObjName "tab_Label"

//Chapter *chapterInEditView = new Chapter("","");
QStringList *chapterTabList = new QStringList();//一个用来储存标签页所属章节的列表
//书籍列表
QList<Book> bookList;




MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    if(!FileConsole::isFilesComplete())//
    {
        qDebug()<<"未检测到目录结构，已经初始化完毕";
    }

    //以下是关于设置treeview
   //加载json数据
    set_book_list();
    //打印树
    out_list_tree();
    //添加数据入树形图中;
    fresh_tree_view();

    //设置控件样式
    set_qss("default");

}

MainWindow::~MainWindow()
{
    if(ui->text_tabWidget->currentIndex()!=-1)
    {
        qDebug()<<"注意\n虽然已经设置好关闭就保存的逻辑\n请一定注意要在关闭之前关闭所有标签页以保证数据不丢失。";
        //调用槽函数将所有标签页关闭
        while(-1 != ui->text_tabWidget->currentIndex()){
            on_text_tabWidget_tabCloseRequested(ui->text_tabWidget->currentIndex());
        }
    }
    //保存bookList的jsonfile
    FileConsole::writeJsonFile(getJsonDocumentByList());
    delete ui;
}


void MainWindow::set_book_list()
{
    QJsonDocument doc = FileConsole::getJsonDocument(FileConsole::readJsonFile());//获取document
    QJsonObject doc_obj = doc.object();//转换成object
    QJsonArray books = doc_obj.value("books").toArray();//获取到了books
    //循环解析books数组
    for(int i=0;i<books.count();i++)
    {
        QJsonValue book_value = books.at(i);//获得一个书籍

        QJsonValue book_name = book_value.toObject().value("book_name");

        //新建一个book对象并载入book列表
        Book book(book_name.toString());

        //循环解析book对象的Volume数组
        QJsonArray volumes = book_value.toObject().value("volumes").toArray();
        for(int v_index=0;v_index<volumes.count();v_index++)
        {
            QJsonValue volume_value = volumes.at(v_index);//获取一个卷

            //新建卷类
            Volume volume(volume_value.toObject().value("volume_name").toString());

            QJsonArray chapters = volume_value.toObject().value("chapters").toArray();//获取章节数组

            for(int c_index=0;c_index<chapters.count();c_index++)
            {
                QJsonValue chapter_value = chapters.at(c_index);
                //新建一个章节类
                Chapter chapter(chapter_value.toObject().value("chapter_name").toString(),chapter_value.toObject().value("src").toString());
                //将章节加载到卷类列表中
                volume.appendChapter(chapter);

            }

            //将卷类加载到书类里
            book.appendVolume(volume);

        }
        bookList.append(book);
    }
}

QJsonDocument MainWindow::getJsonDocumentByList()
{
    /*
     * 生成一个QObject rootObject
     * 生成QJsonArray booksArray
     * 循环1,检索bookList的所有book
     *  生成QJsonObject bookObject
     *  bookObject 加载QJsonValue bookName
     *  生成QJsonValue volumesArray
     *  循环2,检索book中所有volume
     *      生成QJsonObject volumeObject
     *      volumeObject 加载QJsonValue volumeName
     *      生成QJsonArray chapterArray
     *      循环3,检索volume中所有chalpter
     *          生成QJsonObject chapterObject
     *          chapterObject 加载QJsonValue chapterName
     *          chapterObject 加载QjsonValue src
     *          加载chapterObject到chapterArray
     *      加载volumeObject到volumesArray
     * 循环加载bookObject到booksArray
     * 将books加载到rootObject
     * rootObject加载到document
    */

    QJsonObject rootObject;
    QJsonArray booksArray;

    for(int b_index=0;b_index<bookList.count();b_index++)
    {
        Book book = bookList.at(b_index);
        QJsonObject bookObject;
        bookObject.insert("book_name",book.getBookName());//加载书籍名
        QJsonArray volumesArray;
        for(int v_index=0;v_index<book.volumeCount();v_index++)
        {
            Volume volume = book.getVolume(v_index);
            QJsonObject volumeObject;
            volumeObject.insert("volume_name",volume.getVolumeName());//加载卷名
            QJsonArray chaptersArray;
            for(int c_index=0;c_index<volume.chapterCount();c_index++)
            {
                Chapter chapter = volume.getChapter(c_index);
                QJsonObject chapterObject;
                chapterObject.insert("chapter_name",chapter.getChapterName());
                chapterObject.insert("src",chapter.getFileName());
                chaptersArray.append(chapterObject);
            }
            volumeObject.insert("chapters",chaptersArray);
            volumesArray.append(volumeObject);//加载卷object
        }
        bookObject.insert("volumes",volumesArray);//加载卷数组
        booksArray.append(bookObject);
    }

    rootObject.insert("books",booksArray);
    QJsonDocument doc;
    doc.setObject(rootObject);
    return doc;
}

int MainWindow::out_list_tree()
{
    if(bookList.isEmpty())//假如列表是空的，直接退出
        return -1;
    for(int b_index = 0;b_index<bookList.count();b_index++)
    {
        Book book = bookList.at(b_index);//获取书籍对象
        qDebug()<<book.getBookName();//打印书籍
        for(int v_index=0;v_index<book.volumeCount();v_index++)
        {
            Volume volume = book.getVolume(v_index);//获取卷对象
            qDebug()<<"\t--|"<<volume.getVolumeName();
            for(int c_index=0;c_index<volume.chapterCount();c_index++)
            {
                Chapter chapter = volume.getChapter(c_index);
                qDebug()<<"\t\t--|"<<chapter.getChapterName()<<"("<<chapter.getFileName()<<")";
            }
        }
    }
    return 0;
}

int MainWindow::fresh_tree_view()
{
    QIcon* bookIcon = new QIcon(QStringLiteral(":/image/book.png"));
    QIcon* volumeIcon = new QIcon(QStringLiteral(":/image/volume.png"));
    QIcon* chapterIcon = new QIcon(QStringLiteral(":/image/chapter.png"));
    QStandardItemModel *model = new QStandardItemModel(ui->file_treeView);//树状视图的控制模型
    //设置标题名

    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("名称")<<QStringLiteral("说明"));


    //QList<Book> *myBookList;
    //myBookList = &bookList;
    //读取书籍信息
    for(int bookIndex=0;bookIndex<bookList.count();bookIndex++)
    {
        Book book = bookList.at(bookIndex);//获取书籍对象
        QStandardItem* bookItem = new QStandardItem(*bookIcon,book.getBookName());
        QStandardItem* bookAbout= new QStandardItem("书籍");
        bookItem->setEditable(false);
        bookAbout->setEditable(false);
        model->appendRow(QList<QStandardItem*>()<<bookItem<<bookAbout);//加载到模型中

        for(int volIndex=0;volIndex<book.volumeCount();volIndex++)
        {
            Volume volume = book.getVolume(volIndex);//获取卷对象
            //qDebug()<<"加载卷："<<volume.getVolumeName();
            QStandardItem* volumeItem = new QStandardItem(*volumeIcon,volume.getVolumeName());
            QStandardItem*volumeAbout = new QStandardItem("卷");
            volumeItem->setEditable(false);
            volumeAbout->setEditable(false);
            //加载到书籍的子项中
            bookItem->setChild(volIndex,0,volumeItem);
            bookItem->setChild(volIndex,1,volumeAbout);
            for(int chaIndex=0;chaIndex<volume.chapterCount();chaIndex++)
            {

                Chapter chapter = volume.getChapter(chaIndex);//获取章节对象
                QStandardItem* chapterItem = new QStandardItem(*chapterIcon,chapter.getChapterName());
                QStandardItem*chapterAbout = new QStandardItem("章节");
                chapterItem->setEditable(false);
                chapterAbout->setEditable(false);
                //加载到卷对象的子项中
                volumeItem->setChild(chaIndex,0,chapterItem);
                volumeItem->setChild(chaIndex,1,chapterAbout);
            }
        }
    }
    ui->file_treeView->setModel(model);//把模型加载到树形图中
    return 0;
}

//根据书籍名 卷名 章节名找到文件名
QString MainWindow::getFileNameByList(QString bookName,QString volumeName,QString chapterName)
{

    for(int b_index=0;b_index<bookList.count();b_index++)
    {
        Book book(bookList.at(b_index));
        if(bookName == book.getBookName())
        {

            for(int v_index=0;v_index<book.volumeCount();v_index++)
            {
                Volume volume(book.getVolume(v_index));
                if(volumeName == volume.getVolumeName())
                {
                    for(int c_index=0;c_index<volume.chapterCount();c_index++)
                    {
                        Chapter chapter(volume.getChapter(c_index));
                        if(chapterName == chapter.getChapterName())
                        {
                            return chapter.getFileName();
                            break;
                        }
                    }
                    break;
                }
            }
            break;
        }
    }
    return "";

}


void MainWindow::set_qss(QString qssName)
{
    QFile qssFile(":/qss/"+qssName+".qss");
    qssFile.open(QFile::ReadOnly);
    if(qssFile.isOpen())
    {
        this->setStyleSheet((QString)qssFile.readAll());
    }
}


void MainWindow::on_file_treeView_doubleClicked(const QModelIndex &index)
{
    //找到被点击的item
    //qDebug()<<"第"<<index.row()<<"排,第"<<index.column()<<"列";
    QString selectItemName;
    QString selectItemType;
    QString data_str = index.data().toString();//获取数据
    if(data_str=="书籍"||data_str=="卷"||data_str=="章节")
    {
        selectItemType = data_str;
        selectItemName = index.sibling(index.row(),0).data().toString();
    }
    else//点击的是名称区域
    {
        selectItemType = index.sibling(index.row(),1).data().toString();
        selectItemName = index.data().toString();
    }
    //qDebug()<<"点击的Item 名为："<<selectItemName<<"类型是："<<selectItemType;

    //假如点击类型是“章节”,先获取这个章节对象的卷名和卷的书籍名
    if(selectItemType == "章节")
    {

        QModelIndex volumeIndex = index.parent();
        QModelIndex bookIndex = volumeIndex.parent();

        //获取卷和书籍的名称
        QString bookName = bookIndex.data().toString();
        QString volumeName = volumeIndex.data().toString();
        //qDebug()<<"章节所在卷是："<<volumeName<<"”所在书籍是："<<bookName;
        int b_index = 0;//书籍所在书籍列表的索引

        for(;b_index<bookList.count();b_index++)
        {
            Book book(bookList.at(b_index));
            if(bookName == book.getBookName())
            {
                break;
            }
        }
        //获取了书籍 卷 章节 的对象
        Book book(bookList.at(b_index));
        Volume volume(book.getVolume(volumeName));
        Chapter chapter(book.getVolume(volumeName).getChapter(selectItemName));
        //获取章节文件名
        QString fileName = chapter.getFileName();
        //qDebug()<<"章节文本文件名："<<fileName;


        QVBoxLayout *tagLayout = new QVBoxLayout();
        QString lableContent = book.getBookName()+"/";
        lableContent+=volume.getVolumeName();
        lableContent+="/";
        lableContent+=chapter.getChapterName();//获得了一个内容是“书籍名/卷名/章节名 ”的字符串
        if(chapterTabList->contains(lableContent))//该标签页列表已经有这个标签了
        {
            //qDebug()<<"已经存在这个章节的标签页了";
            return;
        }
        else{//新建标签页，并将数据加载到这个新标签页中
            chapterTabList->append(lableContent);//加新建的章节标签加入列表中
            //qDebug()<<"现在标签页有："<<chapterTabList->count()<<"个。";
            QLabel *bvcLable = new QLabel(lableContent);//创建一个保存书籍名，卷名，章节名的标签
            bvcLable->setObjectName(LabelObjName);
            QTextEdit *textEdit = new QTextEdit();
            textEdit->setObjectName(TextEditObjName);
            //将内容加载到textEdit
            QFile f(FileConsole::getTextPath()+chapter.getFileName());
            f.open(QFile::ReadOnly);
            textEdit->setText(f.readAll());
            f.close();
            //加载控件到布局
            tagLayout->addWidget(bvcLable);
            tagLayout->addWidget(textEdit);
            //新建一个tab页
            int newTagIndex = ui->text_tabWidget->addTab(new QTabWidget(),chapter.getChapterName());
            //将布局添加到新建的tab页上
            ui->text_tabWidget->setCurrentIndex(newTagIndex);
            ui->text_tabWidget->currentWidget()->setLayout(tagLayout);
        }
    }
}

void MainWindow::on_text_tabWidget_tabCloseRequested(int index)
{
    qDebug()<<"要关闭的标签页索引是："<<index;
    QString labelContent= ui->text_tabWidget->widget(index)->findChild<QLabel*>(LabelObjName)->text();
//删除标签页列表chapterTabList中的对应数据
    for(int i=0;i<chapterTabList->count();i++)
    {
        if(labelContent == chapterTabList->at(i))
        {
            chapterTabList->removeAt(i);//移除即将关闭的标签页标志
            break;
        }
    }

    QStringList itemlist = labelContent.split('/');// 0:书籍名 1:卷名 2:章节名
//将数据保存到相应文件中
    QString textContent = ui->text_tabWidget->widget(index)->findChild<QTextEdit*>(TextEditObjName)->toPlainText();
    QString fileName = getFileNameByList(itemlist[0],itemlist[1],itemlist[2]);
    FileConsole::writeText(fileName,textContent.toUtf8());

//关闭index的标签页

    ui->text_tabWidget->removeTab(index);

}

//新建书籍的按钮 点击响应
void MainWindow::on_newBook_pushButton_clicked()
{
    bool ok = false;
    QString newBookName = QInputDialog::getText(this,tr("新建书籍"),tr("请输入需要新建的书籍名"),QLineEdit::Normal,QString::null,&ok);
    if(newBookName!="")
    {

        for(int b_index=0;b_index<bookList.count();b_index++)
        {
            Book book = bookList.at(b_index);
            if(newBookName == book.getBookName())
            {
                QMessageBox::warning(this,"重复的书籍","这个书籍已经被创建了",QMessageBox::Ok);
                return;
            }
        }
        qDebug()<<"创建了一个新的书籍: "<<newBookName;
        Book book(newBookName);
        bookList.append(book);
        fresh_tree_view();
    }
}

//新建卷的按钮 点击响应
void MainWindow::on_newVolume_pushButton_clicked()
{
    QModelIndex select = ui->file_treeView->currentIndex();
    QString selectStr = select.data().toString();
    QString selectName,selectType;//选中项的名称和类型
    if(selectStr=="书籍"||selectStr=="卷"||selectStr=="章节")
    {
        selectType = selectStr;
        selectName = select.sibling(select.row(),0).data().toString();
    }
    else
    {
        selectName = selectStr;
        selectType = select.sibling(select.row(),1).data().toString();
    }
    if(selectType!="书籍")
    {
        QMessageBox::warning(this,"无效的新建","请选中需要新建卷的书籍",QMessageBox::Ok);
        return;
    }
    else
    {
        bool ok =false;
        QString newVolumeName = QInputDialog::getText(this,"新建卷","请输入新建的卷名",QLineEdit::Normal,QString::null,&ok);
        if(!ok||newVolumeName=="")
        {
            qDebug()<<"已取消新建卷";
            return;
        }
        qDebug()<<"在"<<selectName<<"中新建新的卷："<<newVolumeName;
        int b_index =0;
        for(;b_index<bookList.count();b_index++)
        {
            Book book = bookList.at(b_index);
            if(selectName == book.getBookName())
            {
                break;
            }

        }
        Volume volume(newVolumeName);

        QList<Volume>* volumeList = bookList[b_index].getVolumeListPointer();

        for(int v_index=0;v_index<volumeList->count();v_index++)
        {
            Volume oldVolume(volumeList->at(v_index));
            if(newVolumeName == oldVolume.getVolumeName())
            {
                //出现重复的卷名
                QMessageBox::warning(this,"重复的卷","出现重复的卷名",QMessageBox::Ok);
                return;
            }
        }
        volumeList->append(volume);
        fresh_tree_view();
    }

}

//新建章节按钮点击响应
void MainWindow::on_newChapter_pushButton_clicked()
{
    QModelIndex select = ui->file_treeView->currentIndex();
    QString selectStr = select.data().toString();
    QString volumeName,selectType;//选中项的名称和类型
    if(selectStr=="书籍"||selectStr=="卷"||selectStr=="章节")
    {
        selectType = selectStr;
        volumeName = select.sibling(select.row(),0).data().toString();
    }
    else
    {
        volumeName = selectStr;
        selectType = select.sibling(select.row(),1).data().toString();
    }
    if(selectType!="卷")
    {
        QMessageBox::warning(this,"无效的新建","请选中需要新建章节的卷",QMessageBox::Ok);
        return;
    }
    else
    {
        QString bookName = select.parent().data().toString();

        bool ok =false;
        QString newChapterName = QInputDialog::getText(this,"新建章节","请输入新建的章节名",QLineEdit::Normal,QString::null,&ok);
        if(!ok||newChapterName=="")
        {
            qDebug()<<"已取消新建章节";
            return;
        }
        qDebug()<<"在书籍"<<bookName<<"的卷"<<volumeName<<"中添加章节"<<newChapterName;

        for(int b_index=0;b_index<bookList.count();b_index++)
        {
            if(bookName == bookList[b_index].getBookName())
            {
                for(int v_index=0;v_index<bookList[b_index].volumeCount();v_index++)
                {
                    if(volumeName == bookList[b_index].getVolume(v_index).getVolumeName())
                    {
                         QDateTime dateTime = QDateTime::currentDateTime();
                         QString fileName =dateTime.toString("yyyyMMddhhmmss");
                        fileName+=".txt";
                        qDebug()<<"新建txt文件名："<<fileName;

                        if(-1 != bookList[b_index].VolumeList[v_index].contained(newChapterName))
                        {
                            QMessageBox::warning(this,"重复的章节","已经有这个名称的章节了。",QMessageBox::Ok);
                            return;
                        }
                        const Chapter *chapter = new Chapter(newChapterName,fileName);
                        bookList[b_index].VolumeList[v_index].ChapterList.append(*chapter);
                        fresh_tree_view();

                        return;
                    }
                }
                break;
            }
        }

    }
}


void removeBook(QString bookName)
{
    for(int b_index=0;b_index<bookList.count();b_index++)
    {
        if(bookName == bookList[b_index].getBookName())
        {
            bookList.removeAt(b_index);
            break;
        }
    }
}
void removeVolume(QString bookName,QString volumeName)
{
    for(int b_index=0;b_index<bookList.count();b_index++)
    {
        if(bookName == bookList[b_index].getBookName())
        {

            for(int v_index=0;v_index<bookList[b_index].volumeCount();v_index++)
            {
                Volume volume(bookList[b_index].getVolume(v_index));
                if(volumeName == volume.getVolumeName())
                {
                    bookList[b_index].getVolumeListPointer()->removeAt(v_index);
                    break;
                }
            }
            break;
        }
    }
}
void removeChapter(QString bookName, QString volumeName,QString chapterName)
{
    for(int b_index=0;b_index<bookList.count();b_index++)
    {
        if(bookName == bookList[b_index].getBookName())
        {

            for(int v_index=0;v_index<bookList[b_index].volumeCount();v_index++)
            {
                Volume volume(bookList[b_index].getVolume(v_index));
                if(volumeName == volume.getVolumeName())
                {
                    bookList[b_index].VolumeList[v_index].removeChapter(chapterName);
                    break;
                }
            }
            break;
        }
    }
}


//删除按钮点击响应
void MainWindow::on_pushButton_4_clicked()
{
    QModelIndex select = ui->file_treeView->currentIndex();
    QString selectStr = select.data().toString();
    //确认是否未选中控件
    if(selectStr =="")
    {
        qDebug()<<"未选中任何控件";
        return;
    }
    //确认是否继续
    //QMessageBox::StandardButton isContinue;
    int isContinue = QMessageBox::warning(this,"请确认","是否继续进行删除操作",QMessageBox::Cancel,QMessageBox::Ok);
    if(isContinue == QMessageBox::Cancel)
    {
        qDebug()<<"已取消删除操作";
        return;
    }

    QString selectName,selectType;//选中项的名称和类型
    if(selectStr=="书籍"||selectStr=="卷"||selectStr=="章节")
    {
        selectType = selectStr;
        selectName = select.sibling(select.row(),0).data().toString();
    }
    else
    {
        selectName = selectStr;
        selectType = select.sibling(select.row(),1).data().toString();
    }

    if(selectType == "书籍")
    {
       qDebug()<<"删除书籍"<<selectName;
        removeBook(selectName);
    }
    else if(selectType == "卷")
    {
        QString bookName = select.parent().data().toString();
        qDebug()<<"删除"<<bookName<<"下的卷"<<selectName;
        removeVolume(bookName,selectName);
    }
    else if(selectType == "章节")
    {
        QModelIndex volumeIndex = select.parent();
        QString volumeName = volumeIndex.data().toString();
        QString bookName = volumeIndex.parent().data().toString();
        qDebug()<<"删除"<<bookName<<"下的"<<volumeName<<"下的章节"<<selectName;
        removeChapter(bookName,volumeName,selectName);

    }

    fresh_tree_view();
    //设置焦点到父节点上
    ui->file_treeView->setCurrentIndex(select.parent());
}

//保存全部数据
void MainWindow::on_saveAll_pushButton_clicked()
{
    //保存全部标签页的章节文本
    //获取标签页个数
    int maxIndex = ui->text_tabWidget->count();
    for(int index=0;index<maxIndex;index++)
    {
        QString labelContent= ui->text_tabWidget->widget(index)->findChild<QLabel*>(LabelObjName)->text();
        qDebug()<<labelContent;
        QStringList itemlist = labelContent.split('/');// 0:书籍名 1:卷名 2:章节名
    //将数据保存到相应文件中
        QString textContent = ui->text_tabWidget->widget(index)->findChild<QTextEdit*>(TextEditObjName)->toPlainText();
        QString fileName = getFileNameByList(itemlist[0],itemlist[1],itemlist[2]);
        FileConsole::writeText(fileName,textContent.toUtf8());
    }
    //保存bookList的jsonfile
    FileConsole::writeJsonFile(getJsonDocumentByList());
}

//关闭所有标签页
void MainWindow::on_closeAll_pushButton_clicked()
{
    int maxIndex = ui->text_tabWidget->count();

    for(int index=0;index<maxIndex;index++)
    {
        QString labelContent= ui->text_tabWidget->widget(index)->findChild<QLabel*>(LabelObjName)->text();
        qDebug()<<labelContent;
        QStringList itemlist = labelContent.split('/');// 0:书籍名 1:卷名 2:章节名
    //将数据保存到相应文件中
        QString textContent = ui->text_tabWidget->widget(index)->findChild<QTextEdit*>(TextEditObjName)->toPlainText();
        QString fileName = getFileNameByList(itemlist[0],itemlist[1],itemlist[2]);
        FileConsole::writeText(fileName,textContent.toUtf8());
        ui->text_tabWidget->setCurrentIndex(index);
        ui->text_tabWidget->currentWidget()->deleteLater();
        for(int i=0;i<chapterTabList->count();i++)
        {
            if(labelContent == chapterTabList->at(i))
            {
                chapterTabList->removeAt(i);
                break;
            }
        }
    }
}



void MainWindow::on_format_text_action_triggered()
{
    if(ui->text_tabWidget->currentIndex() == -1)//假如没有标签页则直接退出
    {
        qDebug()<<"无法格式化标签页-1";
        return;
    }
    QWidget *tabWidget = ui->text_tabWidget->currentWidget();
    QString tabName = tabWidget->findChild<QLabel*>(LabelObjName)->text();
    QTextEdit *textEdit = tabWidget->findChild<QTextEdit*>(TextEditObjName);
    QString tabText = textEdit->toPlainText();


    QStringList textList = tabText.split("\n");
    qDebug()<<"共有"<<textList.count()<<"自然段";
    //清空textedit
    textEdit->clear();
    for(int index =0;index<textList.count();index++)
    {
        QString str = textList.at(index);
        //删除空的字符串
        if(str==""){continue;}
        //删除字符串前后的空格和\t
        str.replace(QRegExp("^[( *)(\t*)]"),"");
        if(str==""){continue;}
        str = "\t"+str;
        //为字符串前面添加一个\t后边添加一个\n
        str.append("\n");
        //append到view中并
        textEdit->append(str);
    }
}

//打包成txt文件
void MainWindow::on_package_action_triggered()
{
    on_saveAll_pushButton_clicked();
    QModelIndex select = ui->file_treeView->currentIndex();
    QString selectStr = select.data().toString();
    QString bookName,selectType;//选中项的名称和类型


    if(selectStr=="书籍"||selectStr=="卷"||selectStr=="章节")
    {
        selectType = selectStr;
        bookName = select.sibling(select.row(),0).data().toString();
    }
    else
    {
        bookName = selectStr;
        selectType = select.sibling(select.row(),1).data().toString();
    }
    if(selectType!="书籍")
    {
        QMessageBox::warning(this,"无效的打包","请选中需要打包的书籍！！",QMessageBox::Ok);
        return;
    }
    else
    {
        QString pkgTxtPath = FileConsole::getPackagePath()+bookName+".txt";
        QFile pkgFile(pkgTxtPath);
        pkgFile.open(QFile::WriteOnly);

        pkgFile.close();//先生成一个txt文件
        pkgFile.open(QFile::ReadWrite|QIODevice::Append);
        QTextStream out(&pkgFile);
        out<<bookName<<"\n";//先写入小说名
        for(int b_index=0;b_index<bookList.count();b_index++)
        {
            if(bookName == bookList[b_index].getBookName())
            {
                QList<Volume> *volumeList = bookList[b_index].getVolumeListPointer();
                for(int v_index=0;v_index<volumeList->count();v_index++)
                {
                    Volume volume(volumeList->at(v_index));
                    QString volumeName = volume.getVolumeName();
                    QRegExp volumeRX("^(第.*卷.*)");
                    if(volumeRX.exactMatch(volumeName))
                    {
                        qDebug()<<"这个卷名不需要添加头";
                    }
                    else
                    {
                        int volume_index = v_index+1;
                        QString title = "\n第"+QString::number(volume_index,10);
                        title = title+"卷 ";
                        volumeName = title+volumeName;
                    }
                    qDebug()<<"打包卷："<<volumeName;
                    out<<volumeName<<"\n";//添加卷名
                    QList<Chapter>* chapterList = volume.getChapterListPointer();
                    for(int c_index=0;c_index<chapterList->count();c_index++)
                    {
                        Chapter chapter(chapterList->at(c_index));
                        QString chapterName = chapter.getChapterName();
                        QString fileName = chapter.getFileName();
                        QString text = FileConsole::readText(fileName);
                        QRegExp chapterRX("^(第.*章.*)");
                        if(chapterRX.exactMatch(chapterName))
                        {
                            qDebug()<<"这个章节名不需要添加头";
                        }
                        else
                        {
                            int chapter_index = c_index+1;
                            QString title = "第"+QString::number(chapter_index,10);
                            title = title+"章 ";
                            chapterName = title+chapterName;
                        }
                        out<<chapterName<<"\n"<<text<<"\n";
                    }

                }
            }
        }
    }
}
