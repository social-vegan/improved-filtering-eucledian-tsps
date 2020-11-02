#include <bits/stdc++.h>
using namespace std;

int n, startCity, l;                                 
double alpha = 0.5 , beta = 0.8 , Q = 80 , ro = 0.2;

class ACO {
    double BESTLENGTH;
	int *BESTROUTE;

public:
	int **ROUTES;
	double **CITIES, **PHEROMONES, **DELTAPHEROMONES, **PROBS;

	ACO (int numAnts, int numCities){
        l = numAnts;
        n 	= numCities;
    }
    
	void init ();
	void optimize (int iterations);
	void printRESULTS ();

private:
	double distance (int cityi, int cityj);
	bool vizited (int antk, int c);
	double PHI (int cityi, int cityj, int antk);	
	double length (int antk);	
	void route (int antk);
	int valid (int antk);	
	void updatePHEROMONES ();
};

void ACO::init () {
	CITIES 			= new double*[n];                      // coordinates of n cities (n x 2)
	PHEROMONES 		= new double*[n];                      // pheromone level on each path (i,j) (n x n)
	DELTAPHEROMONES = new double*[n];                      // change in pheromone level on each path (i,j) (n x n)
	PROBS 			= new double*[n-1];                    // probability of choosing the path (i,j) for all j except j==i (n-1 x 2)

	for(int i=0; i<n; i++) {
		CITIES[i] 			= new double[2];
		PHEROMONES[i] 		= new double[n];
		DELTAPHEROMONES[i] 	= new double[n];
		PROBS[i] 			= new double[2];

		for (int j=0; j<2; j++) {
			CITIES[i][j] = -1.0;
			PROBS[i][j]  = -1.0;
		}

		for (int j=0; j<n; j++) {
			PHEROMONES[i][j] 		= 0.0;
			DELTAPHEROMONES[i][j] 	= 0.0;
		}
	}	

	ROUTES = new int*[l];                        // path followed by each ant (l x n)
	for (int i=0; i<l; i++) {
		ROUTES[i] = new int[n];
		for (int j=0; j<n; j++) {
			ROUTES[i][j] = -1;
		}
	}
	
	BESTLENGTH = (double) INT_MAX;               // best route length
	BESTROUTE  = new int[n];                     // best route coordinates
	for (int i=0; i<n; i++) {
		BESTROUTE[i] = -1;	
	}
}


// if the pheromone level on every path is same, choose the nearest city
// prob. of travelling from city i to city j for ant k is P(i,j,k)
// pheromone(i,j) = intensity of pheromone trail on path i,j
// alpha = influence of pheromone level [0,1]
// eta(i,j) = visibility of city i from city j = 1 / d(i,j)
// d(i,j) = distance between city i and j
// beta = influence of visibility of a city
// P(i,j,k) = [(pheromone(i,j)^alpha) * (eta(i,j)^beta)] / sum[(pheromone(i,s)^alpha) * (eta(i,s)^beta)] if city j is not visited by ant k
void ACO::optimize(int iterations){
    for (int iter=1; iter<=iterations; iter++) {
		for (int antk=0; antk<l; antk++) {
            // repeat while path is not completed by antk
			while (valid(antk) != 0) {
				for (int i=0; i<n; i++) {
					ROUTES[antk][i] = -1;	
				}
				route(antk);
			}

            // update best route
			double rlength = length(antk);

			if (rlength < BESTLENGTH) {
				BESTLENGTH = rlength;
				for (int i=0; i<n; i++) {
					BESTROUTE[i] = ROUTES[antk][i];
				}
			}				
		}		

        
        // UpdateTrails
		updatePHEROMONES ();
		
        // reset all routes to -1
		for (int i=0; i<l; i++) {
			for (int j=0; j<n; j++) {
				ROUTES[i][j] = -1;
			}
		}
	}
}

void ACO::printRESULTS () {
	BESTLENGTH += distance (BESTROUTE[n-1], startCity);
	cout << " BEST ROUTE:" << endl;
	for (int i=0; i<n; i++) {
		cout << BESTROUTE[i] << " ";
	}
	cout << endl << "length: " << BESTLENGTH << endl;
}

double ACO::distance (int cityi, int cityj){
    return (double) sqrt (pow (CITIES[cityi][0] - CITIES[cityj][0], 2) 
                        + pow (CITIES[cityi][1] - CITIES[cityj][1], 2));
}

