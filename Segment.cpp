#include "Segment.h"

Segment::Segment(fstream &seg_strm,Vertices *pVertex){
    vertex_ = pVertex;
    modified_ = false;

    int buffer_int = 0;

    seg_strm >> buffer_int;
    while(buffer_int != -1){
        lines_.push_back(buffer_int);
        seg_strm >> buffer_int;
    }
}

Segment::~Segment(){

}

void Segment::setDirection(vector<float> &direction){
    direction_ = direction;
}
void Segment::setCenter(vector<float> &center){
    center_ = center;
}
void Segment::setColor(vector<float> &color){
    color_ = color;
}
void Segment::setLength(float length){
    length_ = length;
}
void Segment::setModified(bool modified){
    modified_ = modified;
}
