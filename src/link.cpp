#include "packet.h"
#include "node.h"
class link{
    private:
        int latency;
        node* src_node;
        node* dst_node;
        packet* packet_list;
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
    packet_list=new packet[latency];
    for(int i=0; i<latency; ++i){
        packet[i]
    }
}


