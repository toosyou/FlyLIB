#ifndef NEURON_H
#define NEURON_H

#include "Segment.h"
#include "Vertices.h"
#include "PCA.h"
#include <sstream>

class Neuron{

    string neuron_name_;
    Vertices *original_vertices_;

    vector<Segment> original_paths_;
    vector<Segment> modified_paths_;
    vector< vector<int> > removed_paths_;   // removed paths that make diff.
    vector<int> fork_paths_;                // removed paths that make no diff.
    vector<int> soma_;

    vector<int> index_vertices_tips_original_;
    vector<int> index_vertices_branches_original_;

    vector<float> max_;                     //( max_x , max_y , max_z )
    vector<float> min_;                     //( min_x , min_y , min_z )
    vector<float> center_;                  //( center_x , center_y , center_z )

    vector<float> color_;                   //( r , g , b )

    int size_removed_paths_;

    void cal_tips_branches(void);
    void load_from_strm_(fstream &neuron_strm);


public:

    Neuron(fstream &neuron_strm);

    int size_vertices(void){return this->original_vertices_->size();}
    int size_original_paths(void){return this->original_paths_.size();}
    int size_modified_paths(void){return this->modified_paths_.size();}

    string name(void){return this->neuron_name_;}
    Vertices* original_vertices(void){return this->original_vertices_;}
    vector<Segment>& original_paths(void){return this->original_paths_;}

    vector<vector<float> > tips_original(void);//return the vertices of tips
    vector<vector<float> > branches_original(void); // return the vertices of branch points

    vector< vector<float> > pca(void);

    // Paint functions
    void DrawBoundingBox(bool do_shift = false);       //
    void DrawNeurons();

};

#endif // NEURON_H
