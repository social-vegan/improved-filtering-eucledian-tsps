#include <bits/stdc++.h> 
#define point pair<double, double>
#define edge pair<int, int>
#define POPULATION_SIZE 100 
using namespace std; 
typedef long long ll; 
int dom_len=0,dna[50000],vis[50000]={0},domain[50000]={0},p_hull[50000]={0};
int hull_order[50000],curr_hull=0,hull_len=0; 
float d[50000][2];
ll num_global=0,n,ini;

struct Point { 
	ll x,y,num=num_global; 
	bool operator<(Point p) 
	{ 
		return x<p.x || (x==p.x && y<p.y); 
	} 
}; 
 
ll cp(Point c, Point a, Point b) 
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
	for (int i=0;i<ans.size();i++) 
	{	//cout <<ans[i].x<<", "<< ans[i].y<<endl; 
		p_hull[ans[i].num]=1;
		hull_order[i]=ans[i].num;
		hull_len++;
	}	
	return ans; 
		

} 


int randnum(int a, int b) 
{ 
    int r1 = (b-a)+1; 
    int r = a+(rand()%r1); 
    return r; 
} 

int mutated_dna() 
{  
	int r = randnum(0,n);
	/*for(int i=0;i<n;i++)
	cout<<domain[i]<<" ";
	cout<<endl;*/
	//cout<<r<<endl;
	
    for(int i=0;i<=n;i++)
	{if(r-i>=0 && domain[r-i]==1)
     {	if(curr_hull<hull_len-1 && r-i==hull_order[curr_hull+1]) 
		{	//domain[curr_hull+1]=0;
			curr_hull++;
			if(curr_hull<hull_len-1)
			{	//cout<<r-i<<" r-i visited "<<curr_hull+1<<endl;
				domain[hull_order[curr_hull+1]]=1;
				dom_len++;
			}
		}
	 	domain[r-i]=0;dom_len--;return dna[r-i];
	 }
	 else if(r+i<n && domain[r+i]==1)
	 {  if(curr_hull<hull_len-1 && r+i==hull_order[curr_hull+1]) 
		{	//domain[curr_hull+1]=0;
			curr_hull++;
			if(curr_hull<hull_len-1)
			{	//cout<<r+i<<" r+i visited "<<curr_hull+1<<endl;
	 			domain[hull_order[curr_hull+1]]=1;
				dom_len++;
			}
		}
	 	domain[r+i]=0;dom_len--;return dna[r+i];
	 }
	}
	
	//return -1;
} 
  
vector<int> create_gnome() 
{ 
    for(int i=0;i<n;i++)
	domain[i]=0;
	curr_hull=0;
	dom_len=0;
	ini=hull_order[0];
	for(int i=0;i<n;i++)
	if(p_hull[i]!=1)
	{domain[i]=1;
	 dom_len++;}
	domain[hull_order[1]]=1;
	dom_len++;
	vector<int> gnome; 
    gnome.push_back(ini+1);
    for(int i=1;i<n;i++) 
    	gnome.push_back(mutated_dna()); 
    return gnome; 
} 
  
class Individual 
{ 
public: 
    vector<int> chromosome; 
    float length;
    Individual(vector<int> chromosome); 
    Individual mate(Individual parent2); 
    float cal_length(); 
}; 
  
Individual::Individual(vector<int> chromosome) 
{ 
    this->chromosome = chromosome; 
    length = cal_length();  
}
  
Individual Individual::mate(Individual par2) 
{ 
    for(int i=0;i<n;i++)
    vis[i]=0;
	vector<int> child_chromosome;
	child_chromosome.push_back(chromosome[0]);
    for(int i=1;i<n;i++) 
    { 
        float p = randnum(0, 100)/(100.0); 
        if(p < 0.45 && vis[chromosome[i]-1]==0) 
           {child_chromosome.push_back(chromosome[i]);vis[chromosome[i]-1]++;} 
  
        else if(p>=0.45 &&/* p < 0.90 &&*/ vis[par2.chromosome[i]-1]==0) 
            {child_chromosome.push_back(par2.chromosome[i]);vis[par2.chromosome[i]-1]++;} 
  
        else
            child_chromosome.push_back(mutated_dna()); 
    } 
    return Individual(child_chromosome); 
}
  
float Individual::cal_length() 
{ 
    float sum=0;
	for(int i=0;i<n;i++)
		sum+=sqrt(pow(d[chromosome[(i+1)%n]-1][0]-d[chromosome[i]-1][0],2)+pow(d[chromosome[(i+1)%n]-1][1]-d[chromosome[i]-1][1],2));			
	return sum;
}

bool sortBylength(const Individual &ind1, const Individual &ind2) 
{ 
    return (ind1.length < ind2.length); 
} 

    
int main() 
{ 
    srand((unsigned)(time(0))); 
    cout<<"Enter number of points: ";
    cin>>n;
	float max_val=0;
	vector<int> ans1; 

	for(int i=0;i<n;i++)
	dna[i]=i+1;
	 
	
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
	 
    long long flag=0;
    vector<Individual> population;
	vector<int> gnome = create_gnome(); 
    
    for(int i=0;i<POPULATION_SIZE;i++) 
    { 
        vector<int> gnome = create_gnome(); 
        population.push_back(Individual(gnome));
    } 
  
    while(flag++<1000) 
    { 
        sort(population.begin(),population.end(),sortBylength); 
  		vector<Individual> new_generation; 
  		ans1=population[POPULATION_SIZE-1].chromosome;
  		max_val=population[POPULATION_SIZE-1].length;
        int s = (10*POPULATION_SIZE)/100; 
        for(int i= 0;i<s;i++) 
        	new_generation.push_back(population[i]); 
  		s = (90*POPULATION_SIZE)/100; 
        for(int i = 0;i<s;i++) 
        { 
            int len=population.size(); 
            int r=randnum(0,50); 
            Individual parent1=population[r]; 
            r = randnum(0,50); 
            Individual parent2=population[r]; 
            Individual offspring=parent1.mate(parent2); 
            if(offspring.length<max_val)
            {
            	max_val=offspring.length;
            	ans1=offspring.chromosome;
			}
            new_generation.push_back(offspring);  
        } 
        population = new_generation; 
     } 
     cout<<"Optimal Length: "<<max_val<<"\nOptimal Path: ";
    for(int i=0;i<n;i++)
	{
	cout<<ans1[i]<<" ";}
	cout<<endl;
	/*for(int i=0;i<n;i++)
	{
	cout<<hull_order[i]+1<<" ";}*/
	cout<<endl;
} 

