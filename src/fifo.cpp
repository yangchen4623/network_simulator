#include "fifo.h"
#include <stdlib.h>
#include <stdio.h>
void fifo::fifo_init(int Size, flit* In, bool* Out_avail){
    size = Size;
    if(!(flit_array = (flit*)malloc(size * sizeof(flit)))){
        printf("no mem space for %d flits pointed by flit_array\n",size);
        exit(-1);
    }
	for (int i = 0; i < size; ++i){
		flit_array[i].valid = false;
	}
	in_latch.valid = false;
    head_ptr = 0;
    tail_ptr = 0;
    usedw = 0;
    in_avail = true;
    out.valid = false;
    in = In;
    out_avail = Out_avail;
}

void fifo::consume(){
    in_latch = *in;
    out_avail_latch = *out_avail;
}

void fifo::produce(){
    if(in_avail && in_latch.valid){
        flit_array[tail_ptr] = in_latch;
        tail_ptr = tail_ptr < size ? tail_ptr + 1 : 0;
    }
    if(out_avail_latch && out.valid){
        flit_array[head_ptr].valid = false;
        head_ptr = head_ptr < size ? head_ptr + 1 : 0;
    }

    out = flit_array[head_ptr];
    //update in_avail, usedw
    if(tail_ptr >= head_ptr){
        usedw = tail_ptr - head_ptr;
    }
    else{
        usedw = tail_ptr - head_ptr + size;
    }
    in_avail = usedw < (size - 1); 
}

void fifo::fifo_free(){
	free(flit_array);
}
