#include "CameraMaskWidget.h"

CameraMaskWidget::CameraMaskWidget(QWidget *parent)
{
    p1 = QPoint(0,0);
    p2 = QPoint(0,0);

    background_scale = 1.0;

    init_ok = false;
    show_mask = true;
    remove_mode = false;
    polygon_start = false;
    mask_changed = false;
    draw_strict_line = false;

    setFocusPolicy(Qt::StrongFocus);
}

CameraMaskWidget::~CameraMaskWidget()
{

}

void CameraMaskWidget::set_background(QImage img)
{
    background = img.copy();

    background_scale = 1.0 * this->width() / background.width() / 2.0;

    background_scale = fmin(background_scale, 1.0 * this->height() / background.height());

    new_img_width = int(background.width() * background_scale);
    new_img_height = int(background.height() * background_scale);

    start_point.setX((this->width() / 2.0 - new_img_width) / 2.0);
    start_point.setY((this->height() - new_img_height) / 2.0);

    update();
}

QPoint CameraMaskWidget::trans_widget_to_img(QPoint &widget_point)
{
    QPoint trans_point = widget_point;

    trans_point -= start_point;
    trans_point.setX(int(trans_point.x() / background_scale));
    trans_point.setY(int(trans_point.y() / background_scale));

    return trans_point;
}

QPoint CameraMaskWidget::trans_img_to_widget(QPoint &img_point)
{
    QPoint trans_point = img_point;

    trans_point.setX(int(trans_point.x() * background_scale));
    trans_point.setY(int(trans_point.y() * background_scale));

    trans_point += start_point;

    return trans_point;
}

QPoint CameraMaskWidget::trans_widget_to_draw_img(QPoint &widget_point)
{
    QPoint trans_point = widget_point;

    trans_point -= start_bbox_point;

    return trans_point;
}

void CameraMaskWidget::paintEvent(QPaintEvent *event)
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

        if(mask_episode_list.size() > 1)
        {
            for(int i = 1; i < mask_episode_list.size() - 1; ++i)
            {
                QPolygon poly;

                for(int j = 0; j < mask_episode_list[i].size() - 1; ++j)
                {
                    poly.append(mask_episode_list[i][j]);
                }

                painter.setBrush(Qt::gray);
                painter.drawConvexPolygon(poly);
            }

            if(!polygon_start)
            {
                QPolygon poly;

                for(int j = 0; j < mask_episode_list[mask_episode_list.size() - 1].size() - 1; ++j)
                {
                    poly.append(mask_episode_list[mask_episode_list.size() - 1][j]);
                }

                painter.setBrush(Qt::gray);
                painter.drawConvexPolygon(poly);
            }
        }
    }

    if(point_pair_line.size() > 0)
    {
        for(int i = 0; i < point_pair_line.size(); ++i)
        {
            painter.drawLine(point_pair_line[i]);
        }
    }

    if(choose_point_pair)
    {
        painter.drawLine(pair1, pair2);
    }

    if(bbox_matrix.rows() > 0)
    {
        double start_bbox_x = start_bbox_point.x();
        double start_bbox_y = start_bbox_point.y();

        for(int i = 0; i < bbox_matrix.rows(); ++i)
        {
            for(int j = 0; j < bbox_matrix.cols(); ++j)
            {
                painter.drawLine(start_bbox_x + i * fixed_sub_bbox_dist, start_bbox_y + j *fixed_sub_bbox_dist, start_bbox_x + i * fixed_sub_bbox_dist, start_bbox_y + (j + 1) * fixed_sub_bbox_dist);
                painter.drawLine(start_bbox_x + i * fixed_sub_bbox_dist, start_bbox_y + j *fixed_sub_bbox_dist, start_bbox_x + (i + 1) * fixed_sub_bbox_dist, start_bbox_y + j * fixed_sub_bbox_dist);
                painter.drawLine(start_bbox_x + i * fixed_sub_bbox_dist, start_bbox_y + (j + 1) *fixed_sub_bbox_dist, start_bbox_x + (i + 1) * fixed_sub_bbox_dist, start_bbox_y + (j + 1) * fixed_sub_bbox_dist);
                painter.drawLine(start_bbox_x + (i + 1) * fixed_sub_bbox_dist, start_bbox_y + j *fixed_sub_bbox_dist, start_bbox_x + (i + 1) * fixed_sub_bbox_dist, start_bbox_y + (j + 1) * fixed_sub_bbox_dist);

                painter.drawText(start_bbox_point + QPoint(i * fixed_sub_bbox_dist, j * fixed_sub_bbox_dist), QString::number(bbox_matrix(i, j)));
            }
        }
    }
}

