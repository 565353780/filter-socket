
#include <QApplication>
#include <QLabel>
#include <QSurfaceFormat>

#include <QOpenGLContext>
#include <QOffscreenSurface>
#include "Core/Q3DScene.h"
#include "Meshes/QCubeMesh.h"
#include "Core/QMaterial.h"
#include <QWidget>
#include <QOpenGLFramebufferObject>
#include <QDebug>
#include "Widgets/QRenderWidget.h"
#include "Meshes/QQuadMesh.h"
#include "Meshes/QPointMapMesh.h"
#include "PointMapWidget.h"
#include "Evaluator/QShaderEvaluator.h"
#include <QImage>
#include <ctime>
#include "Widgets/QModelFinderWidget.h"
#include <QDir>
#include <QFileInfo>
#include "Meshes/QPrimitiveMesh.h"
#include "Finder/QFinderConfig.h"
#include "Core/QMathUtil.h"

void loadPointClound(const QString &filename, int &w, int &h, std::vector<float> &points, std::vector<uchar> &colors)
{
    FILE *fp = fopen(filename.toLocal8Bit().data(), "rb");
    fread(&w, sizeof(w), 1, fp);
    fread(&h, sizeof(h), 1, fp);

    points.resize(3 * w * h);
    colors.resize(3 * w * h);
    QVector3D center;
    int count = 0;
    for(int j = 0; j < h; j++)
    {
        for(int i = 0; i < w; i++)
        {
            float v[3];
            int c[3];
            fread(v, sizeof(float), 3, fp);
            fread(c, sizeof(int), 3, fp);
            if(v[2] > 0)
            {
                center += QVector3D(v[0], v[1], v[2]);
                count++;
            }
            if(v[0] == 0 && v[1] == 0 && v[2] == 0)
            {
                v[0] = -999999;
                v[1] = -999999;
                v[2] = -999999;
            }
            for(int k = 0; k < 3; k++)
            {
                points[3 * (j * w + i) + k] = v[k];
                colors[3 * (j * w + i) + k] = c[k];
            }

        }
    }
    fclose(fp);
    center /= count;
//    qDebug()<<center;
}
void testEvaluator()
{
    GCL::QShaderEvaluator evalator;

//    evalator.setPackRange(256*256);
    QImage testimg(2048, 2048, QImage::Format_ARGB32);

    double sum = 0.0;
    srand(clock());
    double sum1 = 0.0;
    for(int i = 0; i < testimg.width(); i++)
    {
        for(int j = 0; j < testimg.height(); j++)
        {
            double val = rand() % 400;

            QVector4D vv = GCL::QMaterial::packInt(val);

            vv *= 255;
            testimg.setPixel(i, j, qRgba(vv.x(), vv.y(), vv.z(), vv.w()));

            sum += val;
            QRgb c0 = testimg.pixel(i, j);
            QVector4D v1(qRed(c0), qGreen(c0), qBlue(c0), qAlpha(c0));
            v1 /= 255.0;
            sum1 += (GCL::QMaterial::unpackInt(v1)); // GCL::QMaterial::unpackInt( QVector4D(floor(vv.x()),floor(vv.y()),floor(vv.z()),floor(vv.w())) / 255.0);

            //            if(val > sum) sum = val;
        }
    }

    qDebug() << int(sum) << " " << int(sum1) << " " << (sum - sum1) << " " << GCL::QMaterial::packInt(sum) * 255.0;


    QOpenGLTexture texture(testimg);
    texture.setMinificationFilter(QOpenGLTexture::Nearest);
    texture.setMagnificationFilter(QOpenGLTexture::Nearest);
    texture.create();
    GCL::QShaderEvaluator::Problem problem;
    problem.texture_id_ = texture.textureId();
    problem.w_ = testimg.width();
    problem.h_ = testimg.height();
    problem.val_type_ = GCL::QShaderEvaluator::VT_Pack;
    problem.func_type_ = GCL::QShaderEvaluator::Func_Sum;
    problem.valid_min_ = 0.0 / 255.0;
    problem.valid_max_ = 1000.0;
    problem.map_size_ = GCL::QShaderEvaluator::Size_4096;
    double val1 = evalator.evalTexture(problem);

    double sumE = 0.0;
    for(int i = 0; i < testimg.width(); i++)
    {
        for(int j = 0; j < testimg.height(); j++)
        {
            QRgb c0 = testimg.pixel(i, j);
//            QRgb rgb = evalator.image_.pixel(i,j);

            QVector4D vv(qRed(c0), qGreen(c0), qBlue(c0), qAlpha(c0));

            vv /= 255.0;
            double val = (GCL::QMaterial::unpackInt(vv));
            sumE += val;
        }
    }
    qDebug() << int(val1) << " " << int(sumE);
    qDebug() << (val1 - sum) << " " << (val1 - sum) / (testimg.width() * testimg.height());
}

