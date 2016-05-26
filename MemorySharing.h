#ifndef MEMORYSHARING_H
#define MEMORYSHARING_H

/********************************
 * i gave up on 2015 4/4.
 * All the things i tried didn't work at all..
 * But since i borrowed the code from MSDN,
 * maybe someday i could be lucky enough to make all these work..
 *
 * Girls not worth sadness.
 *
 * toosyou 2015 4/4
 * *******************************/

#include <iostream>
#include <windows.h>
#include <conio.h>
#include <tchar.h>
#include "Brain.h"
#include "Cluster.h"
#include "Neuropil.h"
#include "FilePath.h"

using namespace std;

#define ADDRESS_BRAIN_PAGE_FILE "brain_memory"

class BrainDatabase{

    HANDLE handle_map_file_;
    void* pointer_map_view_;
    Brain* brain_;
    bool brain_neuron_loaded_;
    bool is_online_;
    bool neuropils_loaded_;

public:
    BrainDatabase();
    ~BrainDatabase();
    int create(const char* address_ndb = ADDRESS_NDB_FINAL);
    void load_neuropils(const char* work_directory = ADDRESS_NEUROPILES_WORKDIR);
    void delete_neuropils(void);
    int close(void);

    Brain* connect(void);

    bool brain_neuron_loaded(void){return this->brain_neuron_loaded_;}
    bool is_online(void){return this->is_online_;}
    bool neuropils_loaded(void){return this->neuropils_loaded_;}

    int brain_size_bytes(void){return sizeof(brain_);}
};

BrainDatabase::BrainDatabase(){
    pointer_map_view_ = NULL;
    brain_ = NULL;
    is_online_ = false;
    neuropils_loaded_ = false;
    brain_neuron_loaded_ = false;
}

int BrainDatabase::create(const char *address_ndb){
    if(brain_neuron_loaded_ == false){
        this->brain_ = new Brain(address_ndb);
        brain_neuron_loaded_ = true;
    }

    //create the handle to make the brain memory online
    this->handle_map_file_ =    CreateFileMapping(
                                    INVALID_HANDLE_VALUE,    // use paging file
                                    NULL,                    // default security
                                    PAGE_READWRITE,          // read/write access
                                    0,                       // maximum object size (high-order DWORD)
                                    1024*1024,        // maximum object size (low-order DWORD)
                                    TEXT(ADDRESS_BRAIN_PAGE_FILE));// name of mapping object)
    //if error accrues
    if(handle_map_file_ == NULL){
        cerr << "********Error : Cannot Create Handle*******\t";
        cerr << "Error Code = " << GetLastError() <<endl;
        return -1;
    }

    //get the pointer of map view
    pointer_map_view_ = (void*)MapViewOfFile(handle_map_file_,   // handle to map object
                                      FILE_MAP_ALL_ACCESS, // read/write permission
                                      0,
                                      0,
                                      1024*1024);
    //if error accrues
    if(pointer_map_view_ == NULL){
        cerr << "********Error : Cannot Get Map View*******\t";
        cerr << "Error Code = " << GetLastError() <<endl;
        return -1;
    }
    //copy the brain data to the map
    CopyMemory((PVOID)pointer_map_view_, (const void*)brain_, 1024*1024 );
    cout << ((Brain*)pointer_map_view_)->size() <<endl;
    is_online_ = true;
    return 0;//succeed

}

void BrainDatabase::load_neuropils(const char *work_directory){
    if(brain_ == NULL){
        cout << "*********Error : Brain Hasn't Been Loaded********"<<endl;
        return ;
    }

    bool need_online = is_online_;
    if(is_online_ == true)
        this->close();

    if(this->brain_->size_neuropiles() != 0)
        brain_->delete_neuropils();
    brain_->load_neuropils(work_directory);
    if(brain_->size_neuropiles() != 0)
        neuropils_loaded_ = true;

    if(need_online == true)
        this->create();
}

void BrainDatabase::delete_neuropils(){
    if(this->neuropils_loaded_ == false)
        return;

    bool need_online = is_online_;
    if(is_online_ == true)
        this->close();

    this->brain_->delete_neuropils();
    neuropils_loaded_ = false;

    if(need_online == true)
        this->create();
}

int BrainDatabase::close(){
    UnmapViewOfFile(pointer_map_view_);
    CloseHandle(handle_map_file_);
    is_online_ = false;
}

Brain* BrainDatabase::connect(void){

    //make the connection
    handle_map_file_ = OpenFileMapping(
                        FILE_MAP_ALL_ACCESS,
                        FALSE,
                        TEXT(ADDRESS_BRAIN_PAGE_FILE));
    //if error accrues
    if(handle_map_file_ == NULL){
        cerr << "********Error : Cannot Make Connection With Handle*******\t";
        cerr << "Error Code = " << GetLastError() <<endl;
        return NULL;
    }
    //point brain to mapview
    brain_ = (Brain*)MapViewOfFile(handle_map_file_,
                                    FILE_MAP_ALL_ACCESS,
                                  0,
                                  0,
                                  1024*1024);
    //if error accrues
    if(brain_ == NULL){
        cerr << "*******Error : Cannot Point to Map View********\t\t";
        cerr << "Error Code = " <<GetLastError() <<endl;
        return NULL;
    }
    //succeed
    return brain_;
}

BrainDatabase::~BrainDatabase(){
    this->close();
    if(brain_!=NULL)
        delete brain_;
}

#endif // MEMORYSHARING_H
