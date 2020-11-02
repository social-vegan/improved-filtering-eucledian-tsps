#include <bits/stdc++.h> 
#define point pair<double, double>
#define edge pair<int, int>
#define ll long long int 
using namespace std;
ll num_global=0,d[50000][2];

struct Point { 
	ll x,y,num=num_global; 
	bool operator<(Point p) 
	{ 
		return x<p.x || (x==p.x && y<p.y); 
	} 
}; 
 
ll cp(Point c,Point a,Point b) 
{ 
	return (a.x-c.x)*(b.y-c.y)-(a.y-c.y)*(b.x-c.x); 
} 

vector<Point> convex_hull(vector<Point> a) 
{ 
	int  k=0; 
	if (a.size()<=3) 
		return a; 
	vector<Point> ans(2*a.size()); 
	sort(a.begin(),a.end()); 
	for (int i=0;i<a.size();i++) 
	{ 
		while (k>=2 && cp(ans[k-2],ans[k-1],a[i])<=0) 
			k--; 
		ans[k++]=a[i];
	} 
	
	for (size_t i=a.size()-1,t=k+1;i>0;i--) 
	{ 
		while (k>=t && cp(ans[k-2],ans[k-1],a[i-1])<=0) 
			k--; 
		ans[k++]=a[i-1]; 
	} 

	ans.resize(k-1); 
	return ans; 
} 



int main() 
{ 
	int n;
	cout<<"Enter number of points: ";
    cin>>n;
    vector<Point> points;
	for(int i=0;i<n;i++)
	{
		cout<<"Enter x & y coordinates of Point "<<i+1<<": ";
		for(int j=0;j<2;j++)
			cin>>d[i][j];
		Point e;
		e.x=d[i][0];
		e.y=d[i][1];
		e.num=num_global++;
		points.push_back(e); 
		
	}
	 
	vector<Point> ans=convex_hull(points); 

	for (int i=0;i<ans.size();i++) 
	{	cout <<ans[i].x<<", "<< ans[i].y<<endl; 
		cout<<ans[i].num<<endl;
	}		

	return 0; 
} 

