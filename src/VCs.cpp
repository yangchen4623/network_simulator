#include "VCs.h"
#include "define.h"
#include "flit.h"
#include "math.h"
#include "crossbar_switch.h"
void VCs::VCs_init(int Dir, flit* In, crossbar_switch* Sw){
    dir = Dir;
    in = In;
    sw = Sw;
    
    //init all in_latches
    for(int i = 0; i < VC_NUM; ++i){
        in_latch[i].valid = false;
    }
    for(int i = 0; i < VC_NUM; ++i){
        VC_array[i].fifo_init(VC_SIZE, &(in_latch[i]), &(out_avail_latch[i]));
    }
    in_avail = true; 
    for(int i = 0; i < VC_NUM; ++i){
        out[i].valid = false;
    }
    //init all the VC states to idle state
    for(int i = 0; i < VC_NUM; ++i){
        VC_state[i] = VC_IDLE:
    }
    grant = 0; // initially no grants
    return;
}

void VCs::consume(){
    //do the VC allocation first
    if(in->valid && (in->flit_type == HEAD_FLIT || in->flit_type == SINGLE_FLIT)){//allocate this flit to next available idle VC
        if(!in->VC_NUM){
            for(int i = 0; i < VC_NUM - 1; ++i){
                if(VC_state[i] == VC_IDLE){
                    grant = (1 << i);
                    break;
                }
            }
            if(i == VC_NUM - 1)
                grant = 0;
        }
        else{
            for(int i = 1; i < VC_NUM; ++i){
                if(VC_state[i] == VC_IDLE){
                    grant = (1 << i );
                    break;
                }
            }
            if(i == VC_NUM)
                grant = 0;
        }
    }
    if(grant != 0){
        int grant_index = (int)(log2(grant));
        if(in->valid && VC_array[grant_index].in_avail){
            //latch the in data
            in_latch[grant_index] = *in;
        }
    }

    //latch the out avail 
    for(int i = 0; i < VC_NUM; ++i){
        out_avail_latch[i] = sw->lookup_in_avail((dir - 1) * VC_NUM + i, dir);
    }

    //then call all the VC consume() function
    for(int i = 0; i < VC_NUM; ++i){
        VC_array[i].consume();
    }
    return;
}


void VCs::produce(){
    //update all the VC states
    for(int i = 0; i < VC_NUM; ++i){
        if(VC_state[i] == VC_IDLE){ //empty buffer and No packet is occupying VC  
            if(VC_array[i].in_latch.valid){
                if(VC_array[i].in_avail){
                    if(VC_array[i].in_latch.flit_type == HEAD_FLIT || VC_array[i].in_latch.flit_type == SINGLE_FLIT){
                        VC_state[i] = VC_WAITING_FOR_OVC;
                    }
                    else{
                        VC_state[i] = VC_ACTIVE;
                    }
                }
                else{
                    VC_state[i] = VC_WAITING_FOR_CREDITS;
                }
            }
            else{
                VC_state[i] = VC_IDLE;
            }
        }
        else if(VC_state[i] == VC_ACTIVE){ // the flit_out has a granted OVC, buffer might be empty
            if(VC_array[i].out_avail_latch){
                if(VC_array[i].usedw == 1){
                    if(VC_array[i].in_latch.valid && (VC_array[i].in_latch.flit_type == HEAD_FLIT || VC_array[i].in_latch.flit_type == SINGLE_FLIT)){
                        VC_state[i] = VC_WAITING_FOR_OVC;
                    }
                    else if(VC_array[i].in_latch.valid){
                        VC_state[i] = VC_ACTIVE;
                    }
                    else if(VC_array[i].flit_out.valid && (VC_array[i].flit_out.flit_type == TAIL_FLIT || VC_array[i].flit_out.flit_type == SINGLE_FLIT)){
                        VC_state[i] = VC_IDLE;
                    }
                    else{
                        VC_state[i] = VC_ACTIVE;
                    }
                }
                else{ //there are at least 2 flits in the VC
                    if(VC_array[i].flit_out.valid && (VC_array[i].flit_out.flit_type == TAIL_FLIT || VC_array[i].flit_out.flit_type == SINGLE_FLIT)){ // the next out flit will be a head flit
                        VC_state[i] = VC_WAITING_FOR_OVC;
                    }
                    else{
                        VC_state[i] = VC_ACTIVE;
                    }
                }
            }
            else{
                VC_state[i] = VC_ACTIVE;//downstream OVC has no credits
            }
            
        }
        else if(VC_state[i] == VC_WAITING_FOR_OVC){
            if(VC_array[i].out_avail_latch){
                VC_state[i] = VC_ACTIVE;
            }
            else{
                VC_state[i] = VC_WAITING_FOR_OVC;
            }
        }
    }
    //first call all the VC produce() functions
    for(int i = 0; i < VC_NUM; ++i){
        VC_array[i].produce();
    }
    //update out
    for(int i = 0; i < VC_NUM; ++i){
        out[i] = VC_array[i].out;
    }
    //
    //
    //update in_avail
    if(grant !=0){
        int grant_index = (int)(log2(grant));
        in_avail = VC_array[grant_index].in_avail;
    }
    else{
        in_avail = false;
    }
    //wipe all the in_latch
    for(int i = 0; i < VC_NUM; ++i){
        in_latch[i].valid = false;
    }
    return;

}

