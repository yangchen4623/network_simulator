#include "reduction_tree.h"
#include "N_to_1_reductor.h"
#include "flit.h"
#include <stdlib.h>
#include <stdio.h>
void reduction_tree::reduction_tree_init(int N_Fan_in, int Out_dir, int Level_num, int Mode, int L1_N, int L2_N, int L3_N, flit** In_list, bool* Out_avail){
    N_fan_in = N_Fan_in;
	out_dir = Out_dir;
    level_num = Level_num;
    mode = Mode;
    l1_N = L1_N;
    l2_N = L2_N;
    l3_N = L3_N;
    l4_N = 1;
    if(!(flit_in = (flit**)malloc(N_fan_in * sizeof(flit*)))){
		printf("No mem space for %d slots for flit** flit_in in reductor tree in %d dir\n", N_fan_in, out_dir);
        exit(-1);
    }
    
    if(!(in_latch = (flit*)malloc(N_fan_in * sizeof(flit)))){
        printf("No mem space for %d slots for flit* in_latch in reductor in %d dir\n", N_fan_in, out_dir);
        exit(-1);
    }   

    if(!(in_avail = (bool*)malloc(N_fan_in * sizeof(bool)))){
        printf("No mem space for %d slots for bool* in_avail in reductor in %d dir\n", N_fan_in, out_dir);
        exit(-1);
    }
    
    if(!(in_slot = (flit*)malloc(N_fan_in * sizeof(flit)))){
        printf("No mem space for %d slots for N_fan_in* in_slot in reductor in %d dir\n", N_fan_in, out_dir);
        exit(-1);
    }

    for(int i = 0; i < N_fan_in; ++i){
        flit_in[i] = In_list[i];
        in_latch[i].valid = false;
        in_avail[i] = true;
        in_slot[i].valid = false;
    }
    out_avail_latch = true;
    out_avail = Out_avail;
	out.valid = false;

    //then init all the reductors in l1 and l2 and l3
    if(!(l1_reductors = (N_to_1_reductor*)malloc(l1_N * sizeof(N_to_1_reductor)))){
        printf("No mem space for %d l1 reductors for reduction tree on dir %d\n", l1_N, out_dir);
        exit(-1);
    }
    if(!(l2_reductors = (N_to_1_reductor*)malloc(l2_N * sizeof(N_to_1_reductor)))){
        printf("No mem space for %d l2 reductors for reduction tree on dir %d\n", l2_N, out_dir);
        exit(-1);
    } 
    if(!(l3_reductors = (N_to_1_reductor*)malloc(l3_N * sizeof(N_to_1_reductor)))){
        printf("No mem space for %d l2 reductors for reduction tree on dir %d\n", l2_N, out_dir);
        exit(-1);
    }
    
    //then call init functions of all the reductors
    int l1_W = N_fan_in / l1_N;
    int l2_W = l1_N / l2_N;
    int l3_W = l2_N / l3_N;
    int l4_W = l3_N;
    


    for(int i = 0; i < l1_N; ++i){
		l1_reductors[i].alloc(l1_W);
    }
    for(int i = 0; i < l2_N; ++i){
        l2_reductors[i].alloc(l2_W);
    }
    for(int i = 0; i < l3_N; ++i){
        l3_reductors[i].alloc(l3_W);
    }
    l4_reductor.alloc(l4_W);

    if(!(flit_to_l1 = (flit**)malloc(N_fan_in * sizeof(flit*)))){
        printf("No mem space of %d pointers for to l1 reductors\n", N_fan_in);
        exit(-1);
    }
  
    if(!(flit_l1_to_l2 = (flit**)malloc(l1_N * sizeof(flit*)))){
        printf("No mem space of %d pointers for l1 to l2 reductors\n", l1_N);
        exit(-1);
    }

    if(!(flit_l2_to_l3 = (flit**)malloc(l2_N * sizeof(flit*)))){
        printf("No mem space of %d pointers for l2 to l3 reductors\n", l2_N);
        exit(-1);
    }

    if(!(flit_l3_to_l4 = (flit**)malloc(l3_N * sizeof(flit*)))){
        printf("No mem space of %d pointers for l3 to l4 reductors\n", l3_N);
        exit(-1);
    }

    if(!(in_avail_l2_to_l1 = (bool**)malloc(l1_N * sizeof(bool*)))){
		printf("No mem space for %d pointers for l2 to l1 in_avail\n", l1_N);
        exit(-1);
    }
 
    if(!(in_avail_l3_to_l2 = (bool**)malloc(l2_N * sizeof(bool*)))){
		printf("No mem space for %d pointers for l3 to l2 in_avail\n", l2_N);
        exit(-1);
    }
 
    if(!(in_avail_l4_to_l3 = (bool**)malloc(l3_N * sizeof(bool*)))){
		printf("No mem space for %d pointers for l3 to l2 in_avail\n", l3_N);
        exit(-1);
    }

    if(!(out_avail_to_l4 = (bool*)malloc(sizeof(bool)))){
        printf("No mem space for 1 pointer for to l4 out_avail\n");
        exit(-1);
    }

    for(int i = 0; i < N_fan_in; ++i){
        flit_to_l1[i] = &in_latch[i];
    }

    for(int i = 0; i < l1_N; ++i){
        flit_l1_to_l2[i] = &(l1_reductors[i].out);
        in_avail_l2_to_l1[i] = &(l2_reductors[i / l2_W].in_avail[i % l2_W]);
    }
    for(int i = 0; i < l2_N; ++i){
        flit_l2_to_l3[i] = &(l2_reductors[i].out);
        in_avail_l3_to_l2[i] = &(l3_reductors[i / l3_W].in_avail[i % l3_W]);
    }
    for(int i = 0; i < l3_N; ++i){
        flit_l3_to_l4[i] = &(l3_reductors[i].out);
        in_avail_l4_to_l3[i] = &(l4_reductor.in_avail[i]);
    }

    out_avail_to_l4 = &out_avail_latch; 
    
    for(int i = 0; i < l1_N; ++i){
        l1_reductors[i].N_to_1_reductor_init(out_dir, 1, i, mode, &(flit_to_l1[i * l1_W]), in_avail_l2_to_l1[i]);
    }
    for(int i = 0; i < l2_N; ++i){
		l2_reductors[i].N_to_1_reductor_init(out_dir, 2, i, mode, &(flit_l1_to_l2[i * l2_W]), in_avail_l3_to_l2[i]);
    }
    for(int i = 0; i < l3_N; ++i){
		l3_reductors[i].N_to_1_reductor_init(out_dir, 3, i, mode, &(flit_l2_to_l3[i * l3_W]), in_avail_l4_to_l3[i]);
    }   

	l4_reductor.N_to_1_reductor_init(out_dir, 4, 0, mode, flit_l3_to_l4, out_avail_to_l4);

}

