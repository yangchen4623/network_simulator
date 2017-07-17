#include "define.h"
#include "flit.h"
#include "crossbar_switch.h"

crossbar_switch::crossbar_switch_init(flit** In_list, bool** Out_avail){
    for(int i = 0; i < PORT_NUM; ++i){
        for(int j = 0; j < N_FAN_IN; ++j){
            in_latch[i][j].valid = false;
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

    for(int i = 0; i < PORT_NUM; ++i){
        for(int j = 0; j < N_FAN_IN; ++j){
            in_list_to_tree[i][j] = &in_latch[i][j];        
        }
    }
    for(int i = 0; i < PORT_NUM; ++i){
        out_avail_to_tree[i] = &out_avail_latch[i];
    }
    


    for(int i = 0; i < PORT_NUM; ++i){
        tree_list[i].init(N_fan_in, i + 1, 4, FARTHEST_FIRST, 18, 6, 2, in_list_to_tree[i], out_avail_to_tree[i]);
    }
    

}

void crossbar::consume(){
    //latch all the in data
    for(int i = 0; i < PORT_NUM; ++i){
        for(int j = 0; j < N_FAN_IN; ++j){
            if(flit_in[j]->valid && flit_in[j]->dir_out == i + 1){
                in_latch[i][j] = *(flit[i]);
            }
        }
    }
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
        printf("dir is wrong\n");
        exit(-1);
    }
    if(port_id >= N_FAN_IN || port_id < 0){
        printf("port_id is wrong\n");
        exit(-1);
    }
    return tree_list[out_dir - 1].in_avail[port_id];
}

void crossbar_switch::free(){
    for(int i = 0; i < PORT_NUM; ++i){
        tree_list[i].free();
    }
}