int testOnScreen(QString filename, QString cfg_filename = "finder.json")
{
    using namespace GCL;
    GCL::QModelFinderWidget rw;

//        testEvaluator();

//        return 0;
//    rw.loadModel("c3.obj");

//  传入点云图
    int w, h;
    std::vector<float> points;
    std::vector<uchar> colors;
    w = 1280 * 0.85;
    h = 1024 * 0.85;

    rw.setInitOffset(QVector3D(0, 0, -18));
    rw.loadConfig(cfg_filename);
    rw.loadPointMapPLY(filename, -1, 0, false, 0);
    rw.resize(w, h);

    qDebug() << w << " " << h;

    return QApplication::exec();
}
int testOffScreen(QString filename)
{
    using namespace GCL;
    clock_t t0 = clock();

    // 1、设定屏幕宽、高
    int w, h;
    w = 1280 * 0.8;
    h = 1024 * 0.8;
    // 2、Finder初始化
    QModelFinder rw;
    rw.create();
    rw.init();
    rw.resize(w, h);


    // 3、读入要寻找的3D模型
    rw.loadModel("c3.obj");




    // 4、获得场景scene, 设置相机矩阵

    GCL::Q3DScene *scene = rw.getScene();

    QMatrix4x4 matrix;
    matrix.lookAt(QVector3D(0, 0, 0), QVector3D(0, 0, 1), QVector3D(0, 1, 0));
    matrix.translate(-613, -49, -216);
    scene->setDefaultModelMatrix(matrix);
    scene->setDefaultView();
    /**  也可以通过四元数的方式来设置
        QQuaternion quat(60,QVector3D(0,0,1));    //  度数范围0-360
        matrix.setToIdentity();
        matrix.rotate(quat);
        matrix.translate(0,0,0);
    **/





    // 5、 设置参数
    rw.setDebugMode(true);  // debug 模式为true， 会输出中间过程图片:  b.png是求采样点的参考图片

    /**
     *  rw.setBoundaryExtendLevel(k);
     *  rw.setPartLeastSize(100);
     *
     * 边界点延展的次数, 边界延展---对当前每个的边界点, 使其周围8个点也成为了边界点。
     *  这么操作的目的是将较近的边界连通块更多连通， 如果原先两个边界连通块距离小于k, 那么边界延展之后它们就会连结到一起。
     *  这些边界连通块，将图片分割成多个区域，这些区域一般在物体中间部位(可以参考b.png), 取这些区域的中心作为采样点。
     *
     * extendlevel等级, 跟finder的分辨率<w,h>有关————extendlevel是像素距离,  分辨率越大， extendlevel应该设得越大；
     * 但设置较大分辨率， 会影响获得采样点的速度。
     *
     *
     * partSize, 意思是某个内部区块的边界点总数, 也可以理解为某个内部区块的周长;  partLeastSize的意思是, 我们认为某个区块有效，
     * 前提是它的周长大于给定的partLeastSize。  这样就有效过滤点较多的噪点区域
    **/
    rw.setBoundaryExtendLevel(14);
    rw.setPartLeastSize(100);

    /**
     *  DepthRange: z_max, z_min
     *  只处理这个z值范围内的数据
     **/
    rw.setDepthRange(765, 600);
    /**
     *  匹配得分阈值： 不同项目不一样, 需要调试。
     **/
    rw.setScoreThreshold(2.5);
    /**
      *  局部迭代次数,  理论上次数越高越好。 但迭代次数越多，越需要时间。 因此8-12次比较理想。
     **/
    rw.setLocalIterationNum(8);


    // 6、 读入点云, 并查找模型， 一次找最多8个

    /**
     *  loadPointMapPLY(const QString &filename, int w, int h, bool zflip, float flip_z)
     *  如果zflip为true, 那么读入的点位置会做一个
     *      v.z = flip_z - v.z 的操作
     *      目的是将z值尽可能都变成正数， 且越远数值越大。
     *  如果每个顶点的v.z都为正， 且满足越远z值越大这一条件，则zflip设置为false, flip_z不用设置
     *
     **/
    rw.loadPointMapPLY(filename, w, h, true, 800);
    rw.clearResults();
    rw.quickFindAllModels();


    qDebug() << (clock() - t0) / (CLOCKS_PER_SEC / 1000) << "ms: Total";

    /**
     *  获得结果， eular(rx,ry,rz),  offset(x,y,z) 匹配得分
     **/
    QList<QModelFinder::ResultTransform> list = rw.getResults();
    qDebug() << list.first().euler_ << " " << list.first().offset_ << " " << list.first().score_;


    // 以下只是为了保存图片用
    QOpenGLFramebufferObject fbo(QSize(w, h), QOpenGLFramebufferObject::CombinedDepthStencil);

    fbo.bind();

    rw.render();

    fbo.bindDefault();
    fbo.toImage(true).save("testOff.png");

    return 0;



}
void setGLFormat()
{
    QSurfaceFormat format;
    if(QOpenGLContext::openGLModuleType() == QOpenGLContext::LibGL)
    {
        format.setRenderableType(QSurfaceFormat::OpenGL);
    }
    else
    {
        format.setRenderableType(QSurfaceFormat::OpenGLES);
        //surfaceFmt.setVersion(3,0);
    }
//    format.setRenderableType(QSurfaceFormat::OpenGLES);

    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);
    QSurfaceFormat::setDefaultFormat(format);
}

float getRandFloat()
{
    return ((rand() % 200 - 100) / 100.0) ;
}
int main(int argc, char *argv[])
{

    using namespace GCL;

    QApplication app(argc, argv);
    setGLFormat();
    GCL::QRenderWidget r;
    r.show();
    r.makeCurrent();
    testEvaluator();

    return 0;

}
