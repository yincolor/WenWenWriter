#ifndef VOLUME_H
#define VOLUME_H
#include<QString>
#include<QDebug>
#include<QList>
#include"chapter.h"
class Volume
{
public:
    Volume(QString volumeName);
    Volume();
    QString getVolumeName();
    void setVolumeName(QString newVolumeName);
    int appendChapter(Chapter newChapter);
    int contained(QString chapterName);
    int removeChapter(QString chapterName);
    int removeChapter(Chapter chapter);

    int chapterCount();//获取章节个数
    Chapter getChapter(int index);//获取章节对象
    Chapter getChapter(QString chapterName);//获取章节对象
    QList<Chapter>* getChapterListPointer();
    QList<Chapter> ChapterList;
private:
    QString VolumeName;

};

#endif // VOLUME_H
