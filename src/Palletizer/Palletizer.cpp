#include "Palletizer.h"
#include <algorithm>
#include <map>
#include <QJsonArray>
#include <QJsonDocument>
#include <random>
#include <ctime>
int Palletizer::CandidateVertex::s_sort_type_ = Palletizer::LRTD;
Palletizer::Palletizer():online_num_(0),max_num_test_(10)
{

}

bool Palletizer::CandidateVertex::operator <(const Palletizer::CandidateVertex &other) const
{
    if(Palletizer::LRTD ==  s_sort_type_)
    {
        if(this->v_[2] != other.v_[2])
        {
            return this->v_[2] < other.v_[2]; // Lower one
        }

        return this->v_ < other.v_;   // left top

    }
    else if(Palletizer::OutsideIn == s_sort_type_)
    {
        if(this->v_[2] != other.v_[2])
        {
            return this->v_[2] < other.v_[2];
        }


        int isum = this->v_.maxabsvalue();
        int osum = other.v_.maxabsvalue();
        if(isum != osum)
        {
            return isum > osum; // outside one
        }
        return this->Quadrant() < other.Quadrant();
    }
}

bool Palletizer::CandidateVertex::operator ==(const Palletizer::CandidateVertex &other) const
{
    return v_ == other.v_;
}

int Palletizer::CandidateVertex::Quadrant() const
{
    if(this->v_[0] <= 0 && this->v_[1] <= 0)  return 0;
    if(this->v_[0] > 0 && this->v_[1] <= 0) return 1;
    if(this->v_[0] > 0 && this->v_[1] > 0) return 2;
    if(this->v_[0] <=0 && this->v_[1] > 0) return 3;
    return 0;
}

void Palletizer::init(int w, int h)
{
    this->platform_width_ = w;
    this->platform_height_ = h;
    query_list_.clear();
    order_list_.clear();
    online_list_.clear();
    candidate_vertices_.clear();

}

void Palletizer::loadQueryList(const std::vector<QueryBox> &qlist, bool reId)
{
    query_list_ = qlist;
    if(reId)
    {
        for(int i=0; i < query_list_.size(); i++)
        {
            query_list_[i].id_ = i;
        }
    }

}

void Palletizer::addQueryBox(QueryBox qbox)
{
    online_list_.push_back(qbox);
}

int Palletizer::queryBoxOnline(bool start_direct)
{

    if(start_direct || online_list_.size() >= online_num_ || order_list_.size() >= 2)
    {
        if(!online_list_.empty())
        {
            tryOnceOnLine(online_list_);
        }
    }
    if(order_list_.size() <= 1)
    {
        return -1;
    }
    else
    {
        return order_list_.back().id_;
    }
}