void CameraMaskWidget::mousePressEvent(QMouseEvent *event)
{
    if(choose_point_pair)
    {
        if(event->buttons() == Qt::LeftButton)
        {
            pair1 = event->pos();
            pair2 = event->pos();
        }
        else if(event->buttons() == Qt::RightButton)
        {
            remove_mode = true;
        }
    }
    else
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
                }
            }
            else if(event->buttons() == Qt::RightButton)
            {
                remove_mode = true;
            }
        }
    }

    if(test_mode)
    {
        QPoint event_pos = event->pos();

        QPoint test_p = trans_widget_to_img(event_pos);

        add_Detect_Position(test_p);
    }

    mouseMoveEvent(event);

    update();
}

void CameraMaskWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(choose_point_pair)
    {
        if(event->buttons() == Qt::LeftButton)
        {
            pair2 = event->pos();
        }
    }
    else
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
                else
                {
                    QPoint current_start_point = mask_episode_list[mask_episode_list.size() - 1][0];

                    if((p2 - current_start_point).manhattanLength() < 10)
                    {
                        p2 = current_start_point;
                    }
                }
            }
        }
    }

    update();
}

void CameraMaskWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(choose_point_pair)
    {
        if(remove_mode)
        {
            remove_mode = false;

            if(point_pair_line.size() > 0)
            {
                point_pair_line.pop_back();
            }
        }
        else
        {
            QLine pair_line(pair1, pair2);

            pair1 = pair2;

            point_pair_line.append(pair_line);
        }
    }
    else
    {
        if(show_mask)
        {
            p1 = p2;

            if(remove_mode)
            {
                remove_mode = false;

                if(mask_episode_list.size() > 0)
                {
                    mask_episode_list.pop_back();
                }
            }
            else
            {
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

                if(mask_episode_list[mask_episode_list.size() - 1].size() == 2)
                {
                    if(mask_episode_list[mask_episode_list.size() - 1][0] == mask_episode_list[mask_episode_list.size() - 1][1])
                    {
                        mask_episode_list.pop_back();
                    }
                }
            }
        }
    }

    update();
}

void CameraMaskWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
    {
        draw_strict_line = true;
    }

    if(event->key() == Qt::Key_T)
    {
        test_mode = true;
    }

    if(event->key() == Qt::Key_E)
    {
        test_mode = false;
    }
}

void CameraMaskWidget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Shift)
    {
        draw_strict_line = false;
    }
}

void CameraMaskWidget::init_camera()
{
    if(point_pair_line.size() < 3)
    {
        init_ok = false;

        qDebug() << "please give at least three point pairs!" << endl;

        return;
    }

    MatrixXd A(3, 3);
    MatrixXd b(3, 3);

    for(int i = 0; i < A.cols(); ++i)
    {
        A(0, i) = point_pair_line[i].p1().x();
        A(1, i) = point_pair_line[i].p1().y();
        A(2, i) = 1.0;

        b(0, i) = point_pair_line[i].p2().x();
        b(1, i) = point_pair_line[i].p2().y();
        b(2, i) = 1.0;
    }

    FullPivHouseholderQR<MatrixXd> solver(A);

    if(solver.rank() < 3)
    {
        init_ok = false;

        qDebug() << "three given point pairs are parallel, please give a new point pair!" << endl;

        return;
    }

    trans_matrix = b * solver.inverse();

    qDebug() << "finish init camera!" << endl;

    init_ok = true;
}

void CameraMaskWidget::get_Draw_Background_BBox(int& x_min, int& x_max, int& y_min, int& y_max)
{
    if(mask_episode_list.size() == 0)
    {
        qDebug() << "mask episode list is empty!" << endl;

        return;
    }

    if(mask_episode_list[0].size() == 0)
    {
        qDebug() << "mask episode list is empty!" << endl;

        return;
    }

    x_min = mask_episode_list[0][0].x();
    x_max = x_min;
    y_min = mask_episode_list[0][0].y();
    y_max = y_min;

    for(int i = 0; i < mask_episode_list.size(); ++i)
    {
        for(int j = 0; j < mask_episode_list[i].size(); ++j)
        {
            int current_x = mask_episode_list[i][j].x();
            int current_y = mask_episode_list[i][j].y();

            if(current_x < x_min)
            {
                x_min = current_x;
            }
            if(current_x > x_max)
            {
                x_max = current_x;
            }

            if(current_y < y_min)
            {
                y_min = current_y;
            }
            if(current_y > y_max)
            {
                y_max = current_y;
            }
        }
    }
}

