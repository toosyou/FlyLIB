#include "Brain.h"

#include <iostream>

using namespace std;

Brain::Brain(const char *address_ndb, const char *work_directory){

    loadNDB(address_ndb);

    if(work_directory != NULL){
        this->load_neuropils(work_directory);
    }

}

Brain::~Brain(){

}

void Brain::loadNDB(const char *address_ndb){

    fstream ndb_strm(address_ndb,ios::in);

    if(!ndb_strm){
        cerr << "*******ERROR:CANNOT LOAD .NDB*******" <<endl;
        return ;
    }

    int amount_neuron = 0;
    string buffer;

    ndb_strm >> buffer;                 // Neuron
    if(buffer != "Neuron"){
        cerr << "*******ERROR:ADDRESS OF .NDB NOT CORRECT******" <<endl;
        return;
    }
    ndb_strm >> amount_neuron;

    progressbar *progress = progressbar_new("Loading NDB", amount_neuron);
    for(int i=0;i<amount_neuron;++i){
        neurons_.push_back( Neuron(ndb_strm) );

        progressbar_inc(progress);

    }//i loop
    progressbar_finish(progress);

    ndb_strm.close();
}


void Brain::load_neuropils(const char *work_directory){

    this->neuropils_.resize(58);

    DIR *dir_neuropils = opendir(work_directory);
    dirent *test = readdir(dir_neuropils);
    test = readdir(dir_neuropils);
    test = readdir(dir_neuropils);

    //test
    vector<bool> is_loaded(58,false);

    while(test != NULL){
        //cout << test->d_name <<endl;

        string name_file(test->d_name);

        string temp_address;
        temp_address += work_directory;
        temp_address += name_file;

        //cout << temp_address << endl;

        //getting the index of neuropil
        int number_underline_appeared=0;
        int index_first_underline = 0;
        int index_third_underline = 0;
        int index_neuropil_pair = 0;
        int index_left_right = 0;//left = 0
        int index_neuropil = 0;

        for(int i=0;i<name_file.size();++i){

            if(number_underline_appeared == 0 && name_file[i] == '_'){
                index_first_underline = i;
            }
            if(number_underline_appeared == 2 && name_file[i] == '_'){
                index_third_underline = i;
            }
            if(name_file[i] == '_'){
                number_underline_appeared ++ ;
            }
        }

        string number_neuropil(name_file , index_first_underline+1 , (index_third_underline - index_first_underline - 7));
        if( name_file[index_third_underline+1] == 'r' ){
            index_left_right = 1;
        }
        if(name_file == "lat_tri_7_instd_l_ascii.am")
            index_neuropil = 12;
        else if(name_file == "lat_tri_7_instd_r_ascii.am")
            index_neuropil = 13;
        else{
            index_neuropil_pair = atoi(number_neuropil.c_str()) - 1;
            index_neuropil = index_neuropil_pair*2 + index_left_right;
        }


        //finished getting the index of neuropil

        is_loaded[index_neuropil] = true;
        neuropils_[index_neuropil].load(temp_address.c_str());

        test = readdir(dir_neuropils);
    }
    for(int i=0;i<is_loaded.size();++i){
        if(is_loaded[i] == false){
            cout << i << " is not loaded" <<endl;
        }
    }
    closedir(dir_neuropils);
}

void Brain::delete_neuropils(){
    if(this->neuropils_.size() == 0)
        return ;
    neuropils_.clear();
}

vector<int> vector_tpp(Brain &brain, int index_neuron){

    vector<int> number_tips_in_neuropil(58,0);

    Neuron& neuron = brain.neurons()[index_neuron];

    vector< vector<float> > tips = neuron.tips_original();

    vector<Neuropil>& vector_neuropil = brain.neuropils();

    for(int i=0;i<vector_neuropil.size();++i){
        for(int j=0;j<tips.size();++j){
            if(vector_neuropil[i].is_point_in(tips[j]) == true){
                number_tips_in_neuropil[i]++;
            }
        }
    }

    return number_tips_in_neuropil;

}
