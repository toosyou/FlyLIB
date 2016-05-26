#ifndef BRAIN_H
#define BRAIN_H

#include <vector>
#include <fstream>
#include <iostream>
#include <dirent.h>

#include "Neuron.h"
#include "Neuropil.h"

using namespace std;

class Brain{

    vector<Neuron> neurons_;
    vector<Neuropil> neuropils_;

public:

    Brain(const char* address_ndb,const char* work_directory=NULL);     //load .ndb
    ~Brain();

    int size(void){return this->neurons_.size();}
    int size_neuropiles(void){return this->neuropils_.size();}

    vector<Neuropil>& neuropils(void){return this->neuropils_;}
    vector<Neuron>& neurons(void){return this->neurons_;}

    Neuron& operator [](int index){return this->neurons_[index];}

    //load and save functions

    void loadNDB(const char* address_ndb);
    void load_neuropils(const char* work_directory);
    void saveNDB(const char* address_ndb);

    void delete_neuropils(void);
    // Paint functions

};

vector<int> vector_tpp(Brain &brain,int index_neuron);

#endif // BRAIN_H
