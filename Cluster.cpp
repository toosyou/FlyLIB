#include "Cluster.h"

void Cluster::load(fstream &seg_strm){

    string buffer;
    int buffer_int = 0;

    seg_strm >> buffer;
    if(buffer != "@0"){
        cerr <<"*******ERROR:LOCATION OF SEGSTRM WRONG*******"<<endl;
        return;
    }

    seg_strm >> buffer;     //Paths
    seg_strm >> buffer_int;
    index_segment_.resize(buffer_int);
    seg_strm >> buffer;     //@1
    for(int i=0;i<index_segment_.size();++i){

        index_segment_[i].resize(2);
        seg_strm >> index_segment_[i][0];
        seg_strm >> index_segment_[i][1];

    }

}

SuperCluster::SuperCluster(const char *address_tct, Brain *brain, bool modified){

    brain_ = brain;
    modified_ = modified;

    loadTCT(address_tct);

}

void SuperCluster::loadTCT(const char *address_tct){

    fstream tct_strm(address_tct,ios::in);

    if(!tct_strm){
        cerr <<"*******ERROR:ADDRESS OF TCT WORNG*******"<<endl;
        return;
    }

    string buffer;
    int size_cluster = 0;

    tct_strm >> buffer;
    if(buffer != "Tracts"){
        cerr <<"*******ERROR:TCT FILE NOT CORRECT*******"<<endl;
        return;
    }
    tct_strm >> size_cluster;
    cluster_.resize( size_cluster );
    for(int i=0;i<cluster_.size();++i){
        cluster_[i].load(tct_strm);
        cluster_[i].setModified(modified_);
        cluster_[i].setBrain(brain_);
    }


    tct_strm.close();
    cout << "===========TCT has been loaded!===========" <<endl;
}
