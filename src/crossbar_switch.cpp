#include "define.h"
#include "flit.h"
#include "crossbar_switch.h"
#include<stdio.h>
#include<stdlib.h>

void crossbar_switch::crossbar_switch_init(int Mode, flit** In_list, bool** Out_avail){
	N_fan_in = N_FAN_IN;
	N_fan_out = PORT_NUM;
    mode = Mode;
	for(int j = 0; j < N_FAN_IN; ++j)
		in_latch[j].valid = false;

    for(int i = 0; i < PORT_NUM; ++i){
        for(int j = 0; j < N_FAN_IN; ++j){
            in_avail[i][j] = true;
        }
        out[i].valid = false;
        out_avail_latch[i] = true;
    }
    for(int i = 0; i < N_FAN_IN; ++i){
        flit_in[i] = In_list[i];
    }
    for(int i = 0; i < PORT_NUM; ++i){
        out_avail[i] = Out_avail[i];
    }


        for(int j = 0; j < N_FAN_IN; ++j){
            in_list_to_tree[j] = &in_latch[j];        
        }
  
    for(int i = 0; i < PORT_NUM; ++i){
        out_avail_to_tree[i] = &out_avail_latch[i];
    }
    


    for(int i = 0; i < PORT_NUM; ++i){
        tree_list[i].reduction_tree_init(N_fan_in, i + 1, 4, mode, 18, 6, 2, in_list_to_tree, out_avail_to_tree[i]);
    }
    

}

void crossbar_switch::consume(){
    //latch all the in data
	for(int i = 0; i < N_FAN_IN; ++i)
		in_latch[i] = *(flit_in[i]);


    //latch all the out_avail
    for(int i = 0; i < PORT_NUM; ++i){
        out_avail_latch[i] = *(out_avail[i]);
    }
    
    //call all the consume functions of all the reduction tree
    
    for(int i = 0; i < PORT_NUM; ++i){
        tree_list[i].consume();
    }
    return;
}
void crossbar_switch::produce(){
    //call all the produce functions of all the reduction trees
    for(int i = 0; i < PORT_NUM; ++i){
        tree_list[i].produce();
    }

    //update the out 
    for(int i = 0; i < PORT_NUM; ++i){
        out[i] = tree_list[i].out;
    }

    //the in_avail belongs to the lookup_in_avail

}

bool crossbar_switch::lookup_in_avail(int port_id, int out_dir){
    if(out_dir > 6 || out_dir < 1){
        printf("dir %d is wrong, port_id is %d\n", out_dir, port_id);
        exit(-1);
    }
    if(port_id >= N_FAN_IN || port_id < 0){
        printf("port_id is wrong\n");
        exit(-1);
    }
    return tree_list[out_dir - 1].in_avail[port_id];
}

void crossbar_switch::crossbar_switch_free(){
    for(int i = 0; i < PORT_NUM; ++i){
        tree_list[i].reduction_tree_free();
    }
}
