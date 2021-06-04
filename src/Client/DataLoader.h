#ifndef DATALOADER_H
#define DATALOADER_H

#include <QObject>
#include <QDir>
#include <QQueue>
#include <QDebug>

class DataLoader : public QObject
{
    Q_OBJECT

public:
    explicit DataLoader(QObject *parent = nullptr);
    ~DataLoader();

private:
    void countFiles(QString dir_path);

    void findFiles(QString dir_path);

    void countDir(QString dir_path, QStringList filters);

    void loadDir(QString dir_path, QStringList filters);

private:
    QDir dir_;

    int each_folder_select_image_num_;
    QStringList ignore_folder_name_list_;

    int find_files_num_;

public slots:
    void slot_LoadDir(QString dir_path, QStringList filters);

    void slot_setLoadFileNumForEachFolder(int load_file_num_for_each_folder);

signals:
    void signal_findFilesNum(int);

    void signal_AddOpenFilePath(QString);

    void signal_LoadDir_finished();
};

#endif // DATALOADER_H
