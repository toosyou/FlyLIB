#ifndef SEGMENT_H
#define SEGMENT_H

#include <stdio.h>
#include <fstream>
#include <iostream>
#include <vector>

#include "Vertices.h"

using namespace std;

class Segment{

    Vertices *vertex_;

    vector<int> lines_;         // a1,a2,a3...aN

    vector<float> direction_;   // ( i , j , k )
    vector<float> center_;      // ( x , y , z )
    vector<float> color_;       // ( r , g , b )

    float length_;              // length of the segment
    bool modified_;             // whether this segment has been modified. Modified : true

public:

    Segment(fstream &seg_strm,Vertices *pVertex); // load lines_ and vertex
    ~Segment();

    int size(void){return lines_.size();}
    float length(void){return length_;}
    vector<float> direction(void){return direction_;}
    vector<float> center(void){return center_;}
    vector<float> color(void){return color_;}

    int operator [](int index){return this->lines_[index];}

    //load and save
    void setDirection(vector<float> &direction);
    void setCenter(vector<float> &center);
    void setColor(vector<float> &color);
    void setLength(float length);
    void setModified(bool modified);

    // Paint functions
    void Draw();
    void DrawCenter();
    void DrawDirection();

};

#endif // SEGMENT_H
