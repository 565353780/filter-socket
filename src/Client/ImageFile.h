#ifndef IMAGEFILE_H
#define IMAGEFILE_H

#include <QObject>
#include <QString>
#include <QDir>

class  ImageFile : public QObject
{
public:
    ImageFile();
    ImageFile(QString path);
    ~ImageFile();

    QString type() const;
    QString location() const;
    QString fileName() const;
    double width() const;
    double height() const;

private:
    int check_fileType(const unsigned char *buf);

private:
    char* src;
    QString mpath;
    QString mtype;
    double mwidth;
    double mheight;
};

#endif // IMAGEFILE_H
