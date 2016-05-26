#include "Vertices.h"

Vertices::Vertices(fstream &vertices_strm, int size_vertices){

    string buffer;

    vertices_strm >> buffer;
    if(buffer != "@1"){
        cerr << "*******ERROR:LOCATION OF VERTICES FSTREAM WRONG*******" <<endl;
        return;
    }

    vertex_.resize(size_vertices);
    for(int i=0;i<vertex_.size();++i){
        vertex_[i].resize(3,0);
        vertices_strm >> vertex_[i][0];
        vertices_strm >> vertex_[i][1];
        vertices_strm >> vertex_[i][2];
    }

    calParameter();

}

inline void Vertices::calParameter(){
    calMax();
    calMin();
    calCenter();
}

inline void Vertices::calMax(){

    max_.resize(3,0);

    max_[0] = vertex_[0][0];
    max_[1] = vertex_[0][1];
    max_[2] = vertex_[0][2];

    for(int i=0;i<vertex_.size();++i){
        for(int j=0;j<3;++j){
            if(max_[j] < vertex_[i][j]){
                max_[j] = vertex_[i][j];
            }
        }//j loop
    }//i loop

}

inline void Vertices::calMin(){

    min_.resize(3,0);

    min_[0] = vertex_[0][0];
    min_[1] = vertex_[0][1];
    min_[2] = vertex_[0][2];

    for(int i=0;i<vertex_.size();++i){
        for(int j=0;j<3;++j){
            if(min_[j] > vertex_[i][j]){
                min_[j] = vertex_[i][j];
            }
        }//j loop
    }//i loop

}

inline void Vertices::calCenter(){

    center_.resize(3,0);

    for(int i=0;i<3;++i){
        center_[i] = max_[i] + min_[i];
        center_[i] /= 2.0;
    }

}
