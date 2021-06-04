#include "MaskDrawWidget.h"

MaskDrawWidget::MaskDrawWidget(QWidget *parent)
{
    p1 = QPoint(0,0);
    p2 = QPoint(0,0);

    background_scale = 1.0;

    show_mask = true;
    remove_mode = false;
    polygon_start = false;
    mask_changed = false;
    draw_strict_line = false;

    current_file_index = -1;
    current_activate_poly = -1;

    setFocusPolicy(Qt::StrongFocus);
}

MaskDrawWidget::~MaskDrawWidget()
{

}

void MaskDrawWidget::set_background(QImage img)
{
    background = img.copy();

    background_scale = 1.0 * this->width() / background.width();

    background_scale = fmin(background_scale, 1.0 * this->height() / background.height());

    new_img_width = int(background.width() * background_scale);
    new_img_height = int(background.height() * background_scale);

    start_point.setX((this->width() - new_img_width) / 2.0);
    start_point.setY((this->height() - new_img_height) / 2.0);

    update();
}

void MaskDrawWidget::set_background(QString img_path)
{
    background = QImage(img_path);

    background_scale = 1.0 * this->width() / background.width();

    background_scale = fmin(background_scale, 1.0 * this->height() / background.height());

    new_img_width = int(background.width() * background_scale);
    new_img_height = int(background.height() * background_scale);

    start_point.setX((this->width() - new_img_width) / 2.0);
    start_point.setY((this->height() - new_img_height) / 2.0);

    update();
}

void MaskDrawWidget::update_background()
{
    set_background(fileNames[current_file_index]);
}

QPoint MaskDrawWidget::trans_widget_to_img(QPoint &widget_point)
{
    QPoint trans_point = widget_point;

    trans_point -= start_point;
    trans_point.setX(int(trans_point.x() / background_scale));
    trans_point.setY(int(trans_point.y() / background_scale));

    qDebug() << trans_point << endl;

    return trans_point;
}

void MaskDrawWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    painter.drawImage(QRect(start_point.x(), start_point.y(), new_img_width, new_img_height), background);

    if(show_mask)
    {
        if(mask_episode_list.size() > 0)
        {
            for(int i = 0; i < mask_episode_list.size(); ++i)
            {
                if(current_activate_poly == i)
                {
                    painter.setPen(Qt::red);
                }
                else
                {
                    painter.setPen(Qt::blue);
                }

                for(int j = 0; j < mask_episode_list[i].size() - 1; ++j)
                {
                    painter.drawLine(mask_episode_list[i][j], mask_episode_list[i][j + 1]);
                }
            }
            if(p1 != p2)
            {
                painter.drawLine(p1, p2);
            }
        }

//        if(mask_episode_list.size() > 1)
//        {
//            for(int i = 1; i < mask_episode_list.size() - 1; ++i)
//            {
//                QPolygon poly;

//                for(int j = 0; j < mask_episode_list[i].size() - 1; ++j)
//                {
//                    poly.append(mask_episode_list[i][j]);
//                }

//                painter.setBrush(Qt::gray);
//                painter.drawConvexPolygon(poly);
//            }

//            if(!polygon_start)
//            {
//                QPolygon poly;

//                for(int j = 0; j < mask_episode_list[mask_episode_list.size() - 1].size() - 1; ++j)
//                {
//                    poly.append(mask_episode_list[mask_episode_list.size() - 1][j]);
//                }

//                painter.setBrush(Qt::gray);
//                painter.drawConvexPolygon(poly);
//            }
//        }
    }
}

void MaskDrawWidget::mousePressEvent(QMouseEvent *event)
{
    if(show_mask)
    {
        if(event->buttons() == Qt::LeftButton)
        {
            if(!polygon_start)
            {
                p1 = event->pos();
                p2 = event->pos();

                QPoint trans_point = p1;
                //QPoint trans_point = trans_widget_to_img(p1);

                polygon_start = true;

                QVector<QPoint> current_point_list;

                current_point_list.append(trans_point);

                mask_episode_list.append(current_point_list);

                ++current_activate_poly;
            }
        }
        else if(event->buttons() == Qt::RightButton)
        {
            remove_mode = true;
        }
    }

    mouseMoveEvent(event);

    update();
}

void MaskDrawWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(show_mask)
    {
        if(event->buttons() == Qt::LeftButton)
        {
            p2 = event->pos();

            if(draw_strict_line)
            {
                if(abs(p2.x() - p1.x()) < abs(p2.y() - p1.y()))
                {
                    p2.setX(p1.x());
                }
                else
                {
                    p2.setY(p1.y());
                }

                int min_dist_to_angle;
                QPoint target_point;
                bool is_x_axis;

                int min_x_dist;
                int min_y_dist;

                target_point = mask_episode_list[0][0];

                min_x_dist = abs(p2.x() - target_point.x());
                min_y_dist = abs(p2.y() - target_point.y());

                if(min_x_dist > 0 && min_x_dist < min_y_dist)
                {
                    min_dist_to_angle = min_x_dist;
                    is_x_axis = true;
                }
                else if(min_y_dist > 0)
                {
                    min_dist_to_angle = min_y_dist;
                    is_x_axis = false;
                }

                if(mask_episode_list.size() > 0)
                {
                    for(int i = 0; i < mask_episode_list.size(); ++i)
                    {
                        for(int j = 0; j < mask_episode_list[i].size(); ++j)
                        {
                            QPoint current_angle_point = mask_episode_list[i][j];

                            min_x_dist = abs(p2.x() - current_angle_point.x());
                            min_y_dist = abs(p2.y() - current_angle_point.y());

                            if(min_x_dist > 0 && min_x_dist < min_y_dist && min_x_dist < min_dist_to_angle)
                            {
                                min_dist_to_angle = min_x_dist;
                                is_x_axis = true;
                                target_point = current_angle_point;
                            }
                            else if(min_y_dist > 0 && min_y_dist < min_dist_to_angle)
                            {
                                min_dist_to_angle = min_y_dist;
                                is_x_axis = false;
                                target_point = current_angle_point;
                            }
                        }
                    }
                }

                if(min_dist_to_angle < 10)
                {
                    if(is_x_axis)
                    {
                        p2.setX(target_point.x());
                    }
                    else
                    {
                        p2.setY(target_point.y());
                    }
                }
            }
            else if(mask_episode_list.size() > 0)
            {
                QPoint current_start_point = mask_episode_list[mask_episode_list.size() - 1][0];

                if((p2 - current_start_point).manhattanLength() < 10)
                {
                    p2 = current_start_point;
                }
            }
        }
    }

    update();
}

void MaskDrawWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(show_mask)
    {
        p1 = p2;

        if(remove_mode)
        {
            remove_mode = false;
            polygon_start = false;

            if(mask_episode_list.size() > 0)
            {
                mask_episode_list.pop_back();

                --current_activate_poly;

                emit poly_changed();
            }
        }
        else
        {
            int current_poly_num = mask_episode_list.size();

            QPoint trans_point = p2;
            //QPoint trans_point = trans_widget_to_img(p2);

            if(polygon_start)
            {
                mask_episode_list[mask_episode_list.size() - 1].append(trans_point);
            }

            if(polygon_start && mask_episode_list.size() > 0)
            {
                if(mask_episode_list[mask_episode_list.size() - 1].size() > 0)
                {
                    QPoint current_start_point = mask_episode_list[mask_episode_list.size() - 1][0];

                    if(p2 == current_start_point)
                    {
                        polygon_start = false;

                        mask_changed = true;
                    }
                }
            }

            if(mask_episode_list.size() > 0)
            {
                if(mask_episode_list[mask_episode_list.size() - 1].size() == 2)
                {
                    if(mask_episode_list[mask_episode_list.size() - 1][0] == mask_episode_list[mask_episode_list.size() - 1][1])
                    {
                        mask_episode_list.pop_back();

                        --current_activate_poly;
                    }
                }
            }

            if(current_poly_num == mask_episode_list.size() && !polygon_start)
            {
                emit poly_changed();
            }
        }
    }

    update();
}

void MaskDrawWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
    {
        draw_strict_line = true;
    }
}

void MaskDrawWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
    {
        draw_strict_line = false;
    }
}

