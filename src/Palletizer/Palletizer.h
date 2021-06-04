#ifndef PALLETIZER_H
#define PALLETIZER_H
#include <vector>
#include "QueryBox.h"
#include "Core/Math/MathDefines.h"
#include <set>
#include <QString>
#include <memory>
#include <QList>
using namespace GCL;


class Palletizer
{
public:
    enum SortType{
        LRTD = 0, // left to right , top to down
        OutsideIn = 1 // outsize first
    };


    struct CandidateVertex
    {
        CandidateVertex()= default;
        ~CandidateVertex() = default;
        CandidateVertex(const Vec3i &v):v_(v){}
        CandidateVertex(const CandidateVertex &cv) = default;
        CandidateVertex(CandidateVertex &&cv) = default;
        CandidateVertex &operator = (const CandidateVertex &cv) = default;
        CandidateVertex &operator = (CandidateVertex &&cv) = default;

        CandidateVertex &operator = (const Vec3i &x){
            this->v_ = x;            
            return *this;
        }

        bool operator < (const CandidateVertex &other) const;
        bool operator == (const CandidateVertex &other) const;

        int Quadrant() const;
        Vec3i v_;
        Vec3i size_;

        static int s_sort_type_;
    };


    struct ResOrder
    {
        ResOrder()= default;
        std::vector<Vec2i> order_;
        Vec4 score_;

        bool is_computed_{};
    };
    Palletizer();


    void init(int w, int h);
    void loadQueryList(const std::vector<QueryBox> &qlist,bool reId = true);


    void addQueryBox(QueryBox qbox);
    int queryBoxOnline(bool start_direct = false );

    bool tryOnceOnLine(std::vector<QueryBox> &qlist);


    void initOrder();
    void solveOffLine();
    void solveOrder(ResOrder *res_order);

    void initOnline();
    void solveOnLine();

    QByteArray toJson() const;

    int getOnlineNum() const;
    void setOnlineNum(int online_num);
    void setMaxTestNumAtOnce(int t);

    std::vector<QueryBox> getOrderedList() const {return order_list_;}
    std::vector<QueryBox> getQueryList() const {return query_list_;}

    int n_ordered_list_size() const {return order_list_.size();}
    int n_query_list_size() const {return query_list_.size();}


    QueryBox getOrderedBoxAt(int i) const {return order_list_[i];}
    QueryBox getQueryBoxAt(int i) const {return query_list_[i];}

    QueryBox popQueryBox();


    int getPlatformWidth() const;

    int getPlatformHeight() const;


private:
    void geneSearch(std::vector< std::shared_ptr<ResOrder> > &ans_list, int vary_pos = 0, int generation_num = 30, int inherit_num = 10);

    void addCandidate(const QueryBox &qbox);

    void tryOnce(const std::vector<QueryBox> &qlist, std::vector<QueryBox> &failed_list);
    bool tryLayOn(QueryBox &qbox ,Vec3i v);

    int getMaxTop(QRect rect);
    int getAreaAtTop(QRect rect, int t);

    Vec4 getLayOnScore(const QueryBox &qbox);
private:
    int online_num_;
    int platform_width_;
    int platform_height_;
    int max_num_test_;

    Scalar rate_level_;
    std::vector<QueryBox> query_list_;
    std::vector<QueryBox> online_list_;



    std::vector<QueryBox> order_list_;
    std::set<CandidateVertex> candidate_vertices_;

};

#endif // PALLETIZER_H
