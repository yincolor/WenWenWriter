#include "fileconsole.h"
#include<QDebug>
#include<QFile>
#include<QJsonDocument>
#include<QDir>


FileConsole::FileConsole()
{

}

///判断家目录里是否有.wenwenxiezuo及相关目录结构
/// .wenwenxiezuo-|
///               -info.json
///               -text-|
///                     -xxxxxxx.txt
/// \brief FileConsole::isFilesComplete
/// \return 0
int FileConsole::isFilesComplete(){
    QString file_path = QDir::homePath()+DocumentDir; // /home/xxx/.wenwenxiezuo/
    QString json_path = QDir::homePath()+JsonSrc; // /home/xxx/.wenwenxiezuo/info.json
    QString text_path = QDir::homePath()+TextDir; // /home/xx/./wenwenxiezuo/text/
    //判断是否有.wenwenxiezuo目录
    QDir d1(file_path);
    if(!d1.exists()){
        d1.mkpath(file_path);
    }
    //判断.wenwenxiezuo目录下是否有info.json文件
    QFile f_json(json_path);
    if(!f_json.exists()){
        f_json.open(QIODevice::WriteOnly);
        f_json.close();
    }
    //判断./wenwenxiezuo下是否有text目录
    QDir d2(text_path);
    if(!d2.exists()){
        d2.mkpath(text_path);
    }
    return 0;
}

QString FileConsole::getJsonPath(){
    QString json_path = QDir::homePath()+JsonSrc;// /home/xxx/.wenwenxiezuo/info.json
    return json_path;
}

QString FileConsole::getTextPath()
{
    return QDir::homePath()+TextDir;// /home/xxx/.wenwenxiezuo/text/
}
QString FileConsole::readJsonFile(){

    QFile f(getJsonPath());

    if(!f.open(QIODevice::ReadOnly)){
        return "null";//表示不存在该json文件
    }
    QString buffer = f.readAll();//读取全部数据
    f.close();
    return buffer;
}

int FileConsole::writeJsonFile(QJsonDocument doc_json)
{
    QFile jsonFile(getJsonPath());
    jsonFile.open(QFile::WriteOnly);

    if(jsonFile.isOpen())
    {
        jsonFile.write(doc_json.toJson());
        return 0;
    }
    return -1;
}

QJsonDocument FileConsole::getJsonDocument(QString str_json)
{
    QJsonDocument d = QJsonDocument::fromJson(str_json.toUtf8());//根据字符串解析出整个jsondocument
    //qDebug()<<d.isObject();
    //QJsonObject books = d.object();
    //qDebug()<<"books is "<<books.value("books");
    return d;
}

QString FileConsole::readText(QString fileName)
{
    if(!fileName.endsWith(".txt"))
    {
        fileName = fileName+".txt";
    }
    QString text_path = QDir::homePath()+TextDir; // /home/xx/./wenwenxiezuo/text/xxxx.txt

    text_path = text_path+fileName;
    QFile f(text_path);
    f.open(QFile::ReadOnly);
    if(f.isOpen())
    {
        QString str = f.readAll();
        f.close();
        return str;
    }
    else
        return "";
}

int FileConsole::writeText(QString fileName, QByteArray content)
{
    QString filePath = getTextPath()+fileName;
    QFile f(filePath);
    f.open(QFile::WriteOnly);
    if(f.isOpen())
    {
        f.write(content);
        qDebug()<<"向"<<fileName<<"写入："<<(QString)content;
        f.close();
        return 0;
    }
    else
        return -1;
}

void FileConsole::makeNewFile(QString fileName)
{
    QString filePath = getTextPath()+fileName;

    QFile f(filePath);
    if(f.exists())
    {
        return;
    }
    f.open(QFile::ReadWrite|QIODevice::Text);
    f.close();
}