bool ACO::vizited (int antk, int c){
    for (int i=0; i<n; i++) {
        if (ROUTES[antk][i] == -1) {
            break;
        }
        if (ROUTES[antk][i] == c) {
            return true;
        }
    }
    return false;
}
double ACO::PHI (int cityi, int cityj, int antk){
    double ETAij = (double) pow (1 / distance (cityi, cityj), beta);
    double TAUij = (double) pow (PHEROMONES[cityi][cityj], alpha);

    double sum = 0.0;
    for (int c=0; c<n; c++) {
        if (!vizited(antk, c)) {
            double ETA = (double) pow (1 / distance (cityi, c), beta);
            double TAU = (double) pow (PHEROMONES[cityi][c], alpha);
            sum += ETA * TAU;
        }	
    }
    return (ETAij * TAUij) / sum;
}

double ACO::length (int antk){
    double sum = 0.0;
    for (int j=0; j<n-1; j++) {
        sum += distance (ROUTES[antk][j], ROUTES[antk][j+1]);	
    }
    return sum;
}

void ACO::route (int antk){                   
    ROUTES[antk][0] = startCity;

    for (int i=0; i<n-1; i++) {		
        int cityi = ROUTES[antk][i];
        int count = 0;
        
        // select city to be visited next
        int next_city = -1;

        for (int c=0; c<n; c++) {
            if (c == cityi) {
                continue;	
            }
            if (!vizited (antk, c)) {
                PROBS[c][0] = PHI (cityi, c, antk);
                PROBS[c][1] = (double) c;
                count++;

                if (next_city == -1){
                    next_city = c;
                }
                else if (PROBS[next_city][0] < PROBS[c][0]){
                    next_city = c;
                }
            }
        }
        
        // deadlock
        if (count == 0) {
            return;
        }
        
        ROUTES[antk][i+1] = (int) PROBS[next_city][1];
    }
}
int ACO::valid (int antk){
    for(int i=0; i<n-1; i++) {
        int cityi = ROUTES[antk][i];
        int cityj = ROUTES[antk][i+1];

        // if antk did not visit all the cities return -1
        if (cityi < 0 || cityj < 0) {
            return -1;	
        }

        // if antk is stuck in loop return -2
        for (int j=0; j<i-1; j++) {
            if (ROUTES[antk][i] == ROUTES[antk][j]) {
                return -2;
            }	
        }
    }
    // if path followed by antk is a valid path return 0
    return 0;
}

void ACO::updatePHEROMONES (){
    // pheromone(i,j) =  ro * pheromone(i,j) + change in pheromone(i,j)
    // ro = evaporation factor [0,1]
    // change(pheromone(i,j)) = sum[pheromone(i,j) for each ant[1,l]]
    // pheromone(i,j) for ant k = Q / L[k] if ant k travels on path i,j ; else 0
    // L[k] = length of the tour by ant k 
    for (int k=0; k<l; k++) {
        double rlength = length(k);
        for (int r=0; r<n-1; r++) {
            int cityi = ROUTES[k][r];
            int cityj = ROUTES[k][r+1];
            DELTAPHEROMONES[cityi][cityj] += Q / rlength;
            DELTAPHEROMONES[cityj][cityi] += Q / rlength;
        }
    }
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            PHEROMONES[i][j] = (1 - ro) * PHEROMONES[i][j] + DELTAPHEROMONES[i][j];
            DELTAPHEROMONES[i][j] = 0.0;
        }	
    }
}

int main()
{
    ACO *ANTS = new ACO (3,5);

    ANTS -> init();

    ANTS -> CITIES[0][0] = 0;
    ANTS -> CITIES[0][1] = 0;
    ANTS -> CITIES[1][0] = 1;
    ANTS -> CITIES[1][1] = 1;
    ANTS -> CITIES[2][0] = 2;
    ANTS -> CITIES[2][1] = 2;
    ANTS -> CITIES[3][0] = 3;
    ANTS -> CITIES[3][1] = 3;
    ANTS -> CITIES[4][0] = 4;
    ANTS -> CITIES[4][1] = 4;

    ANTS -> optimize (10);

	ANTS -> printRESULTS ();

	return 0;
}