void Palletizer::initOrder()
{
    if(online_num_ == 0) // 离线问题
    {
        std::map<int,int> depth_count;
        for(auto & i : query_list_)
        {
            int d = i.d();
            if(!depth_count.count(d))
            {
                depth_count[d] = 0;
            }
            depth_count[d] += i.w() * i.h();

        }


        for(auto & i : query_list_)
        {
            i.same_depth_count_ = depth_count[i.d()];
            if(i.w() < i.h())
            {
                i.rotate_Z90();
            }
        }
        std::sort(query_list_.begin(),query_list_.end());
    }
    else
    {
        std::random_device rd;  // 将用于为随机数引擎获得种子
        std::mt19937 gen(rd()); // 以播种标准 mersenne_twister_engine
        std::uniform_int_distribution<> dis(1, query_list_.size());
        for(int i=0; i < query_list_.size(); i++)
        {
            int j =  (i + dis(gen)) % query_list_.size();
            if(i == j) continue;
            std::swap(query_list_[i],query_list_[j]);
        }
    }

}
bool compare_res_order(const std::shared_ptr<Palletizer::ResOrder> &b0, const std::shared_ptr<Palletizer::ResOrder>  &b1)
{
    return b0->score_ < b1->score_;
}
void Palletizer::solveOffLine()
{
    clock_t t0 = clock();



    std::vector< std::shared_ptr<ResOrder> > ans_list;

    int inherit_size = 10;
    int vary_size_1 = 15;
    int vary_size_2 = 5;
    int generation_num = 30;  // 遗传代数

    // 1: 构造初始解
    ans_list.push_back(std::make_shared<ResOrder>());
    for(int i=0; i < query_list_.size(); i++)
    {
        ans_list.back()->order_.emplace_back(i,0);
    }
    ans_list.push_back(std::make_shared<ResOrder>());
    for(int i=0; i < query_list_.size(); i++)
    {
        ans_list.back()->order_.emplace_back(i,1);
    }
    ans_list.push_back(std::make_shared<ResOrder>());
    for(int i=0; i < query_list_.size(); i++)
    {
        ans_list.back()->order_.emplace_back(i,(i > query_list_.size() / 2));
    }
    ans_list.push_back(std::make_shared<ResOrder>());
    for(int i=0; i < query_list_.size(); i++)
    {
        ans_list.back()->order_.emplace_back(i,(i < query_list_.size() / 2));
    }


    // 针对同类箱子较多的问题的初始化方法
    std::map<Vec3i,Vec4i> box_counter; // Vec4i : (类别ID, 数量, 同层填充个数，同层填充朝向)
    for(auto & i : this->query_list_)
    {
        Vec3i s = i.size_;
        if(box_counter.count(s))
        {
            box_counter[s][1]++;
        }
        else
        {
            Vec4i ct;
            ct[0] = box_counter.size(); // 类别ID
            ct[1] = 1; //类别数量

            int w0 = (int)(platform_width_ + s[0] * 0.2) / s[0];
            int h0 = (int)(platform_height_ + s[1] * 0.2) / s[1];

            int num0 = w0 * h0;

            int w1 =(int)(platform_width_ + s[1] * 0.2) / s[1];
            int h1 =(int)(platform_height_ + s[0] * 0.2) / s[0];
            int num1 = w1 * h1;

            if(num0 >= num1)
            {
                ct[2] = num0;
                ct[3] = 0;
            }
            else
            {
               ct[2] = num1;
               ct[3] = 1;
            }
            box_counter[s] = ct;
        }
    }
    std::vector<Vec3i> cate_list;

    while(cate_list.size() < query_list_.size())
    {
        bool flag = false;
        for(auto & iter : box_counter)
        {
            Vec4i& info = iter.second;
            if(info[1] > info[2])
            {
                for(int k=0; k < info[2]; k++)
                {
                    cate_list.push_back(iter.first);
                }
                info[1] -= info[2];
                flag = true;
                break;
            }
        }
        if(!flag)
        {
            int max_num = 0;
            Vec3i max_cate;
            for(auto & iter : box_counter)
            {
                Vec4i& info = iter.second;
                if(info[1] > 0)
                {
                    if(info[1] > max_num)
                    {
                        max_num = info[1];
                        max_cate = iter.first;
                    }
                }
            }
            if(max_num > 0)
            {
                for(int k=0; k < max_num; k++)
                {
                    cate_list.push_back(max_cate);
                }
                box_counter[max_cate][1] = 0;
                flag = true;
            }

        }
        if(!flag)
        {
            break;
        }

    }


    std::map<Vec3i, int> cate_index;
    std::vector<Vec2i> cate_order;
    for(auto si : cate_list)
    {
         int start_id = 0;
        if(cate_index.count(si))
        {
            start_id = cate_index[si];
        }
        for(int j=start_id; j < query_list_.size(); j++)
        {
           if(query_list_[j].size_ == si)
           {
               cate_order.emplace_back(j,box_counter[si][3]);
               cate_index[si] = j+1;
               break;
           }
        }

    }
    ans_list.push_back(std::make_shared<ResOrder>());
    ans_list.back()->order_ = cate_order;

    std::cout<<ans_list.back()->order_.size()<<" "<<query_list_.size()<<std::endl;



    this->geneSearch(ans_list);


    for(int ti=ans_list.size(); ti < inherit_size; ti++)
    {
        ans_list.push_back(std::make_shared<ResOrder>());
        for(int i=0; i < query_list_.size(); i++)
        {
            ans_list.back()->order_.emplace_back(i,std::rand() % 2);
        }
    }





    for(int i=0; i < inherit_size && i <ans_list.size(); i++)
    {
      std::cout<<i<<":"; ans_list[i]->score_.print();
    }
    std::cout<<std::endl;

    solveOrder(ans_list[0].get());

    std::sort(order_list_.begin(),order_list_.end(), compare_orderbox);



//    std::cout<<clock() - t0<<"ms"<<std::endl;
}

