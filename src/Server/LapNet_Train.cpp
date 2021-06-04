#include "LapNet_Train.h"

LapNet_Train::LapNet_Train()
{
//    wchar_t python_home_path[] = L"D:/Program Files/Anaconda2/envs/pytorch";

//    Py_SetPythonHome(python_home_path);

//    Py_Initialize();

//    PyRun_SimpleString("import sys");
//    PyRun_SimpleString("sys.path.append('./../src/Python/LapNet/')");
}

LapNet_Train::~LapNet_Train()
{
//    Py_Finalize();
}

bool LapNet_Train::train(QString conda_env_name)
{
    if(conda_env_name != "")
    {
#ifdef WIN32
        system(("start conda activate " + conda_env_name + "&&python ../src/Python/LapNet/train.py&&exit").toStdString().c_str());
#endif
#ifdef Linux
        system(("RUN_AFTER_BASHRC=\"conda activate " + conda_env_name + "&&python ../src/Python/LapNet/train.py&&exit\" gnome-terminal").toStdString().c_str());
#endif
    }
    else
    {
#ifdef WIN32
        system("start python ../src/Python/LapNet/train.py&&exit");
#endif
#ifdef Linux
        system("RUN_AFTER_BASHRC=\"python ../src/Python/LapNet/train.py&&exit\" gnome-terminal");
#endif
    }

//    if (!Py_IsInitialized())
//    {
//        return -1;
//    }

//    PyObject* pModule = PyImport_ImportModule("train");

//    if (!pModule)
//    {
//        std::cout << "Can not open python file!\n" << std::endl;

//        return -1;
//    }

//    PyObject* pFunLapNetTrain= PyObject_GetAttrString(pModule,"Main_LapNet_Train");

//    if(!pFunLapNetTrain)
//    {
//        std::cout << "Get function Main_LapNet_Train failed!\n" << std::endl;
//        return -1;
//    }

//    std::cout << "begin LapNet_Train" << std::endl;

//    PyEval_CallObject(pFunLapNetTrain,NULL);

//    std::cout << "end LapNet_Train" << std::endl;

    return 1;
}
