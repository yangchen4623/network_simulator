#include "packet.h"
#include "phit.h"
#include "node.h"
class link{
    private:
        int latency;
        int direction;//0 is xpos, 1 is xneg, 2 is ypos, 3 is yneg, 4 is zpos, 5 is zneg
        node* src_node;
        node* dst_node;
        phit* phit_list;
        bool back_pressure;//ture: turn on the backpressure to sender. false: no backpressure to sender 
                           //default: no backpressure
    public:
        //default constructor
        link();

        void link_alloc();
        void set_src();
        void set_dst();
        void set_dir();
        void advance_one_cycle()

};

link::link(){
    latency=28;
    back_pressure=false;
}

void link::set_src(node* src){
    src_node=src;

}

void link::set_dst(node* dst){
    dst_node=dst;
}

void link::set_dir(int dir){
    direction=dir;
}

void link::link_alloc(){
    phit_list=new phit[latency];

    //phit[0] conncect to sender
    //phit[latency-1] connect to receiver
    for(int i=0; i<latency; ++i){
        phit_list[i].valid=false;
        phit_list[i].cur_link=this;
    }
}

void link::advance_one_cycle(){
    for(int i=0;i<latency;++i){
        if(back_pressure){
            
        }
        phit_list[0]=src_node
        
    }

}


