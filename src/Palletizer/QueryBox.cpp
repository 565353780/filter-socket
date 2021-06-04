#include "QueryBox.h"

QueryBox::QueryBox(int w, int h, int d):size_(w,h,d),rotate_flag_(0),same_depth_count_(0),id_(-1),layon_rate_(1.0)
{

}

void QueryBox::layOnVertex(Vec3i v, int k)
{
    if(k == 0)
    {
        pos_ = v;
    }
    else if(k == 1)
    {
        pos_[0] = v[0];
        pos_[1] = v[1] - size_[1];
        pos_[2] = v[2];
    }
    else if(k == 2)
    {
        pos_[0] = v[0] - size_[0];
        pos_[1] = v[1] - size_[1];
        pos_[2] = v[2];
    }
    else if(k == 3)
    {
        pos_[0] = v[0] - size_[0];
        pos_[1] = v[1];
        pos_[2] = v[2];
    }
}

bool QueryBox::operator <(const QueryBox &other) const
{
    if(other.same_depth_count_ == this->same_depth_count_)
    {
        Vec2i ot(other.d(),other.maxLen());
        Vec2i it(this->d(),this->maxLen());
        return it > ot;
    }


   return this->same_depth_count_ > other.same_depth_count_;

}

bool QueryBox::equatSize(const QueryBox &other) const
{
    return this->w()==other.w() && this->h()==other.h() && this->d() == other.d();
}

QueryBox &QueryBox::operator =(const QueryBox &other)= default;

void QueryBox::rotate_Z90()
{
    std::swap(size_[0],size_[1]);
    rotate_flag_ = (rotate_flag_ + 1) % 2;
}

int QueryBox::maxLen() const
{
    return std::max(size_[0],size_[1]);
}

Vec3i QueryBox::center() const
{
    return Vec3i(x()+w()/2,y()+h()/2,z()+d()/2);
}

QRect QueryBox::getRect() const
{
    return {x(),y(),w(),h()};
}

bool QueryBox::intersects(const QRect &rect) const
{
    QRect tr(x(),y(),w(),h());
    return tr.intersects(rect);
}

int QueryBox::inteserctedArea(const QRect &rect) const
{
    QRect tr(x(),y(),w(),h());
    QRect ir = tr.intersected(rect);
    if(ir.isValid())
        return ir.width() * ir.height();

    return 0;


}

Vec3i QueryBox::getVertex(int k) const
{
    if(k < 0 || k >= 8) return Vec3i(999999);
    return Vec3i(x()+ w() * (k % 2), y() + h() * (k / 2 % 2), z()+d()*(k/4));
}

bool compare_orderbox(const QueryBox &b0, const QueryBox &b1)
{
    Vec3i v0(b0.z(),b0.x(),b0.y());
    Vec3i v1(b1.z(),b1.x(),b1.y());
    return v0 < v1;
}
