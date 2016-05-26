#ifndef CLUSTER_H
#define CLUSTER_H

#include <vector>
#include <fstream>
#include <iostream>

#include "Neuron.h"
#include "Segment.h"
#include "Brain.h"
#include "../FlyLIB/similaritymatrix.h"

using namespace std;

class Cluster{

    Brain *brain_;

    vector< vector<int> > index_segment_; // [index_neuron][index_segment]

    vector<float> center_;
    vector<float> color_;

    bool modified_;

    friend class SimilarityMatrix;

public:

    Cluster(){}
    ~Cluster(){}

    int size(void){return index_segment_.size();}
    vector<float> center(void){return center_;}
    vector<float> color(void){return color_;}
    bool modified(void){return modified_;}
    vector<int>& operator [](int index){return index_segment_[index];}

    //set functions
    void setModified(bool modified){modified_ = modified;}
    void setBrain(Brain *brain){brain_ = brain;}

    //load and save functions
    void  load(fstream &seg_strm);

    // Paint functions
    void Draw();

};

class SuperCluster{

    Brain *brain_;

    vector<Cluster> cluster_;

    bool modified_;

    friend class SimilarityMatrix;

public:

    SuperCluster(const char* address_tct ,Brain* brain ,bool modified);
    SuperCluster(Brain* brain=NULL,bool modified=false):brain_(brain),modified_(modified){}
    ~SuperCluster(){}

    int size(void){return cluster_.size();}
    Cluster& operator [](int index){return cluster_[index];}

    void resize(int new_size){cluster_.resize(new_size);}
    void push_back(Cluster& new_cluster){cluster_.push_back(new_cluster);}

    //load functions
    void loadTCT(const char* address_tct);

    //Paint functions

};
#endif // CLUSTER_H