void Palletizer::solveOrder(Palletizer::ResOrder *res_order)
{
    order_list_.clear();
    candidate_vertices_.clear();
    QueryBox platform(platform_width_,platform_height_,5);
    platform.pos_ = Vec3i((int)-platform_width_/2,(int)-platform_height_/2,-5);
    order_list_.push_back(platform);
    addCandidate(platform);

    std::vector<QueryBox> query_list;
    for(auto tt : res_order->order_)
    {
         QueryBox qbox = query_list_[tt[0]];
        if(tt[1]){
            qbox.rotate_Z90();
        }
        query_list.push_back(qbox);
    }
    rate_level_ = 0.8;
    std::vector<QueryBox> failed_list;
    this->tryOnce(query_list,failed_list);
    while(!failed_list.empty() && rate_level_ > 0.6)
    {
        rate_level_ *= 0.8;
        std::vector<QueryBox> f2_list;
        this->tryOnce(failed_list,f2_list);
        failed_list = f2_list;
    }


    int maxtop = 0;
    Scalar min_rate = 1.0;

    for(int i= this->order_list_.size()-1; i >=0; i--)
    {
        if(this->order_list_[i].top() > maxtop)
        {
            maxtop = this->order_list_[i].top();
        }
        if(this->order_list_[i].layon_rate_ < min_rate)
        {
            min_rate = this->order_list_[i].layon_rate_;
        }
    }
    res_order->score_[0] = maxtop;
    res_order->score_[1] = (min_rate < 0.8);
    res_order->score_[2] = getAreaAtTop(this->order_list_[0].getRect(),maxtop) ;
    res_order->score_[2] /= 100;
    res_order->score_[2] += int(50 -int(min_rate * 5) * 10);
    std::map<Vec3i,int> v_count;

    for(auto & i : order_list_)
    {
        for(int k=0; k < 8; k++)
        {
            Vec3i v = i.getVertex(k);
            if(v_count.count(v))
            {
                v_count[v] ++;
            }
            else
            {
                v_count[v] = 0;
            }
        }
    }
    int c_sum = 0;
    for(auto & iter : v_count)
    {
        c_sum += 4 - iter.second;
    }
    res_order->score_[2] += c_sum * 8;
    res_order->score_[3] = c_sum;
    res_order->is_computed_ = true;
}

void Palletizer::initOnline()
{
    order_list_.clear();
    candidate_vertices_.clear();
    QueryBox platform(platform_width_,platform_height_,0);
    platform.pos_ = Vec3i(-platform_width_/2,-platform_height_/2,0);
    order_list_.push_back(platform);
    addCandidate(platform);
}

void Palletizer::solveOnLine()
{
    clock_t t0 = clock();

    std::vector<QueryBox> online_list;
    initOnline();

    std::cout<<"Init Online"<<" "<<query_list_.size()<< std::endl;
    for(const auto & qbox : query_list_)
    {

        online_list.push_back(qbox);

        if(!online_list.empty())
        {
            tryOnceOnLine(online_list);
        }
    }

    for(int i=0; i< online_num_; i++)
    {
        tryOnceOnLine(online_list);
    }



}

