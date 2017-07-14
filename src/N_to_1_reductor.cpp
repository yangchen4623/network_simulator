#include "N_to_1_reductor.h"

void N_to_1_reductor::N_to_1_reductor_init(int N_Fan_in, int Our_dir, int Level, int Id, int Mode, flit** In_list, bool* Out_avail){   
    //init all the arrays
    N_fan_in = N_Fan_in;
    out_dir = Out_dir;
    level = Level;
    id = Id;
    selector = 0;
    occupy = false;
    mode = Mode;
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
    
    for(int i = 0; i < N_fan_in; ++i){
        flit_in[i] = In_list[i];
        in_latch[i].valid = false;
        in_avail[i] = true;
        in_slot[i].valid = false;
    }

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
    //remove the selected data if out_avail
    if(out_avail){
        if(in_slot[selector].valid){
            in_slot[selector].valid = 
        }
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
    out = 

    

    
    

    
      
}


