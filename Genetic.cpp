#include <bits/stdc++.h> 
#define point pair<double, double>
#define edge pair<int, int>
#define ll long long int 
#define POPULATION_SIZE 100 
using namespace std;
int dna[50000],vis[50000]={0}; 
float d[50000][2];
int n;

int randnum(int a, int b) 
{ 
    int r1 = (b-a)+1; 
    int r = a+(rand()%r1); 
    return r; 
} 

int mutated_dna() 
{  
	int r = randnum(0,n-1); 
    for(int i=0;i<n;i++)
	{if(r-i>=0 && vis[r-i]==0)
     {vis[r-i]++;return dna[r-i];}
	 else if(r+i<n && vis[r+i]==0)
	 {vis[r+i]++;return dna[r+i];}
	}
	return -1;
} 
  
vector<int> create_gnome() 
{ 
    for(int i=0;i<n;i++)
    vis[i]=0;
	vector<int> gnome; 
    for(int i=0;i<n;i++) 
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
    for(int i=0;i<n;i++) 
    { 
        float p = randnum(0, 100)/(100.0); 
        if(p < 0.45 && vis[chromosome[i]-1]==0) 
           {child_chromosome.push_back(chromosome[i]);vis[chromosome[i]-1]++;} 
  
        else if(p>=0.45 && p < 0.90 && vis[par2.chromosome[i]-1]==0) 
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
	vector<int> ans; 

	for(int i=0;i<n;i++)
	dna[i]=i+1; 
	
	for(int i=0;i<n;i++)
	{
		cout<<"Enter x & y coordinates of Point "<<i+1<<": ";
		for(int j=0;j<2;j++)
		cin>>d[i][j];
	}
	 
    long long flag=0;
    vector<Individual> population; 
    
    for(int i=0;i<POPULATION_SIZE;i++) 
    { 
        vector<int> gnome = create_gnome(); 
        population.push_back(Individual(gnome));
    } 
  
    while(flag++<1000) 
    { 
        sort(population.begin(),population.end(),sortBylength); 
  		vector<Individual> new_generation; 
  		ans=population[POPULATION_SIZE-1].chromosome;
  		max_val=population[POPULATION_SIZE-1].length;
        int s = (10*POPULATION_SIZE)/100; 
        for(int i= 0;i<s;i++) 
        	new_generation.push_back(population[i]); 
  		s = (90*POPULATION_SIZE)/100; 
        for(int i = 0;i<s;i++) 
        { 
            int len = population.size(); 
            int r = randnum(0, 50); 
            Individual parent1 = population[r]; 
            r = randnum(0, 50); 
            Individual parent2 = population[r]; 
            Individual offspring = parent1.mate(parent2); 
            if(offspring.length<max_val)
            {
            	max_val=offspring.length;
            	ans=offspring.chromosome;
			}
            new_generation.push_back(offspring);  
        } 
        population = new_generation; 
     } 
     cout<<"Optimal Length: "<<max_val<<"\nOptimal Path: ";
    for(int i=0;i<n;i++)
	{
	cout<<ans[i]<<" ";}
	cout<<endl;
} 