void MaskDrawWidget::findFile(const QString& path, vector<QString>& fileNames)
{
    QDir dir(path);
    if(!dir.exists())
    {
        qDebug() << "findFile.path doesn't exist !" << endl;

        return;
    }

    //获取filePath下所有文件夹和文件
    dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);//文件夹|文件|不包含./和../

    //排序文件夹优先
    dir.setSorting(QDir::DirsFirst);

    //获取文件夹下所有文件(文件夹+文件)
    QFileInfoList list = dir.entryInfoList();

    /**********直接获取带文件后缀的文件;如果使用,则只搜索当前文件夹下的文件*************
    QStringList filer;
    filer << "*.jpg" <<"*.bmp";//设定需要的文件类型(*为所有类型)
    QFileInfoList list = dir.entryInfoList(filer);
    //QList<QFileInfo> *list= new QList<QFileInfo>(dir.entryInfoList(filter));
    /*******************************************************************************/

    /**********************只获取文件,只搜索当前文件夹下的文件************************
    QStringList list= dir.entryList(filer, QDir::Files | QDir::NoDotAndDotDot);
    /*******************************************************************************/

    if(list.size() == 0)
    {
        return;
    }

    //遍历
    for(int i = 0; i < list.size(); i++)
    {
        QFileInfo fileInfo = list.at(i);

        if(fileInfo.isDir())//判断是否为文件夹
        {
            findFile(fileInfo.filePath(), fileNames);//递归开始
        }
        else
        {
            if(fileInfo.suffix() == "bmp" || fileInfo.suffix() == "jpg" || fileInfo.suffix() == "png" || fileInfo.suffix() == "jpeg")//设定后缀
            {
                fileNames.emplace_back(list.at(i).filePath());
            }
        }
    }
}

void MaskDrawWidget::get_File_List(QString path)
{
    if(file_path_copy == path)
    {
        return;
    }

    fileNames.clear();

    findFile(path, fileNames);

    if(fileNames.size() > 0)
    {
        current_file_index = 0;

        file_path_copy = path;

        train_json_path = "../Client_DataBase/message_set/" + file_path_copy.replace("/", "_").replace(":", "_") + "_poly_yolo.json";

        update_background();
    }
    else
    {
        current_file_index = -1;
    }
}

void MaskDrawWidget::show_Previous_Img()
{
    if(current_file_index == -1)
    {
        qDebug() << "No image is opened !" << endl;

        return;
    }

    if(current_file_index == 0)
    {
        qDebug() << "Current image is the first image !" << endl;

        return;
    }

    --current_file_index;

    update_background();

    update_train_json();
}

void MaskDrawWidget::show_Next_Img()
{
    if(current_file_index == -1)
    {
        qDebug() << "No image is opened !" << endl;

        return;
    }

    if(current_file_index == fileNames.size() - 1)
    {
        qDebug() << "Current image is the last image !" << endl;

        return;
    }

    ++current_file_index;

    update_background();

    update_train_json();
}

void MaskDrawWidget::load_train_json()
{
    QDir dir("./");

    qDebug() << dir.absolutePath() << endl;
}

void MaskDrawWidget::update_train_json()
{
    QFile file(train_json_path);

    if(file.open(QFile::ReadWrite))
    {
        qDebug() << "opened.";

        file.close();
    }
    else
    {
        qDebug() << "MaskDrawWidget::update_train_json : file open failed.";
    }

    qDebug() << train_json_path << endl;
}

bool MaskDrawWidget::write_json(QString json_file)
{
    QByteArray array;
    QJsonDocument doc;
    doc.setObject(train_json_obj);
    array.push_back(doc.toJson());

    QFile file(json_file);

    if(file.open(QFile::WriteOnly))
    {
        file.write(array);
        file.close();

        return true;
    }
    else
    {
        qDebug() << "MaskDrawWidget::write_json : file open failed.";\

        return false;
    }
}

bool MaskDrawWidget::read_json(QString json_file)
{
    QFile file(json_file);

    if(file.open(QFile::ReadOnly))
    {
        QByteArray array = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(array);
        train_json_obj = doc.object();

        return true;
    }
    else
    {
        qDebug() << "MaskDrawWidget::read_json : file open failed.";

        return false;
    }
}
