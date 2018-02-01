#include "volume.h"


Volume::Volume(QString volumeName)
{
    this->VolumeName = volumeName;

}
Volume::Volume()
{

}

QString Volume::getVolumeName(){
    return this->VolumeName;
}

void Volume::setVolumeName(QString newVolumeName){
    this->VolumeName = newVolumeName;
}

int Volume::appendChapter(Chapter newChapter){
    if(this->contained(newChapter.getChapterName()) == -1){
        //之前该列表没有包含有此章节名的章节元素
        QString chapterName = newChapter.getChapterName();
        QString fileName = newChapter.getFileName();
        Chapter chapter(chapterName,fileName);

        this->getChapterListPointer()->append(Chapter(chapterName,fileName));
        return 0;
    }
    else{
        //已经有这个章节名了，不能再加一个重复的章节。
        return -1;
    }
}

int Volume::contained(QString chapterName)
{
    for(int i=0;i<this->ChapterList.length();i++){

        if(this->ChapterList[i].getChapterName() == chapterName){
            //发现该章节列表已经存在该名称的章节，返回该章节所在索引
            return i;
        }
    }
    return -1;//不存在
}

int Volume::removeChapter(QString chapterName){
    int index = this->contained(chapterName);
    if(index == -1){
        //不存在该章节，无法移除
        return -1;
    }
    else{
        //存在该章节
        this->ChapterList.removeAt(index);//移除index索引的元素
        return 0;
    }
}
int Volume::removeChapter(Chapter chapter){
    int index = this->contained(chapter.getChapterName());//获取
    if(index == -1){
        //不存在该章节，无法移除
        return -1;
    }
    else{
        //存在该章节
        this->ChapterList.removeAt(index);//移除index索引的元素
        return 0;
    }
}


int Volume::chapterCount()
{
    return this->ChapterList.count();
}

Chapter Volume::getChapter(int index)
{
    return this->ChapterList.at(index);
}

Chapter Volume::getChapter(QString chapterName)
{
    int index = this->contained(chapterName);

    if(index == -1)
    {
        Chapter c("","");
        return c;
    }

    return getChapter(index);
}

QList<Chapter>*Volume::getChapterListPointer()
{
    return &(this->ChapterList);
}




