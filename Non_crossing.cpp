#include <bits/stdc++.h>
#include<cmath>
using namespace std;
#define point pair<double, double>
#define edge pair<int, int>

class TSP
{
    //private:
public:
    int V;
    vector<point> points;
    set<point> initial_domain;

    TSP(int V, vector<point> points)
    {
        this->V = V;
        this->points = points;
        for (int i = 0; i < points.size(); i++)
            initial_domain.insert(points[i]);
    }

    // euclidean distance calculator
    double euclidean_d(point a, point b)
    {
        return sqrt(pow(a.first - b.first, 2) + pow(a.second, b.second));
    }

    // Starting from vertex zero
    /*void TSP_Solver()
    {
        set<point> cur_domain;
        vector<point> tsp;

        tsp.push_back(points[0]);
        initial_domain.erase(points[0]);
        cur_domain = initial_domain;

        int left_V = V - 1;

        while (left_V != 0)
        {
            // updating cur_domain by non-crossing constraints
            non_crossing(cur_domain, tsp);

            //  left_V = 0;
        }
    }*/

    double angle(point& p1,point & p2, point &p3)
    {
        double v1x=p1.first-p2.first;
        double v1y=p1.second-p2.second;
        double v2x=p3.first-p2.first;
        double v2y=p3.second-p2.second;
        double v1_mod = sqrt(pow(v1x,2)+pow(v1y,2));
        double v2_mod = sqrt(pow(v2x,2)+pow(v2y,2));

        double cs = (v1x*v2x +v1y*v2y)/(v1_mod*v2_mod);
        return acos(cs);
    }

    void angle_update(point i, set<point> &i_domain, point j,set<point> &j_domain)
    {
        vector<point> remove;
        double alpha_min = INT_MAX;

        for(auto pt:i_domain)
        {
            point idp = pt;
            alpha_min = min(alpha_min,angle(idp,j,i));
        }
        double beta_max = INT_MIN;
        for(auto pt:i_domain)
        {
            point idp = pt;
            beta_max =  max(beta_max,angle(j,i,idp));
        }

        for(auto pt:j_domain)
        {
            point jdp = pt;

            bool thm3 = false;
            bool thm4 = false;


            if(angle(jdp,j,i)<alpha_min)
            {
                thm3=true;

                if(angle(j,i,jdp)>beta_max)
                    thm4=true;
            }

            if(thm3 && thm4)
            {
                remove.push_back(jdp);
            }
        }

        for(auto pt:remove)
            j_domain.erase(pt);

        return;
    }

    void non_crossing(set<point> &cur_domain, vector<point> tsp)
    {
        if (tsp.size() <= 2)
            return;

        int n = tsp.size();

        long double max_slop = INT_MIN, min_slop = INT_MAX;
        point min_slop_point,max_slop_point;
        for (int i = 0; i < n - 1; i++)
        {
            point a = tsp[i];
            point b = tsp[n - 1];

            if (a.first == b.first)
            {
                max_slop = INT_MAX;
                max_slop_point = a;
            }
            else
            {
                long double slop = (a.second - b.second) / (a.first - b.first);

                if (slop > max_slop)
                {
                    max_slop = slop;
                    max_slop_point = a;
                }
                if (slop < min_slop)
                {
                    min_slop = slop;
                    min_slop_point = a;
                }
            }
        }
        cout<<"min - max "<<min_slop<<" "<<max_slop<<endl<<endl;
        vector<point> remove;
        for (auto pt : cur_domain)
        {
            point a = pt;
            point b = tsp[n - 1];

            long double slop;

            if (a.first == b.first)
                slop = INT_MAX;

            else
                slop = (a.second - b.second) / (a.first - b.first);

            if (!(min_slop <= slop && slop <= max_slop))
                continue;

            if(!(same_side(min_slop_point,b,min_slop_point,a)))
                continue;

            for (int i = 0; i < n - 1; i++)
            {
                point point1 = tsp[i];
                point point2 = tsp[i + 1];

                if (i < n - 2)
                {
                    if (Is_crossing(point1, point2, b, a))
                    {
                        remove.push_back(a);
                        break;
                    }
                }
                else
                {
                    if (Is_overlapping(tsp[n - 2], tsp[n - 1], b, a))
                    {
                        cout<<"olc for "<<a.first<<" "<<a.second<<endl;
                        remove.push_back(a);
                        break;
                    }

                }
            }
        }
        for(int i=0;i<remove.size();i++)
        {
            cur_domain.erase(remove[i]);
        }
        return;
    }

    bool same_side(point &p1, point &p2, point &p3, point &p4)
    {
        double x1 = p1.first;
        double y1 = p1.second;
        double x2 = p2.first;
        double y2 = p2.second;

        double x3 = p3.first;
        double y3 = p3.second;
        double x4 = p4.first;
        double y4 = p4.second;

        double A1 = -(y2 - y1) / (x2 - x1);
        double B1 = 1;
        double C1 = A1 * x1 + B1 * y1;

        long double p3_sign = A1*x3 + B1 * y3 - C1;
        long double p4_sign = A1*x4 + B1 * y4 - C1;

        return (p3_sign*p4_sign >=0 );
    }

