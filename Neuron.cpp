#include "Neuron.h"

void Neuron::cal_tips_branches(){

    vector<int> counter(this->original_vertices_->size(),0);

    for(int i=0;i<this->original_paths_.size();++i){

        counter[ original_paths_[i][0] ]++;
        counter[ original_paths_[i][ original_paths_[i].size()-1 ] ]++;

    }

    for(int i=0;i<counter.size();++i){
        if(counter[i] == 1){ // it's a tip
            this->index_vertices_tips_original_.push_back(i);
        }
        else if(counter[i] > 2){ // it's a branch point
            this->index_vertices_branches_original_.push_back(i);
        }
    }

}

void Neuron::load_from_strm_(fstream &neuron_strm){
    string buffer;
    float buffer_float = 0;
    int buffer_int = 0;
    int size_vertices = 0;
    int size_original_paths =0;
    int size_modified_paths =0;
    this->soma_.resize(3, 0);

    neuron_strm >> buffer;
    if(buffer!="@0"){
        cerr << "******ERROR:LOCATION OF NEURON FSTREAM WRONG*******" <<endl;
        return;
    }
    neuron_strm >> buffer;          //Neuron
    neuron_strm >> neuron_name_;
    neuron_strm >> buffer;          //Lines
    neuron_strm >> size_original_paths;
    neuron_strm >> buffer;          //Vertices
    neuron_strm >> size_vertices;
    for(int i=0;i<8;++i){
        neuron_strm >> buffer;
    }
    neuron_strm >> buffer;          //ModLines
    neuron_strm >> size_modified_paths;
    neuron_strm >> buffer;          //ModVertices
    neuron_strm >> buffer_int;      //not used yet
    neuron_strm >> buffer;          //RemLines
    neuron_strm >> size_removed_paths_;
    neuron_strm >> buffer >> soma_[0] >> soma_[1] >> soma_[2];          //Soma
    // Soma hasn't haven data yet.

    original_vertices_ = new Vertices(neuron_strm,size_vertices);

    neuron_strm >> buffer;          //@2
    for(int i=0;i<size_original_paths/3;++i){
        original_paths_.push_back( Segment(neuron_strm,original_vertices_) );
    }
    neuron_strm >> buffer;          //@3
    for(int i=0;i<size_modified_paths;++i){
        modified_paths_.push_back( Segment(neuron_strm,original_vertices_) );
    }
    neuron_strm >> buffer;          //@4
    removed_paths_.resize(size_modified_paths);
    for(int i=0;i<size_modified_paths;++i){
        neuron_strm >> buffer_int;
        while( buffer_int != -1 ){
            removed_paths_[i].push_back(buffer_int);
            neuron_strm >> buffer_int;
        }
    }
    neuron_strm >> buffer;          //@5
    if(size_modified_paths != 0 ){ // it's modified
        neuron_strm >> buffer_int;
        while(buffer_int != -1){
            fork_paths_.push_back(buffer_int);
            neuron_strm >> buffer_int;
        }
    }
    neuron_strm >> buffer;          //@6
    for(int i=0;i<size_modified_paths;++i){
        vector<float> temp_direction(3,0);
        neuron_strm >> temp_direction[0];
        neuron_strm >> temp_direction[1];
        neuron_strm >> temp_direction[2];
        modified_paths_[i].setDirection(temp_direction);
    }
    neuron_strm >> buffer;          //@7
    for(int i=0;i<size_modified_paths;++i){
        neuron_strm >> buffer_float;
        modified_paths_[i].setLength(buffer_float);
    }
    neuron_strm >>buffer;           //@8
    for(int i=0;i<size_modified_paths;++i){
        vector<float> temp_center(3,0);
        neuron_strm >> temp_center[0];
        neuron_strm >> temp_center[1];
        neuron_strm >> temp_center[2];
        modified_paths_[i].setCenter(temp_center);
        modified_paths_[i].setModified(true);
    }
}

Neuron::Neuron(fstream &neuron_strm){

    load_from_strm_(neuron_strm);
    cal_tips_branches();

}

vector< vector<float> > Neuron::tips_original(){

    vector< vector<float> > tips;

    for(int i=0;i<index_vertices_tips_original_.size();++i){
        int index_vertices = index_vertices_tips_original_[i];
        tips.push_back( (*original_vertices_)[ index_vertices ] );
    }

    return tips;

}

vector<vector<float> > Neuron::branches_original(){

    vector<vector<float> > branches;

    for(int i=0;i<index_vertices_branches_original_.size();++i){
        int index_vertices = index_vertices_branches_original_[i];
        branches.push_back( (*original_vertices_)[index_vertices] );
    }

    return branches;
}

vector< vector<float> > Neuron::pca(void){

    float **result;
    vector< vector<float> > result_vector;

    //make the data for PCA()
    int size_points = this->original_vertices_->size();

    float **points = new float*[size_points];
    for(int i=0;i<size_points;++i){
        points[i] = new float[3];
        for(int j=0;j<3;++j){
            points[i][j] = (*original_vertices_)[i][j];
        }
    }

    result = PCA(points,3,size_points);

    result_vector.resize(3);
    for(int i=0;i<3;++i){
        result_vector[i].resize(4);
        for(int j=0;j<4;++j){
            result_vector[i][j] = result[i][j];
        }
    }

    //delete points[][] and result**
    for(int i=0;i<size_points;++i){
        delete [] points[i];
    }
    delete [] points;
    for(int i=0;i<3;++i){
        delete [] result;
    }
    delete [] result;

    //return result;
    return result_vector;
}
