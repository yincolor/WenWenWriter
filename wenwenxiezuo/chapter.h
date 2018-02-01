#ifndef CHAPTER_H
#define CHAPTER_H
#include<QString>
#include<QList>

class Chapter
{
public:
    Chapter(QString chapterName, QString fileName);
    Chapter();
    QString getChapterName();
    void setChapterName(QString newChapterName);
    void setFileName(QString newFileName);
    QString getFileName();
private:
    QString ChapterName;
    QString FileName;
};

#endif // CHAPTER_H
