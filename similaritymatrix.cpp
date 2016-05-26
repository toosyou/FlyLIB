#include "similaritymatrix.h"
#include "Cluster.h"

SimilarityMatrix::SimilarityMatrix( const char* address ){
    load(address);
}

void SimilarityMatrix::load(const char *address){
    //initialization
    matrix_.clear();
    matrix_.resize(15583);

    //Processing Bar
    int size_bar = 40;
    int progress = 0;
    int progress_mtx = -1;
    int step_bar = 15583 / size_bar;

    //Load MTX file
    float buffer[4] ;
    fstream mtx_in(address,fstream::in);

    if(!mtx_in.is_open()){
        cerr << "*******FILE NOT OPENED CORRECTLY*******" <<endl;
    }

    cout << "Loading MTX\t[" <<string(size_bar,' ')<<']'; //Processing Bar

    while(mtx_in >> buffer[0]){ // n1
        mtx_in >> buffer[1];    // n2
        mtx_in >> buffer[2];    // p1
        mtx_in >> buffer[3];    // p2

        //push data in
        vector<float> matrix__temp(3,0);

        matrix__temp[0] = buffer[1];
        matrix__temp[1] = buffer[2];
        matrix__temp[2] = buffer[3];

        matrix_[ (int)buffer[0] ].push_back(matrix__temp);

        matrix__temp[0] = buffer[0];
        matrix__temp[1] = buffer[3];
        matrix__temp[2] = buffer[2];

        matrix_[ (int)buffer[1] ].push_back(matrix__temp);

        //Processing bar
        if(progress_mtx != (int)buffer[0]){

            progress_mtx = (int)buffer[0];

            if( ( progress_mtx +1) % step_bar == 0 ){
                progress++;
                cout << string( size_bar-progress+2,'\b' );
                cout << '=' << string( size_bar-progress,' ' ) <<']';
            }
        }
    }
    cout <<endl;

    mtx_in.close();
    return;
}


SuperCluster* SimilarityMatrix::clustering(float threshold, int min_size){
    /*Using BFS for the time being.
      Union_Find will bring better performance*/
    /*******NOT DONE YET*******/
    SuperCluster* cluster = new SuperCluster();

    if(min_size > 0){
        cluster.resize(1);
    }

    //Processing Bar
    int size_bar = 40;
    int progress = 0;
    int step_bar = 15583 / size_bar;

    cout << "Calculating BFS\t[" <<string(size_bar,' ')<<']'; //Processing Bar

    for(int i=0;i<15583;++i){

        //Processing bar
        if( (i+1) % step_bar == 0 ){
            progress++;
            cout << string( size_bar-progress+2,'\b' );
            cout << '=' << string( size_bar-progress,' ' ) <<']';
        }

        vector<int> cluster_temp;
        vector<int> cluster_cur;

        cluster_cur.push_back(i);

        while(!cluster_cur.empty()){

            vector<int> cluster_precur(cluster_cur);

            //push back cluster_cur to cluster_temp
            for(vector<int>::iterator it=cluster_cur.begin() ; it!=cluster_cur.end() ; ++it){
                cluster_temp.push_back(*it);
                neuron_is_taken[*it] = true;
            }

            cluster_cur.clear();

            for(vector<int>::iterator it = cluster_precur.begin() ; it != cluster_precur.end() ; ++it){

                for(int j=0;j<matrix_[*it].size();++j){

                    int id_neuron = matrix_[*it][j][0];

                    if( neuron_is_taken[ id_neuron ] == false )                     // neuron not taken
                        if( matrix_[*it][j][1] >= P_THRESHOLD )                   // p1 >= 0.6
                            if( matrix_[*it][j][2] >= P_THRESHOLD ){              // p2 >= 0.6

                            cluster_cur.push_back( id_neuron );
                            neuron_is_taken[ id_neuron ] = true;

                    }

                }//j loop

            }// it loop

        }


        if(min_size > 0){
            // push in the cluster if its size is larger than(and equal to) 5
            if(cluster_temp.size() >= min_size)
                cluster.push_back(cluster_temp);
            //otherwise, push the cluster into cluster[0]
            else{
                for(int j=0;j<cluster_temp.size();++j){
                    cluster[0].push_back(cluster_temp[j]);
                }
            }
        }
        else{
            cluster.push_back(cluster_temp);
        }



    }//i loop
    cout << endl;
}
