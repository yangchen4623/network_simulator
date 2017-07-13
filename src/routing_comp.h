#ifndef ROUTING_CONP_H
#define ROUTING_COMP_H
#include "define.h"
class routing_comp{
public: 
    char mode; 
    char cur_x;
    char cur_y;
    char cur_z;
    char dir_in;
    flit* in;
    flit in_latch;
    bool in_avail;
    bool out_avail_latch;
    bool* out_avail;
    flit out;
    flit flit_after_RC;
    flit flit_eject;
    bool eject_enable;

    void routing_comp_init(int Cur_x, int Cur_y, int Cur_z, int Dir_in, int mode, flit* In, bool* Out_avail);
    void consume();
    void produce();




}




#endif
