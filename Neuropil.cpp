#include "Neuropil.h"

//#define INFINITY (INT_MAX-1000)

Neuropil::Neuropil(const char *address_am){

    this->load(address_am);

}

void Neuropil::load(const char *address_am){

    cout << "Loading " << address_am <<" ...\t";

    fstream strm_am(address_am);
    string buffer(1,' ') ;

    if(strm_am.is_open() == false){
        cerr << "********FAIL TO OPEN .AM FILE********" <<endl;
        return ;
    }

    while(buffer != "Lattice"){
        strm_am >> buffer;
    }

    size_.resize(3,0);
    strm_am >> size_[0];
    strm_am >> size_[1];
    strm_am >> size_[2];

    intensity_.resize(size_[0]);
    for(int i=0;i<size_[0];++i){
        intensity_[i].resize(size_[1]);
        for(int j=0;j<size_[1];++j){
            intensity_[i][j].resize(size_[2]);
        }
    }

    while(buffer != "BoundingBox"){
        strm_am >> buffer;
    }

    max_.resize(3,0);
    min_.resize(3,0);

    strm_am >> min_[0];
    strm_am >> max_[0];
    strm_am >> min_[1];
    strm_am >> max_[1];
    strm_am >> min_[2];
    strm_am >> max_[2];

    while(buffer != "@1"){
        strm_am >> buffer;
    }
    buffer = ' ';
    while(buffer != "@1"){
        strm_am >> buffer;
    }

    for(int z=0;z<size_[2];++z){
        if(z >= intensity_[0][0].size())
            cout << "z = " << z <<endl;
        for(int y=0;y<size_[1];++y){
            if(y >= intensity_[0].size())
                cout << "y = " << y <<endl;
            for(int x=0;x<size_[0];++x){
                if(x >= intensity_.size())
                    cout << "x = " << x <<endl;

                strm_am >> intensity_[x][y][z];
            }
        }
    }


    strm_am.close();
    cout << "Done." <<endl;
}

void Neuropil::write(const char *address_am){
    cout << "Writing " << address_am << " ...\t" ;
    fstream out_am(address_am,fstream::out);
    if(!out_am.is_open()){
        cerr << "********FAIL TO OPEN .AM FILE********" <<endl;
        return ;
    }

    out_am <<"# AmiraMesh 3D ASCII 2.0" <<endl <<endl<<endl;
    out_am <<"define Lattice "<<size_[0] <<" ";
    out_am <<size_[1]<<" ";
    out_am <<size_[2]<<endl<<endl;

    out_am <<"Parameters {"<<endl;
    out_am <<"    Colormap \"redblue.icol\"," <<endl;
    out_am <<"    Expression \"(b)\"," <<endl;
    out_am <<"    Content \""<<size_[0]<<"x"<<size_[1]<<"x"<<size_[2]<<" ushort, uniform coordinates\","<<endl;
    out_am <<"    SaveInfo \"AmiraMesh ZIP\","<<endl;
    out_am <<"    BoundingBox "<< min_[0] <<" ";
    out_am << max_[0] <<" ";
    out_am << min_[1] <<" ";
    out_am << max_[1] <<" ";
    out_am << min_[2] <<" ";
    out_am << max_[2] <<","<<endl;
    out_am <<"    CoordType \"uniform\""<<endl;
    out_am <<"}"<<endl<<endl;

    out_am <<"Lattice { ushort Data } @1"<<endl<<endl;

    out_am <<"# Data section follows"<<endl;
    out_am <<"@1"<<endl;

    for(int z=0;z<size_[2];++z)
        for(int y=0;y<size_[1];++y)
            for(int x=0;x<size_[0];++x){
                out_am << (int)intensity_[x][y][z] <<endl;
            }
    cout << "DONE." <<endl;
    out_am.close();
}

bool Neuropil::is_point_in(vector<float> point){

    //shift point to the center of neuropil
    for(int i=0;i<3;++i){
        point[i] -= min_[i];

        if( (int)(point[i]+0.5) < 0 || (int)(point[i]+0.5) > size_[i]-1 ) // out of boundary
            return false;
    }

    if(intensity_[ (int)(point[0]+0.5) ][  (int)(point[1]+0.5) ][ (int)(point[2]+0.5) ] > 0 )
        return true;

    else
        return false;
}

