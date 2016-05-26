#ifndef SIMILARITYMATRIX_H
#define SIMILARITYMATRIX_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../FlyLIB/Cluster.h"

using namespace std;

class SimilarityMatrix{

    vector< vector< vector<float> > > matrix_;

public:

    SimilarityMatrix(const char* address);

    void load(const char* address);

    /*******NOT DONE YET*******/
    //SuperCluster* clustering(float threshold,int min_size = 5);
};

#endif // SIMILARITYMATRIX_H