bool Palletizer::tryOnceOnLine(std::vector<QueryBox> &qlist)
{
    if(online_num_ == 1)
    {
        if(qlist.empty()) return false;
        int titer = 1;
        rate_level_ = 0.95;
        while(titer--)
        {
            int mi = -1;
            Vec4 max_score;
            QueryBox ans_box;
            for(int i=0; i < qlist.size(); i++)
            {
                auto qbox = qlist[i];
                auto iter = candidate_vertices_.begin();
                int tcount = 0;
                for(;iter != candidate_vertices_.end(); iter++)
                {
                    int on_boundary = 0;
                    QRect r0(iter->v_[0]-1,iter->v_[1]-1,3,3);
                    QRect r1 = order_list_[0].getRect();
                    QRect r2 = r0.intersected(r1);
                    if(r2.width() < r0.width() || r2.height() < r0.height())
                    {
                        on_boundary =  1;
                    }

                    for(int ri=0; ri < 2; ri++)
                    {
                        if(tryLayOn(qbox, iter->v_))
                        {
                            Vec4 score = getLayOnScore(qbox);
                            score[0] = on_boundary;
                            if(mi < 0 || score > max_score)
                            {
                                mi = i;
                                max_score = score;
                                ans_box = qbox;
                            }
                            tcount++;
                        }
                        qbox.rotate_Z90();
                    }
                    if(tcount > max_num_test_)
                    {
                        break;
                    }

                }
            }
            if(mi < 0){
                rate_level_ *= 0.8;
                continue;
            }
            order_list_.push_back(ans_box);
            addCandidate(ans_box);
            std::swap(qlist[mi],qlist[qlist.size()-1]);
            qlist.pop_back();
            return true;
        }
    }
    else
    {
        int mi = -1;
        QueryBox ans_box;
        Vec4i ans_score;
        rate_level_ = 0.95;
        for(int i=0; i < qlist.size(); i++)
        {
            auto qbox = qlist[i];
            if(qbox.w() < qbox.h())
            {
                qbox.rotate_Z90();
            }
            auto iter = candidate_vertices_.begin();

            // 先找同类点
            for(;iter != candidate_vertices_.end(); iter++)
            {
                for(int ri=0; ri < 2; ri++)
                {
                    if(tryLayOn(qbox, iter->v_))
                    {
                        int islink = (candidate_vertices_.count(qbox.pos_) && candidate_vertices_.count(qbox.pos_ + Vec3i(0,qbox.h(),0)));
                        int z = qbox.d()>0? (int)(qbox.z()) / (qbox.d()) - islink:qbox.top();
                        Vec4i score(z,qbox.x(),qbox.y(),0);
                        if(mi < 0 || score < ans_score )
                        {
                            mi = i;
                            ans_box = qbox;
                            ans_score = score;
                        }

                    }
                    qbox.rotate_Z90();
                }
            }
        }
        if(mi < 0)
        {
            for(int i=0; i < qlist.size(); i++)
            {
                auto qbox = qlist[i];
                auto iter = candidate_vertices_.begin();

                // 同类无可行解时
                for(;iter != candidate_vertices_.end(); iter++)
                {
                    for(int ri=0; ri < 2; ri++)
                    {
                        if(tryLayOn(qbox, iter->v_))
                        {
                            if(mi < 0 || Vec3(qbox.z(),qbox.x(),qbox.y()) <Vec3(ans_box.z(),ans_box.x(),ans_box.y()) )
                            {
                                mi = i;
                                ans_box = qbox;
                            }
                        }
                        qbox.rotate_Z90();
                    }
                }
            }
        }

        while(mi < 0 && rate_level_ > 0.6)
        {
            rate_level_ *= 0.8;
            for(int i=0; i < qlist.size(); i++)
            {
                auto qbox = qlist[i];
                auto iter = candidate_vertices_.begin();

                // 同类无可行解时
                for(;iter != candidate_vertices_.end(); iter++)
                {
                    for(int ri=0; ri < 2; ri++)
                    {
                        if(tryLayOn(qbox, iter->v_))
                        {
                            if(mi < 0 || Vec3(qbox.z(),qbox.y(),qbox.x()) <Vec3(ans_box.z(),ans_box.y(),ans_box.x()) )
                            {
                                mi = i;
                                ans_box = qbox;
                            }
                        }
                        qbox.rotate_Z90();
                    }
                }
            }
        }


        rate_level_ = 0.95;
        if(mi < 0)
        {
            return false;
        }


            order_list_.push_back(ans_box);
            addCandidate(ans_box);
            std::swap(qlist[mi],qlist[qlist.size()-1]);
            qlist.pop_back();
            return true;

    }
    return false;
}

