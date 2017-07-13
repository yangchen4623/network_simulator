#ifndef FIFO_H
#define FIFO_H
#include "flit.h"
class fifo(
public:
    flit* in;
    flit in_latch;
    bool out_avail_latch;
    bool in_avail;
    bool* out_avail;
    flit out;
    flit* flit_array;
    int size;
    int head_ptr;
    int tail_ptr;
    int usedw;

    void fifo_init(int Size, flit* In, bool* Out_avail);
    void consume();
    void produce();
    void fifo_free();

    
);



#endif