void Neuropil::distance_transform(int threshold){

    cout << "Distance transforming ...\t";

    vector< vector< vector<int> > > binary_map;
    queue< vector<int> > border_points; //[n] ( x , y , z )

    //create binary map
    binary_map.resize(intensity_.size());
    for(int i=0;i<intensity_.size();++i){
        binary_map[i].resize(intensity_[i].size());
        for(int j=0;j<intensity_[i].size();++j){
            binary_map[i][j].resize(intensity_[i][j].size());
            for(int k=0;k<intensity_[i][j].size();++k){
                if(intensity_[i][j][k] < threshold)
                    binary_map[i][j][k] = 0;
                else
                    binary_map[i][j][k] = 1;
            }
        }
    }

    //initialize intensity with 1 and infinity
    //where border is 1 ,otherwise is infinity
    //and put border into vector
#pragma omp parallel for
    for(int i=0;i<intensity_.size();++i){
        for(int j=0;j<intensity_[i].size();++j){
            for(int k=0;k<intensity_[i][j].size();++k){
                if(binary_map[i][j][k] == 0){
                    intensity_[i][j][k] = 0;
                    continue;
                }

                if( i == 0 || j == 0 || k == 0){
                    intensity_[i][j][k] = INFINITY;
                    continue;
                }
                if( i == intensity_.size()-1 || j == intensity_[i].size()-1 || k == intensity_[i][j].size()-1){
                    intensity_[i][j][k] = INFINITY;
                    continue;
                }

                bool continue_all = false;
                for(int p=0;p<3;++p){
                    if(continue_all == true)
                        break;
                    for(int q=0;q<3;++q){
                        if(continue_all == true)
                            break;
                        for(int r=0;r<3;++r){
                            if( binary_map[i+p-1][j+q-1][k+r-1] != binary_map[i][j][k]){
                                intensity_[i][j][k] = 1;
                                vector<int> temp_point(3,0);
                                temp_point[0] = i;
                                temp_point[1] = j;
                                temp_point[2] = k;
                                border_points.push(temp_point);
                                continue_all = true;
                                break;
                            }
                        }
                    }
                }
                if(continue_all == true)
                    continue;

                intensity_[i][j][k] = INFINITY;

            }
        }
    }

    //DijkstraVectors(DV) distance transform method
    vector< vector< vector<float> > > window_distance;
    window_distance.resize(3);
    for(int i=0;i<3;++i){
        window_distance[i].resize(3);
        for(int j=0;j<3;++j){
            window_distance[i][j].resize(3,0);
            for(int k=0;k<3;++k){
                window_distance[i][j][k] = sqrt( ((float)(i-1))*((float)(i-1)) +
                                                 ((float)(j-1))*((float)(j-1)) +
                                                 ((float)(k-1))*((float)(k-1)) );
            }
        }
    }

    while(!border_points.empty()){
        vector<int> temp_point(3,0);
        vector<int> point = border_points.front();
        border_points.pop();

        int x = point[0];
        int y = point[1];
        int z = point[2];

        //check elements inside the window
        for(int i=0;i<3;++i){
            for(int j=0;j<3;++j){
                for(int k=0;k<3;++k){
                    if( x-1+i < 0 || y-1+j < 0 || z-1+k < 0){
                        continue;
                    }
                    if( x-1+i >= intensity_.size() || y-1+j >= intensity_[i].size() || z-1+k >= intensity_[i][j].size()){
                        continue;
                    }

                    if( binary_map[x-1+i][y-1+j][z-1+k] == 1 &&
                            intensity_[x][y][z] + window_distance[i][j][k] < intensity_[x-1+i][y-1+j][z-1+k]){
                        intensity_[x-1+i][y-1+j][z-1+k] = intensity_[x][y][z] + window_distance[i][j][k];
                        temp_point[0] = x-1+i;
                        temp_point[1] = y-1+j;
                        temp_point[2] = z-1+k;
                        border_points.push(temp_point);
                    }
                }
            }
        }
    }
#pragma omp parallel for
    for(int i=0;i<intensity_.size();++i){
        for(int j=0;j<intensity_[i].size();++j){
            for(int k=0;k<intensity_[i][j].size();++k){
                if(intensity_[i][j][k] == 0 || intensity_[i][j][k] == INFINITY){
                    intensity_[i][j][k] = 0;
                    continue;
                }
                intensity_[i][j][k] *= 100;
            }
        }
    }
    cout << "DONE." <<endl;

}
