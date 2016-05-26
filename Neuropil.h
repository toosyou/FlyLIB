#ifndef NEUROPIL_H
#define NEUROPIL_H

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <math.h>

using namespace std;

class Neuropil{

    string name_;

    vector< vector< vector<float> > > intensity_; //[x][y][z]

    vector<int> size_; //( x , y , z )

    vector<float> max_; //( x , y , z )
    vector<float> min_; //( x , y , z )

public:

    Neuropil(){}

    Neuropil(const char* address_am);
    void load(const char* address_am);
    void write(const char* address_am);

    bool is_point_in(vector<float> point);

    void distance_transform(int threshold = 10);

};

#endif // NEUROPIL_H