void reduction_tree::consume(){
    //latch the out_avail
    out_avail_latch = *out_avail;
    //latch the flit_in
    for(int i = 0; i < N_fan_in; ++i){
        in_latch[i] = *(flit_in[i]);
    }

    //call all the subentities consume
    for(int i = 0; i < l1_N; ++i){
        l1_reductors[i].consume();
    }
    for(int i = 0; i < l2_N; ++i){
        l2_reductors[i].consume();
    }
    for(int i = 0; i < l3_N; ++i){
        l3_reductors[i].consume();
    }
    l4_reductor.consume();

}

void reduction_tree::produce(){
     //call all the subentities produce()
    for(int i = 0; i < l1_N; ++i){
        l1_reductors[i].produce();
    }
    for(int i = 0; i < l2_N; ++i){
        l2_reductors[i].produce();
    }
    for(int i = 0; i < l3_N; ++i){
        l3_reductors[i].produce();
    }
    l4_reductor.produce();

    //update in_avail and out
    for(int i = 0; i < N_fan_in; ++i){
        in_avail[i] = l1_reductors[N_fan_in / l1_W].in_avail[N_fan_in % l1_W];
    }
    out = l4_reductor.out;
}
void reduction_tree::reduction_tree_free(){
	for(int i = 0; i < l1_N; ++i){
        l1_reductors[i].N_to_1_reductor_free();
    }
    for(int i = 0; i < l2_N; ++i){
		l2_reductors[i].N_to_1_reductor_free();
    }
    for(int i = 0; i < l3_N; ++i){
		l3_reductors[i].N_to_1_reductor_free();
    }
	l4_reductor.N_to_1_reductor_free();
    free(l1_reductors);
    free(l2_reductors);
    free(l3_reductors);

    free(flit_to_l1);
    free(flit_l1_to_l2);
    free(flit_l2_to_l3);
    free(flit_l3_to_l4);
    free(in_avail_l2_to_l1);
    free(in_avail_l3_to_l2);
    free(in_avail_l4_to_l3);
    free(out_avail_to_l4);
    
    free(flit_in);
    free(in_latch);
    free(in_avail);
    free(in_slot);

    return;

}






