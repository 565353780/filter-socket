#ifndef QUERYBOX_H
#define QUERYBOX_H
#include <QRect>
#include "Core/Math/MathDefines.h"
using namespace GCL;


class QueryBox
{
public:   
    QueryBox(int w = 100, int h = 100, int d = 100);


    void layOnVertex(Vec3i v, int k );
    bool operator < (const QueryBox &other) const;
    bool equatSize(const QueryBox &other) const;
    QueryBox &operator = (const QueryBox &other);

    void rotate_Z90();

    int maxLen() const;

    int w() const {return size_[0]; }
    int h() const {return size_[1]; }
    int d() const {return size_[2]; }

    int x() const {return pos_[0];}
    int y() const {return pos_[1];}
    int z() const {return pos_[2];}

    int top() const {return pos_[2] + size_[2];}
    Vec3i center() const;
    QRect getRect() const;
    bool intersects(const QRect &rect) const;
    int inteserctedArea(const QRect &rect) const;
    Vec3i getVertex(int k) const;
    int id_;
    Vec3i size_;
    Vec3i pos_;
    Vec4 color_{0.6,0.6,0.6,1.0};
    int same_depth_count_;
    int rotate_flag_;
    Scalar layon_rate_;
};
bool compare_orderbox(const QueryBox &b0, const QueryBox &b1);
#endif // QUERYBOX_H
