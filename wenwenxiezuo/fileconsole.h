#ifndef FILECONSOLE_H
#define FILECONSOLE_H

#include<QDebug>
#include<QFile>
#include<QIODevice>
#include<QJsonDocument>
#include<QJsonObject>
#include<QJsonArray>
#include<QDir>
#include<QList>
#include"book.h"
#include"volume.h"
#include"chapter.h"

class FileConsole
{
public:
    FileConsole();

    //定义一些固定的目录(json文件在家目录下的.wenwenxiezuo目录下，文本文件在家目录下的./wenwenxiezuo/files目录下)
#define DocumentDir "/.wenwenxiezuo/";
#define JsonSrc "/.wenwenxiezuo/info.json";

#define TextDir  "/.wenwenxiezuo/text/";


    //获得info.json地址
    static QString getJsonPath();
    //获取text的目录地址 /home/xxx/.wenwenxiezuo/text
    static QString getTextPath();
    //判断家目录是否有固定的目录结构
    static bool isFilesComplete();
    //读取json文件
    static QString readJsonFile();
    //写入json文件
    static int writeJsonFile(QJsonDocument doc_json);
    //将读取的数据封装为一个jsondocument
    static QJsonDocument getJsonDocument(QString str_json);
    //读取文件数据
    static QString readText(QString fileName);
    //写入文件数据
    static int writeText(QString fileName,QByteArray content);
    //在./text/目录生成一个新的.txt文件
    static void makeNewFile(QString fileName);

    //获取打包的地址 桌面
    static QString getPackagePath();

};

#endif // FILECONSOLE_H
