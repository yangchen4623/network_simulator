#include "define.h"
#include "N_to_1_reductor.h"
#include<stdlib.h>
#include<stdio.h>

void N_to_1_reductor::alloc(int N_Fan_in){
    N_fan_in = N_Fan_in;
    if(!(flit_in = (flit**)malloc(N_fan_in * sizeof(flit*)))){
        printf("No mem space for %d slots for flit** flit_in in reductor in %d dir, %d level, %d id\n", N_fan_in, out_dir, level, id);
        exit(-1);
    }
    
    if(!(in_latch = (flit*)malloc(N_fan_in * sizeof(flit)))){
        printf("No mem space for %d slots for flit* in_latch in reductor in %d dir, %d level, %d id\n", N_fan_in, out_dir, level, id);
        exit(-1);
    }   

    if(!(in_avail = (bool*)malloc(N_fan_in * sizeof(bool)))){
        printf("No mem space for %d slots for bool* in_avail in reductor in %d dir, %d level, %d id\n", N_fan_in, out_dir, level, id);
        exit(-1);
    }
    
    if(!(in_slot = (flit*)malloc(N_fan_in * sizeof(flit)))){
        printf("No mem space for %d slots for N_fan_in* in_slot in reductor in %d dir, %d level, %d id\n", N_fan_in, out_dir, level, id);
        exit(-1);
    }

    

}

void N_to_1_reductor::N_to_1_reductor_init(int Out_dir, int Level, int Id, int Mode, flit** In_list, bool* Out_avail){   
    //init all the arrays

    out_dir = Out_dir;
    level = Level;
    id = Id;
    selector = 0;
    occupy = false;
    mode = Mode;

    for(int i = 0; i < N_fan_in; ++i){
        flit_in[i] = In_list[i];
        in_latch[i].valid = false;
        in_avail[i] = true;
        in_slot[i].valid = false;
    }
	out_avail = Out_avail;
	out.valid = false;

    return;
}

void N_to_1_reductor::consume(){
    //latch the out avail
    out_avail_latch = *out_avail;
    //latch the flit in
    for(int i = 0; i < N_fan_in; ++i){
        in_latch[i] = *(flit_in[i]);
    }
}

void N_to_1_reductor::produce(){
    //determine the in_avail first
    if(out_avail_latch){
        for(int i = 0; i < N_fan_in; ++i){
            if(i == selector)
                in_avail[i] = true;
            else
                in_avail[i] = !(in_slot[i].valid);
        }       
    }
    else{
        for(int i = 0; i < N_fan_in; ++i){
            in_avail[i] = !(in_slot[i].valid);
        }
    }
    //remove the selected data if out_avail
    for(int i = 0; i < N_fan_in; ++i){
        if(in_avail[i])
            in_slot[i] = in_latch[i];
    }


    int max = 0;
    //then decide who goes first
    if(!occupy){
        for(int i = 0; i < N_fan_in; ++i){
            int cur_priority;
            if(mode == SA_FARTHEST_FIRST){
                cur_priority = in_slot[i].priority_dist;
            }
            else{
                cur_priority = in_slot[i].priority_age;
            }//needs more code to implement mixed priority

            if(in_slot[i].valid && (in_slot[i].flit_type == HEAD_FLIT || in_slot[i].flit_type == SINGLE_FLIT) && cur_priority > max){
                selector = i;
                max = cur_priority;
            }
        }
    }
    //then produce out 
    out = in_slot[selector];
	if (out.valid && (out.flit_type == HEAD_FLIT || out.flit_type == BODY_FLIT))
		occupy = true;
	else if (out.valid && out.flit_type == TAIL_FLIT)
		occupy = false;
    return;
}

void N_to_1_reductor::N_to_1_reductor_free(){
    free(flit_in);
    free(in_latch);
    free(in_slot);
    free(in_avail);
    return;
}


