#include "RectNode.h"
#include <QDebug>

RectNode::RectNode(QRect r):rect_(r),max_val_(-1)
{

}

RectNode::~RectNode()
{

}

int RectNode::num_children() const
{
    return children_.size();
}

bool RectNode::has_children() const
{
    return children_.size() > 0;
}

int RectNode::getValArea(const QRect &rect, int val)
{
    if(!rect.intersects(this->rect_))
    {
        return 0;
    }

    if(this->max_val_ < val) return 0;

    if(!this->has_children())
    {
        if(this->max_val_ == val)
        {
            QRect ir = rect.intersected(this->rect_);
            return ir.width() * ir.height();
        }
    }
    else
    {
        int sum = 0;
        for(int i=0; i < this->children_.size(); i++)
        {
            sum+=this->children_[i]->getValArea(rect,val);
        }
        return sum;
    }

}

int RectNode::getMaxVal(const QRect &rect) const
{
    if(!rect.intersects(this->rect_))
    {
        return -1;
    }

    if(!this->has_children()) return this->max_val_;

    int max_val = -1;
    for(int i=0; i < this->children_.size(); i++)
    {
        int val = this->children_[i]->getMaxVal(rect);

        if(val > max_val) max_val = val;
    }
    return max_val;
}

int RectNode::getMaxValTest(const QRect &rect, int &qcount) const
{
    qcount++;
    if(!rect.intersects(this->rect_))
    {
        return -1;
    }

    if(!this->has_children()) return this->max_val_;

    int max_val = -1;
    for(int i=0; i < this->children_.size(); i++)
    {
        int val = this->children_[i]->getMaxValTest(rect,qcount);

        if(val > max_val) max_val = val;
    }
    return max_val;
}

bool RectNode::addValue(const QRect &rect, const int &val)
{
    if(rect.intersects(this->rect_))
    {
        if(val > this->max_val_)
        {
            this->max_val_ = val;
        }
        if(this->children_.size() > 0)
        {
            for(int i=0; i < this->children_.size(); i++)
            {
                this->children_[i]->addValue(rect,val);
            }

            return true;
        }
        else
        {
            if(rect == this->rect_)
            {
                return true;
            }
            else
            {
                int x = this->rect_.x();
                int y = this->rect_.y();
                int w = this->rect_.width();
                int h = this->rect_.height();

                if(w > 1 && h > 1)
                {

                    QRect r0(x,y,w / 2, h / 2);
                    if(r0.isValid())
                    {
                        children_.push_back(std::shared_ptr<RectNode>(new RectNode(QRect(r0))));
                    }

                    QRect r1(x+w/2,y,w - w / 2, h / 2);
                    if(r1.isValid())
                    {
                        children_.push_back(std::shared_ptr<RectNode>(new RectNode(QRect(r1))));
                    }

                    QRect r2(x+w/2,y+h/2,w - w / 2, h - h / 2);
                    if(r2.isValid())
                    {
                        children_.push_back(std::shared_ptr<RectNode>(new RectNode(QRect(r2))));
                    }

                    QRect r3(x,y+h/2, w / 2, h - h / 2);
                    if(r3.isValid())
                    {
                        children_.push_back(std::shared_ptr<RectNode>(new RectNode(QRect(r3))));
                    }
                }
                else if(w > 1)
                {
                    QRect r0(x,y,w / 2, h);
                    if(r0.isValid())
                    {
                        children_.push_back(std::shared_ptr<RectNode>(new RectNode(QRect(r0))));
                    }


                    QRect r1(x+w/2,y,w - w / 2, h);
                    if(r1.isValid())
                    {
                        children_.push_back(std::shared_ptr<RectNode>(new RectNode(QRect(r1))));
                    }

                }
                else if(h > 1)
                {
                    QRect r0(x,y,w, h/2);
                    if(r0.isValid())
                    {
                        children_.push_back(std::shared_ptr<RectNode>(new RectNode(QRect(r0))));
                    }


                    QRect r1(x,y+h/2,w, h-h/2);
                    if(r1.isValid())
                    {
                        children_.push_back(std::shared_ptr<RectNode>(new RectNode(QRect(r1))));
                    }
                }


                for(int i=0; i <children_.size(); i++)
                {
                    children_[i]->addValue(rect,val);
                }

            }
        }
    }
    else
    {
        return false;
    }
}
