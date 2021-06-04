#include "MaskWidget.h"

MaskWidget::MaskWidget(QWidget *parent)
{
    p1 = QPoint(0,0);
    p2 = QPoint(0,0);

    background_scale = 1.0;

    remove_mode = false;
    polygon_start = false;
    mask_changed = false;
}

MaskWidget::~MaskWidget()
{

}

void MaskWidget::set_background(QImage img)
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

QPoint MaskWidget::trans_widget_to_img(QPoint &widget_point)
{
    QPoint trans_point = widget_point;

    trans_point -= start_point;
    trans_point.setX(int(trans_point.x() / background_scale));
    trans_point.setY(int(trans_point.y() / background_scale));

    return trans_point;
}

void MaskWidget::paintEvent(QPaintEvent *event)
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
            for(int i = 0; i < mask_episode_list.size() - 1; ++i)
            {
                QPolygon poly;

                for(int j = 0; j < mask_episode_list[i].size() - 1; ++j)
                {
                    poly.append(mask_episode_list[i][j]);
                }

                painter.setBrush(Qt::black);
                painter.drawConvexPolygon(poly);
            }
        }

        if(mask_episode_list.size() > 0 && !polygon_start)
        {
            QPolygon poly;

            for(int j = 0; j < mask_episode_list[mask_episode_list.size() - 1].size() - 1; ++j)
            {
                poly.append(mask_episode_list[mask_episode_list.size() - 1][j]);
            }

            painter.setBrush(Qt::black);
            painter.drawConvexPolygon(poly);
        }
    }
}

void MaskWidget::mousePressEvent(QMouseEvent *event)
{
    if(show_mask)
    {
        if(event->buttons() == Qt::LeftButton)
        {
            if(!polygon_start)
            {
                p1 = event->pos();

                QPoint trans_point = trans_widget_to_img(p1);

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

        update();
    }
}

void MaskWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(show_mask)
    {
        if(event->buttons() == Qt::LeftButton)
        {
            p2 = event->pos();

            QPoint current_start_point = mask_episode_list[mask_episode_list.size() - 1][0];

            if((p2 - current_start_point).manhattanLength() < 10)
            {
                p2 = current_start_point;
            }
        }

        update();
    }
}

void MaskWidget::mouseReleaseEvent(QMouseEvent *event)
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
            QPoint trans_point = trans_widget_to_img(p2);

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
        }

        update();
    }
}

void MaskWidget::keyPressEvent(QKeyEvent *event)
{

}
