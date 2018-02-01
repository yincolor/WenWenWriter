#include"book.h"

Book::Book(QString bookName)
{
    this->BookName = bookName;
}

QString Book::getBookName(){
    return this->BookName;
}

void Book::setBookName(QString newBookName){
    this->BookName = newBookName;
}

int Book::appendVolume(Volume newVolume){
    if(this->contained(newVolume.getVolumeName()) == -1)
    {
        this->VolumeList.append(newVolume);
        return 0;
    }
    else{
        return -1;
    }
}

int Book::contained(QString volumeName){
    for(int i=0;i<this->VolumeList.length();i++){
        if(this->VolumeList[i].getVolumeName() == volumeName){
            return i;
        }
    }
    return -1;//不存在该卷
}

int Book::removeVolume(QString volumeName){
    int index = this->contained(volumeName);
    if(index == -1){
        //不存在该章节，无法移除
        return -1;
    }
    else{
        //存在该章节
        this->VolumeList.removeAt(index);//移除index索引的volume元素
        return 0;
    }
}

int Book::removeVolume(Volume volume){
    int index = this->contained(volume.getVolumeName());
    if(index == -1){
        //不存在该章节，无法移除
        return -1;
    }
    else{
        //存在该章节
        this->VolumeList.removeAt(index);//移除index索引的volume元素
        return 0;
    }
}
int Book::volumeCount()
{
    return this->VolumeList.count();
}

Volume Book::getVolume(int index)
{
    return this->VolumeList.at(index);
}
Volume Book::getVolume(QString volumeName)
{
    int index = this->contained(volumeName);
    if(index == -1)
    {
        Volume v("");
        return v;
    }
    return getVolume(index);
}

QList<Volume>* Book::getVolumeListPointer()
{
    return &(this->VolumeList);
}
QList<Volume> Book::getVolumeList()
{
    return this->VolumeList;
}
