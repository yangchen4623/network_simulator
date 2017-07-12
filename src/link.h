#ifdef LINK_H
#define LINK_H
#include "packet.h"
#include "phit.h"
#include "flit.h"
#include "node.h"
class link{
    private:
        int latency;
        int dir;// direction
        node* src_node;
        node* dst_node;
        flit* phit_list;
        flit in_latch; // this is a latched for 
    public:
        //default constructor
        flit* in;
        flit out;
        link();
        link(int Latency, int Dir, node* Src_node, node* Dst_node);

        void consume();
        void produce();

};

#endif
