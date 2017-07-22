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

	if (!(out_avail_latch_to_fifos = (bool*)malloc(N_fan_in * sizeof(bool)))){
		printf("No mem space for %d slots for bool* out_avail_latch_to_fifos in reductor in %d dir, %d level, %d id\n", N_fan_in, out_dir, level, id);
		exit(-1);
	}


	if (!(in_slot = (flit*)malloc(N_fan_in * sizeof(flit)))){
		printf("No mem space for %d slots for flit* in_slot in reductor in %d dir, %d level, %d id\n", N_fan_in, out_dir, level, id);
		exit(-1);
	}
    
    if(!(in_Q_inst = (fifo*)malloc(N_fan_in * sizeof(fifo)))){
        printf("No mem space for %d slots(fifos) for fifo* in_Q_inst in reductor in %d dir, %d level, %d id\n", N_fan_in, out_dir, level, id);
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
   
		in_Q_inst[i].fifo_init(4, &(in_latch[i]), &(out_avail_latch_to_fifos[i]));
		in_avail[i] = in_Q_inst[i].in_avail;
		in_slot[i] = in_Q_inst[i].out;
  //      in_slot[i].valid = false;
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
		if (i == selector){
			out_avail_latch_to_fifos[i] = out_avail_latch;
		}
		else{
			out_avail_latch_to_fifos[i] = false;
		}
    }

	for (int i = 0; i < N_fan_in; ++i){
		in_Q_inst[i].consume();
	}
	
}

void N_to_1_reductor::produce(){
	for (int i = 0; i < N_fan_in; ++i)
		in_Q_inst[i].produce();

	for (int i = 0; i < N_fan_in; ++i){
		in_slot[i] = in_Q_inst[i].out;
	}

	int max = 0;
	//then decide who goes first
	if (!occupy){
		for (int i = 0; i < N_fan_in; ++i){
			int cur_priority;
			if (mode == SA_FARTHEST_FIRST){
				cur_priority = in_slot[i].priority_dist;
			}
			else{
				cur_priority = in_slot[i].priority_age;
			}//needs more code to implement mixed priority

			if (in_slot[i].valid && (in_slot[i].flit_type == HEAD_FLIT || in_slot[i].flit_type == SINGLE_FLIT) && cur_priority > max){
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

	for (int i = 0; i < N_fan_in; ++i){
		in_avail[i] = in_Q_inst[i].in_avail;
	}



/*	if (out_avail_latch){
		in_slot[selector].valid = false;
	}
	for (int i = 0; i < N_fan_in; ++i){
		if (!(in_slot[i].valid))
			in_slot[i] = in_latch[i];
	}


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


   */ 



    return;
}

void N_to_1_reductor::N_to_1_reductor_free(){
	for (int i = 0; i < N_fan_in; ++i)
		in_Q_inst[i].fifo_free();
	free(flit_in);
    free(in_latch);
    free(in_slot);
    free(in_avail);
	free(out_avail_latch_to_fifos);
    return;
}