QByteArray Palletizer::toJson() const
{
    QJsonArray array;
    for(auto qbox : order_list_)
    {
        QJsonArray info;
        info.push_back(qbox.size_[0]);
        info.push_back(qbox.size_[1]);
        info.push_back(qbox.size_[2]);
        info.push_back(qbox.center()[0]);
        info.push_back(qbox.center()[1]);
        info.push_back(qbox.center()[2]);
        array.push_back(info);
    }

    QJsonDocument doc;
    doc.setArray(array);
    return doc.toJson(QJsonDocument::Compact);
}

void Palletizer::addCandidate(const QueryBox &qbox)
{
    for(int i=0; i < 8; i++)
    {
        CandidateVertex cv(qbox.getVertex(i));
        cv.size_ = qbox.size_;
        if(!candidate_vertices_.count(cv))
        {
            candidate_vertices_.insert(cv);
        }
        else
        {
            candidate_vertices_.erase(cv);
            candidate_vertices_.insert(cv);
        }
    }
}

void Palletizer::tryOnce(const std::vector<QueryBox> &qlist, std::vector<QueryBox> &failed_list)
{
    for(auto qbox : qlist)
    {
         auto iter = candidate_vertices_.begin();
        CandidateVertex candidate_v;
        for(;iter != candidate_vertices_.end(); iter++)
        {
             candidate_v = *iter;
             if(tryLayOn(qbox,iter->v_))
             {
                 if(qbox.d() == 0)
                 {
                     std::cout<<"error:qbox.d() == 0"<<std::endl;
                 }
                 order_list_.push_back(qbox);
                 this->addCandidate(qbox);
                 break;
             }
             if(this->online_num_ > 0)
             {
                 qbox.rotate_Z90();
                 if(tryLayOn(qbox,iter->v_))
                 {
                     order_list_.push_back(qbox);
                     this->addCandidate(qbox);
                     break;
                 }
                 qbox.rotate_Z90();
             }
        }
        if(iter == candidate_vertices_.end())
        {
            failed_list.push_back(qbox);
        }
        else
        {
            candidate_vertices_.erase(candidate_vertices_.find(candidate_v));
        }
    }
}

bool Palletizer::tryLayOn(QueryBox &qbox, Vec3i v)
{
    for(int ti=0; ti < 1; ti++)
    {
        qbox.layOnVertex(v,ti);
        QRect rect(qbox.x(),qbox.y(),qbox.w(),qbox.h());
        int top = getMaxTop(rect);
        if(top < 0) continue;
        if(top > v[2]) continue;
        qbox.pos_[2] = top;
        int layOnarea = getAreaAtTop(rect,top);
        int totalArea = rect.width() * rect.height();
        Scalar rate = layOnarea / (Scalar)(totalArea);
        if(rate > rate_level_)
        {
            qbox.layon_rate_ = rate;
            return true;
        }
    }



    return false;
}

int Palletizer::getMaxTop(QRect rect)
{
    int t = -1;
    for(int i=order_list_.size()-1; i>=0; i--)
    {
        QueryBox &qbox =  order_list_[i];
        if(qbox.top() > t  && qbox.intersects(rect))
        {
            t = qbox.top();
        }
    }
    return t;
}

int Palletizer::getAreaAtTop(QRect rect, int t)
{
    int sum = 0;
    for(const auto & qbox : order_list_)
    {
         if(qbox.top() == t && qbox.intersects(rect))
        {
            sum +=  qbox.inteserctedArea(rect);

        }
    }
    return sum;
}

