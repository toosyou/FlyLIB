#ifndef VERTICES_H
#define VERTICES_H

#include <stdio.h>
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
//#include <GL/gl.h>

using namespace std;

class Vertices{

    vector< vector<float> > vertex_;    //[N][3] ( x , y , z )

    vector<float> max_;                 //[3] ( max_x , max_y , max_z )
    vector<float> min_;                 //[3] ( min_x , min_y , min_z )
    vector<float> center_;              //[3] ( center_x , center_y , center_z )

    inline void calParameter();
    inline void calMax();
    inline void calMin();
    inline void calCenter();

public:

    Vertices(fstream &vertices_strm,int size_vertices);
    ~Vertices(){}

    vector<float> operator[](int index){return this->vertex_[index];}
    int size(void){return this->vertex_.size();}

    //Paint functions
    void DrawBoundingBox();
    void DrawCenterPoint();

};

#endif // VERTICES_H
