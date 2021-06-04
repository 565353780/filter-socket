//#include <Python.h>
#include <iostream>
#include <stdlib.h>
#include <QString>
#include <QDir>
#include <QDebug>

class LapNet_Train
{
public:
    LapNet_Train();
    ~LapNet_Train();

public:
    bool train(QString conda_env_name);
};