Vec4 Palletizer::getLayOnScore(const QueryBox &qbox)
{

    QRect rect1(qbox.x()-2,qbox.y()-2,qbox.w()+4,qbox.h()+4);
    Scalar max_rate = 0.0;
    int q_area = qbox.w() * qbox.h();
    int area_sum = q_area;
    for(const auto &obox : order_list_)
    {
        if(obox.intersects(rect1))
        {
            QRect rect = obox.getRect();
            if(obox.top() == qbox.top())
            {
                int area = rect.width() * rect.height() + q_area;
                QRect united_rect = rect.united(qbox.getRect());
                int united_area = united_rect.width() * united_rect.height();
                Scalar rate = area / (Scalar)(united_area);
                if(rate >= max_rate)
                {
                    if(rate > max_rate+TOLERANCE)
                    {
                        max_rate = rate;
                    }
                }
                area_sum += rect.width() * rect.height();
            }
            else if(obox.top() > 0)
            {
                area_sum -=rect.width() * rect.height();
            }
        }
    }
    return Vec4(0,-qbox.top(),area_sum,max_rate);
}

int Palletizer::getOnlineNum() const
{
    return online_num_;
}

int Palletizer::getPlatformHeight() const
{
    return platform_height_;
}

int Palletizer::getPlatformWidth() const
{
    return platform_width_;
}

void Palletizer::setOnlineNum(int online_num)
{
    online_num_ = online_num;
}

void Palletizer::setMaxTestNumAtOnce(int t)
{
    max_num_test_ = t;
}

QueryBox Palletizer::popQueryBox()
{
    if(query_list_.empty()) return QueryBox(-1,-1,-1);
    QueryBox qb = query_list_.back();
    query_list_.pop_back();
    return qb;
}

void Palletizer::geneSearch(std::vector<std::shared_ptr<Palletizer::ResOrder> > &ans_list, int vary_pos, int generation_num, int inherit_num)
{
    int vary_size_1 = 15;
    int vary_size_2 = 5;
    // 开始遗传检索
    for(int gi=0; gi < generation_num; gi++)
    {
        for(auto & qi : ans_list)
        {
            if(!qi->is_computed_)
                solveOrder(qi.get());
        }
        // 排序

        std::sort(ans_list.begin(),ans_list.end(),compare_res_order);

        std::vector< std::shared_ptr<ResOrder> > next_generation_list;


        // 精英遗传
        for(int i=0; i < ans_list.size() && i < inherit_num; i++)
        {
            next_generation_list.push_back(ans_list[i]);
        }

        // 变异1: 次序变异
        for(int vi=0; vi < vary_size_1; vi++)
        {
            std::shared_ptr<ResOrder>  son(new ResOrder());
            int id = std::rand() % ans_list.size();
            son->order_ = ans_list[id]->order_;
            int try_num = 10;
            while(try_num--)
            {
                int ii = vary_pos + (std::rand() % (son->order_.size() - vary_pos));
                int jj = vary_pos + (std::rand() % (son->order_.size() - vary_pos));

                const auto &qbox0 = query_list_[son->order_[ii][0]];
                const auto &qbox1 = query_list_[son->order_[jj][0]];
                if(qbox0.equatSize(qbox1))
                {
                    continue;
                }
                std::swap(son->order_[ii],son->order_[jj]);
                next_generation_list.push_back(son);
                break;
            }

        }

        // 变异2:翻转变异
        for(int vi=0; vi < vary_size_2; vi++)
        {
            std::shared_ptr<ResOrder>  son(new ResOrder());
            int id = std::rand() % ans_list.size();
            son->order_ = ans_list[id]->order_;
            int ii = vary_pos + (std::rand() % (son->order_.size() - vary_pos));
            son->order_[ii][1] = !son->order_[ii][1];
            next_generation_list.push_back(son);
        }
        ans_list = next_generation_list;


    }
}

