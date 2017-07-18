#ifdef LINK_H
#define LINK_H
#include "packet.h"
#include "phit.h"
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
        flit in_latch; // this is a latched for 

        flit out;
        void link_init(int Latency, int Dir, flit* In, node* Src, node* Dst);
        void consume();
        void produce();
        void free();

};

#endif
