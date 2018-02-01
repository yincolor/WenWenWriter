#include "chapter.h"

Chapter::Chapter(QString chapterName, QString fileName)
{
    this->ChapterName = chapterName;
    this->FileName = fileName;

}
Chapter::Chapter()
{

}

QString Chapter::getChapterName(){
    return this->ChapterName;
}

void Chapter::setChapterName(QString newChapterName){
    this->ChapterName = newChapterName;
}

void Chapter::setFileName(QString newFileName)
{
    this->FileName = newFileName;
}

QString Chapter::getFileName(){
    return this->FileName;
}
