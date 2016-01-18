//Copyright [2016] [Wei Zhang]

//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//http://www.apache.org/licenses/LICENSE-2.0
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and

//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
//*Create date: 1/17/2016 By: Wei Zhang
//
//*Description: 
//  **Random model generation
//
//*Update:
//  **
//^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>
#include <limits>
#include <utility>

#include <boost/timer.hpp>

#include "common.h"
#include "lib/rng.h"

#include "../facthawkes/hawkes.h"

using namespace std;

RNG rng;

int MODELID=0;
int NUSER=0;
int NITEM=0;
int NDIM=10;
string MODELFILE;

int parseArgs(int argc, char* argv[]) {
	for(int i=1;i<argc;i++) {
		if(strcmp("-modelid", argv[i])==0) {
			MODELID = atoi(argv[++i]);
			continue;
		}
		if(strcmp("-nuser", argv[i])==0) {
			NUSER = atoi(argv[++i]);
			continue;
		}
		if(strcmp("-nitem", argv[i])==0) {
			NITEM = atoi(argv[++i]);
			continue;
		}
		if(strcmp("-modelfn", argv[i])==0) {
			MODELFILE = argv[++i];
			continue;
		}
		cout << "Unknown parameters" << endl;
		return -1;
	}
	return 0;
}


void genHawkes() {
    HawkesModel model;
	
    model.mu = rng.uniform(0,1);
	model.alpha = rng.uniform(0.1,0.5);
    model.w = 1;

    out << model << endl;
}


void genMultiHawkes() {
    MultiHawkesModel model(NITEM);

    model.w = 1;
	for(size_t i=0; i<model.D; i++) model.mu[i] = rng.uniform(0,0.1)*10.00/D;
    
	int_pairs_t ind((model.D+2)*(model.D+1)/2);
    int idx = 0;
    for(size_t i=0; i<model.D; i++)
        for(size_t j=0; j<=i; j++) {
            ind[idx].first = i;
            ind[idx].second = j;
            idx += 1;
        }
	random_shuffle(ind.begin(), ind.end());
	for(size_t i=0;i<ind.size()*0.1;i++) 
        model.alpha[ind[i].first][ind[i].second] = rng.uniform(0.1,0.4)*10.0/model.D;

    out << model << endl;
}


void genFactHawkes1() {
    FactHawkesModel1 model(NUSER, NITEM, NDIM);

    for (int u=0; u<model.n_users; u++)
        for (int k=0; k<model.n_dim; k++) {
            u_factor[u][k] = rng.uniform(0,0.4)*10.0/model.n_dim;
            ut_factor[u][k] = rng.uniform(0,0.4)*10.0/model.n_dim;
        }

    for (int i=0; i<model.n_items; i++)
        for (int k=0; k<model.n_dim; k++) {
            u_factor[u][k] = rng.uniform(0,0.4)*10.0/model.n_dim;
            ut_factor[u][k] = rng.uniform(0,0.4)*10.0/model.n_dim;
        }

    out << model << endl;
}


int main(int argc, char* argv[]) {
	if(parseArgs(argc, argv)!=0)
		return -1;
   
    if (MODELID == 0)
        return -1;
    else if (MODELID == 1)
        genHawkes();
    else if (MODELID == 2)
        genMultiHawkes();
    else if (MODELID == 3) {
        genFactHawkes1();
    }
    else
        return -1;

	return 0;
}

