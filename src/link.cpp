#include "link.h"
#include <stdlib.h>
#include <stdio.h>

void link::link_init(int Latency, int Dir, flit* In, node* Src, node* Dst){
    latency = Latency;
    dir = Dir;
    in = In;
    dst = Dst;
    src = Src;
    if(!(phit_list = (flit*)malloc(latency * sizeof(flit)))){
        printf("No mem space for phit list in link from (%d, %d, %d) to (%d, %d, %d)\n", src->cur_x, src->cur_y, src->cur_z, dst->cur_x, dst->cur_y, dst->cur_z);
        exit(-1);
    }
    //phit[0] conncect to sender
    //phit[latency-1] connect to receiver
    for(int i = 0; i < latency; ++i){
        phit_list[i].valid = false;
    }
    out = phit_list[latency - 1];

	in_latch.valid = false;
    //allocate space for all the flits in alloc
}


void link::consume(){
    in_latch = *in;
    
}

void link::produce(){
    out = phit_list[latency - 1];
    for(int i = latency - 1; i > 0; --i){
        phit_list[i] = phit_list[i - 1];
    }
    phit_list[0] = in_latch;
}

void link::link_free(){
    free(phit_list);
}

