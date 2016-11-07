#include "packet.h"
#include "phit.h"
#include "node.h"
class link{
    private:
        int latency;
        node* src_node;
        node* dst_node;
        phit* phit_list;
        bool back_pressure;//ture: turn on the backpressure to sender. false: no backpressure to sender
    public:
        //default constructor
        link();

        void link_alloc();
        void set_src();
        void set_dst();

};

link::link(){
    latency=28;
}

void link::set_src(node* src){
    src_node=src;

}

void link::set_dst(node* dst){
    dst_node=dst;
}



void link::link_alloc(){
    phit_list=new phit[latency];
    for(int i=0; i<latency; ++i){
        phit_list[i].valid=false;
        phit_list[i].cur_link=this;
    }
}

void link::advance_one_clock(){
    for(int i=0;i<la

}


