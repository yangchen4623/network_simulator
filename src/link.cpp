#include "link.h"

link::link(){
    latency = LINKDELAY;
}



link::link(int Latency, int Dir, node* Src_node, node* Dst_node){
    latency = Latency;
    dir = Dir;
    src_node = Src_node;
    dst_node = Dst_node;
    //allocate space for all the flits in alloc
}

void link::link_alloc(){
    phit_list=new flit[latency];

    //phit[0] conncect to sender
    //phit[latency-1] connect to receiver
    for(int i = 0; i < latency; ++i){
        phit_list[i].valid = false;
    }
    out = phit_list[latency - 1];
}

void link::consume(){
    in_latch = src_node -> out[dir];
    
}

void link::produce(){
    int out = phit_list[latency - 1];
    for(int i = latency - 1; i > 0; --i){
        phit_list[i] = phit_list[i - 1];
    }
    phit_list[0] = in_latch;
}