void CameraMaskWidget::get_Common_Int(int& a, int& b, int& common_int)
{
    if(a == 0)
    {
        qDebug() << "get_Common_Int.input(a) == 0" << endl;

        common_int = 0;

        return;
    }

    if(b == 0)
    {
        qDebug() << "get_Common_Int.input(b) == 0" << endl;

        common_int = 0;

        return;
    }

    if(a == b)
    {
        common_int = a;

        return;
    }

    int max = a;
    int min = a;

    if(a > b)
    {
        min = b;
    }
    else
    {
        min = a;
    }

    int c = max % min;

    while(c != 0)
    {
        max = min;

        min = c;

        c = max % min;
    }

    common_int = min;
}

void CameraMaskWidget::cut_Draw_Background_By_Common_Int()
{
    int x_min, x_max, y_min, y_max;

    get_Draw_Background_BBox(x_min, x_max, y_min, y_max);

    int max_common_int_x = x_max - x_min;
    int max_common_int_y = y_max - y_min;

    int current_len = 0;

    for(int i = 0; i < mask_episode_list.size(); ++i)
    {
        for(int j = 0; j < mask_episode_list[i].size(); ++j)
        {
            int current_x = mask_episode_list[i][j].x();
            int current_y = mask_episode_list[i][j].y();

            for(int k = 0; k < mask_episode_list.size(); ++k)
            {
                for(int l = 0; l < mask_episode_list[k].size(); ++l)
                {
                    if(i == k && j == l)
                    {
                        continue;
                    }

                    int search_x = mask_episode_list[k][l].x();
                    int search_y = mask_episode_list[k][l].y();

                    if(current_x != search_x)
                    {
                        current_len = abs(current_x - search_x);
                        get_Common_Int(current_len, max_common_int_x, max_common_int_x);
                    }
                    if(current_y != search_y)
                    {
                        current_len = abs(current_y - search_y);
                        get_Common_Int(current_len, max_common_int_y, max_common_int_y);
                    }
                }
            }
        }
    }

    qDebug() << "max common int x = " << max_common_int_x << endl;
    qDebug() << "max common int y = " << max_common_int_y << endl;
}

void CameraMaskWidget::cut_Draw_Background_By_Fixed_Dist(int fixed_dist)
{
    fixed_sub_bbox_dist = fixed_dist;

    int x_min, x_max, y_min, y_max;

    get_Draw_Background_BBox(x_min, x_max, y_min, y_max);

    int x_bbox_num = (x_max - x_min) / fixed_sub_bbox_dist;
    int y_bbox_num = (y_max - y_min) / fixed_sub_bbox_dist;

    if((x_max - x_min) % fixed_sub_bbox_dist != 0)
    {
        ++x_bbox_num;
    }

    if((y_max - y_min) % fixed_sub_bbox_dist != 0)
    {
        ++y_bbox_num;
    }

    start_bbox_point.setX(x_min - (fixed_sub_bbox_dist * x_bbox_num - (x_max - x_min)) / 2);
    start_bbox_point.setY(y_min - (fixed_sub_bbox_dist * y_bbox_num - (y_max - y_min)) / 2);

    bbox_matrix.resize(x_bbox_num, y_bbox_num);
    bbox_matrix.setZero();
}

void CameraMaskWidget::add_Detect_Position(int x, int y)
{
    QPoint position_point(x, y);

    position_point = trans_img_to_widget(position_point);

    MatrixXd position(3, 1);

    position(0, 0) = position_point.x();
    position(1, 0) = position_point.y();
    position(2, 0) = 1.0;

    position = trans_matrix * position;

    position_point = trans_widget_to_draw_img(position_point);

    bbox_matrix(int(position_point.x() / fixed_sub_bbox_dist), int(position_point.y() / fixed_sub_bbox_dist)) += 1;
}

void CameraMaskWidget::add_Detect_Position(QPoint point)
{
    add_Detect_Position(point.x(), point.y());
}
