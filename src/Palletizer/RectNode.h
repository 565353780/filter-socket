#ifndef RECTNODE_H
#define RECTNODE_H
#include <memory>
#include <vector>

#include <QRect>
class RectNode
{
public:
    RectNode(QRect r);
    virtual ~RectNode();

    int num_children() const;
    bool has_children() const;

    int getValArea(const QRect &rect, int val);
    int getMaxVal(const QRect &rect) const;
    int getMaxValTest(const QRect &rect, int &qcount) const;

    bool addValue(const QRect &rect, const int &val);
public:
    QRect rect_;
    int max_val_;

private:
    std::vector<std::shared_ptr<RectNode> > children_;
    RectNode *parent_;

};

#endif // RECTNODE_H
