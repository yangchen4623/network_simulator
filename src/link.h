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
        node* src_node;
        node* dst_node;
        flit* phit_list;
        flit in_latch; // this is a latched for 

        flit out;
        link();
        link(int Latency, int Dir, node* Src_node, node* Dst_node);
        void link_init();
        void consume();
        void produce();
        void link_free();

};

#endif
