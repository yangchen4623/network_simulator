#ifndef LINK_H
#define LINK_H

#include "flit.h"
#include "node.h"
class link{
    public:
        int latency;
        int dir;// direction
        flit* in;
        flit in_latch;
        flit out;
        node* src;
        node* dst;


        flit* phit_list;



        void link_init(int Latency, int Dir, flit* In, node* Src, node* Dst);
        void consume();
        void produce();
        void link_free();

};

#endif
