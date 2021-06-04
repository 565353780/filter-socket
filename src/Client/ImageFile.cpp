#include "ImageFile.h"

#define JPEG_FILE_TYPE          1
#define BMP_FILE_TYPE           2
#define PNG_FILE_TYPE           3
#define GIF_FILE_TYPE           4

ImageFile::ImageFile()
{

}

ImageFile::ImageFile(QString path)
{
    int type;
    mpath = path;
    QDir dir_;
    if(!dir_.isAbsolutePath(mpath))
    {
        mpath = dir_.absoluteFilePath(mpath);
    }

    mwidth = 0;
    mheight = 0;
    mtype = "";
    int i = 0;
    int size;
    unsigned char *buff = NULL;
    FILE *fp;
    if((fp = fopen(mpath.toStdString().c_str(),"rb+")) == NULL)
    {
        mtype = "The file was not opened!";
        return;
    }
    fseek(fp,0,SEEK_END);
    size = ftell(fp);
    buff = (unsigned char*)malloc(size);
    if(buff)
        memset(buff,0,size);
    fseek(fp,0,SEEK_SET);
    if(fread(buff,1,size,fp)!=size)
    {
        mtype ="read error!";
        return;
    }

    type = check_fileType(buff);
    switch(type)
    {
        case JPEG_FILE_TYPE:
            mtype = "jpg";
            for(i = 0; i < size ; i++)
            {
                if(buff[i] == 0xff && buff[i+1] == 0xc0)
                {
                    mwidth = (buff[i+7]<<8) | buff[i+8];
                    mheight = (buff[i+5]<<8) | buff[i+6];
                    break;
                }
            }
            break;

        case BMP_FILE_TYPE:
            mtype = "bmp";
            for(i = 0; i < size ; i++)
            {
                if(buff[i] == 0x28 && buff[i+1] == 0x00)
                {
                    mwidth = (buff[i+7]<<24) | buff[i+6]<<16 | buff[i+5]<<8 | buff[i+4];
                    mheight = (buff[i+11]<<24) | buff[i+10]<<16 | buff[i+9]<<8 | buff[i+8];
                    break;
                }
            }
            break;

        case PNG_FILE_TYPE:
            mtype = "png";
            for(i = 0; i < size ; i++)
            {
                if(buff[i] == 0x49 && buff[i+1] == 0x48)
                {
                    mheight = (buff[i+8]<<24) | buff[i+9]<<16 | buff[i+10]<<8 | buff[i+11];
                    mwidth = (buff[i+4]<<24) | buff[i+5]<<16 | buff[i+6]<<8 | buff[i+7];
                    break;
                }
            }
            break;

        case GIF_FILE_TYPE:
            mtype = "gif";
            for(i = 0; i < size ; i++)
            {
                if(buff[i] == 0x00 && buff[i+1] == 0x2c)
                {
                    mwidth = (buff[i+7]<<8) | buff[i+6];
                    mheight = (buff[i+9]<<8) | buff[i+8];
                    break;
                }
            }
            break;
        default:
            break;
    }
    fclose(fp);
    free(buff);

}

ImageFile::~ImageFile()
{

}

QString ImageFile::type() const
{
    return mtype;
}

QString ImageFile::location() const
{
    int length = mpath.length();
    int pos = mpath.lastIndexOf('/');

    while (pos == length - 1)
    {
        pos = mpath.lastIndexOf('/' ,pos - 1);
        length--;
    }

    if (pos < 0)
    {
        return "";
    }

    return mpath.mid(0,pos + 1);
}

QString ImageFile::fileName() const
{
    int length = mpath.length();
    int pos = mpath.lastIndexOf('/');

    while (pos == length - 1)
    {
        pos = mpath.lastIndexOf('/' , pos - 1);
        length--;
    }

    if (pos < 0)
    {
        return "";
    }

    return mpath.mid(pos + 1);
}

double ImageFile::width() const
{
    return mwidth;
}

double ImageFile::height() const
{

    return mheight;
}

/*
　　通过文件头标识判断图片格式，
　　jpg： FF, D8
　　bmp： 42 4D
　　gif： 47 49 46 38
　　png： 89 50 4E 47
*/

/*在构造函数内获取像素宽高：mwidth、mheigh*/
int ImageFile::check_fileType(const unsigned char *buf)
{
    if(buf[0] == 0xFF && buf[1] == 0xd8 && buf[2] == 0xFF)
    {
        return JPEG_FILE_TYPE;
    }
    if(buf[0] == 0x42 && buf[1] == 0x4d)
    {
        return BMP_FILE_TYPE;
    }
    if(buf[0] == 0x47 && buf[1] == 0x49 && buf[2] == 0x46 && buf[3] == 0x38)
    {
        return GIF_FILE_TYPE;
    }
    if(buf[0] == 0x89 && buf[1] == 0x50 && buf[2] == 0x4e && buf[3] == 0x47)
    {
        return PNG_FILE_TYPE;
    }
    else
        return 0;
}