    void show_points(point &p1, point &p2, point &p3, point &p4)
    {
        cout << '(' << p1.first << ',' << p1.second << "),(" << p2.first << ',' << p2.second << "),("
             << p3.first << ',' << p3.second << "),(" << p4.first << ',' << p4.second << ')';
        return;
    }

    bool Is_overlapping(point &p1, point &p2, point &p3, point &p4)
    {
        double x1 = p1.first;
        double y1 = p1.second;
        double x2 = p2.first;
        double y2 = p2.second;

        double x3 = p3.first;
        double y3 = p3.second;
        double x4 = p4.first;
        double y4 = p4.second;

        double A1 = -(y2 - y1) / (x2 - x1);
        double B1 = 1;
        double C1 = A1 * x1 + B1 * y1;

        double A2 = -(y4 - y3) / (x4 - x3);
        double B2 = 1;
        double C2 = A2 * x3 + B2 * y3;

        if (A1 != A2)
        {
            return false;
        }
        else
        {
            if (x1 == x2)
            {
                if ( ((min(y1, y2) <= y3 && y3 <= max(y1, y2)) && (min(y1, y2) <= y4 && y4 <= max(y1, y2))) || ((min(y3, y4) <= y1 && y1 <= max(y3, y4)) && (min(y3, y4) <= y2 && y2 <= max(y3, y4))))
                    return true;
            }
            else if (((min(x1, x2) <= x3 && x3 <= max(x1, x2)) && (min(x1, x2) <= x4 && x4 <= max(x1, x2))) || ((min(x3, x4) <= x1 && x1 <= max(x3, x4)) && (min(x3, x4) <= x2 && x2 <= max(x3, x4))))
                return true;
        }
        return false;
    }

    bool Is_crossing(point &p1, point &p2, point &p3, point &p4)
    {
        double x1 = p1.first;
        double y1 = p1.second;
        double x2 = p2.first;
        double y2 = p2.second;

        double x3 = p3.first;
        double y3 = p3.second;
        double x4 = p4.first;
        double y4 = p4.second;

        double a1 = x1 - x2; // a1x + b1y = c1
        double b1 = x4 - x3; // a2x + b2y = c2
        double c1 = x1 - x3;
        double a2 = y1 - y2;
        double b2 = y4 - y3;
        double c2 = y1 - y3;

        double determinant = a1 * b2 - a2 * b1;

        if (determinant != 0)
        {
            double val1 = (b2 * c1 - b1 * c2) / determinant;
            double val2 = (a1 * c2 - a2 * c1) / determinant;

            if (0 <= val1 && val1 <= 1 && 0 <= val2 && val2 <= 1)
            {
                return true;
            }
        }
        else
        {
            if (x1 == x2)
            {
                if ((min(y1, y2) <= y3 && y3 <= max(y1, y2)) || ((min(y1, y2) <= y4 && y4 <= max(y1, y2))))
                    return true;
                else
                    false;
            }
            else
            {
                //  y = m1*x + c1
                //  y = m2*x + c2
                //  A1*x + B1*y = C1

                double A1 = -(y2 - y1) / (x2 - x1);
                double B1 = 1;
                double C1 = A1 * x1 + B1 * y1;

                double A2 = -(y4 - y3) / (x4 - x3);
                double B2 = 1;
                double C2 = A2 * x3 + B2 * y3;

                if ((A1 * x3 + B1 * y3 - C1 == 0) && (min(x1, x2) <= x3 && x3 <= max(x1, x2)))
                {
                    return true;
                }
                else if ((A1 * x4 + B1 * y4 - C1 == 0) && (min(x1, x2) <= x4 && x4 <= max(x1, x2)))
                {
                    return true;
                }
                else if ((A2 * x1 + B2 * y1 - C2 == 0) && (min(x3, x4) <= x1 && x1 <= max(x3, x4)))
                {
                    return true;
                }
                else if ((A2 * x2 + B2 * y2 - C2 == 0) && (min(x3, x4) <= x2 && x2 <= max(x3, x4)))
                {
                    return true;
                }
            }
        }
        return false;
    }
};

int main()
{
    int V = 7;
    double x, y;
    vector<point> points(V);
    points = {{0, 0}, {3, 3}, {4, 5}, {6, 5}, {8, 10}, {10, 5}, {12, 5}}; // current tsp

    TSP tsp(V, points);

    set<point> i_domain,j_domain;
    i_domain.insert({2,1.5});
    i_domain.insert({4, 5});
    j_domain.insert({2, 1});
    j_domain.insert({5, 1});
    j_domain.insert({6, 1});
    j_domain.insert({5, 3});
    point i = {4,0};
    point j = {0,0};
    tsp.angle_update(i,i_domain, j, j_domain);

    cout<<"I dom"<<endl;
    for (auto pt : i_domain)
    {
        cout << pt.first << " " << pt.second << endl;
    }
    cout<<"j dom"<<endl;
    for (auto pt : j_domain)
    {
        cout << pt.first << " " << pt.second << endl;
    }

    return 0;
}

