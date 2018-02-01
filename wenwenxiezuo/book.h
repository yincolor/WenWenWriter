#ifndef BOOK_H
#define BOOK_H
#include<QString>
#include<QList>
#include"volume.h"
#include"chapter.h"

class Book
{
public:
    Book(QString bookName);
    Book();
    QString getBookName();
    void setBookName(QString newBookName);
    int appendVolume(Volume newVolume);
    int contained(QString volumeName);
    int removeVolume(QString volumeName);
    int removeVolume(Volume volume);
    int volumeCount();//获取卷的个数
    Volume getVolume(int index);
    Volume getVolume(QString volumeName);
    QList<Volume>* getVolumeListPointer();
    QList<Volume> getVolumeList();

    QList<Volume> VolumeList;
private:

    QString BookName;

};

#endif // BOOK_H
