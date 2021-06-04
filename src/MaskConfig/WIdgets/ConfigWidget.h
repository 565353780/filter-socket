#ifndef CONFIGWIDGET_H
#define CONFIGWIDGET_H

#include <QWidget>
#include <memory>
#include <QMap>
#include <QImage>

#include "ProgressBarWidget.h"

class QToolBar;
namespace MaskConfig{
class Config;
class Setter;
class PaintWidget;
class PanelTabDialog;
/**
 * @brief 操作界面主widget
 */
class ConfigWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConfigWidget(QWidget *parent = 0);

    Setter *current_setter();

    /**
     * @brief 读取qJson文件并初始化标定配置信息和对应的界面
     * @param json文件的相对路径
     */
    void initConfig(const QString& filename);
    /**
     * @brief 加载qJson文件读取标定配置信息
     * @param json文件的相对路径
     */
    void loadConfig(const QString& filename);
    /**
     * @brief 保存标定配置信息至json文件
     * @param filename
     */
    void saveConfig(const QString& filename);
    /**
     * @brief 读取图片
     * @param 图片相对路径
     */
    void loadImage(const QString& filename);
    /**
     * @brief 得到当前图片
     * @return 当前图片
     */
    const QImage& image() const;

    /**
     * @brief 得到Config类
     * @return 返回Config类
     */
    Config* config() {return config_.get();}
    /**
     * @brief 获取key对应的Setter类
     * @param key
     * @return Setter类
     */
    Setter* getSetter(const QString& key);
    /**
     * @brief 重新初始化key对应的Setter类
     * @param key
     */
    void changeSetter(const QString& key);

    /**
     * @brief 加载当前图片以及对应json文件
     */
    void loadCurrentFile();
    /**
     * @brief 保存当前图片的信息至json文件
     */
    void saveCurrentFile();

    /**
     * @brief 更新显示程序顶端的信息栏
     */
    void updateWindowsTitle();
signals:

public slots:
    /**
     * @brief 加载图片(暂无作用)
     */
    void slotLoadImage();
    /**
     * @brief 加载json文件(暂无作用)
     */
    void slotLoadConfig();
    /**
     * @brief 保存到json文件(暂无作用)
     */
    void slotSaveConfig();
    /**
     * @brief 加载上一张图片的json文件信息
     */
    void slotLoadPrev();

    /**
     * @brief 打开文件夹内的图片
     */
    void slotOpenDir();
    /**
     * @brief 转入下一张图片
     */
    void slotNext();
    /**
     * @brief 转入上一张图片
     */
    void slotPrev();
    /**
     * @brief 进行差值
     */
    void slotInterpolation();

    /**
     * @brief slotUpdatePaint   更新绘制
     */
    void slotUpdatePaint();

    void slotChangeToImageIndex(int index);
protected:
    void resizeEvent(QResizeEvent *event);
    void paintEvent(QPaintEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void resizeWidgets();
protected:
    void setterChanged();
private:

    std::shared_ptr<Config> config_;
    QMap<QString, std::shared_ptr<Setter> > setters_;
    Setter *current_setter_;

private:
    PanelTabDialog* dialog_;
    QToolBar* toolbar_;
    PaintWidget* paint_widget_;
    QImage image_;

    ProgressBarWidget* progress_bar_widget_;

    QString dirname_;
    QStringList filelist_;
    int current_file_id_;

    int marked_id_;

public:
    QAction* actPress0;
    QAction* actPress1;
    QAction* actPress2;
    QAction* actPress3;
    QAction* actPress4;
    QAction* actPress5;
    QAction* actPress6;
    QAction* actPress7;
    QAction* actPress8;
    QAction* actPress9;
    QAction* actPressBS;
};
}
#endif // CONFIGWIDGET_H
