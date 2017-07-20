//a set of VCs associated with a input port
//The nunmber of VCs is determined by macro VC_NUM
//This module is connected to the routing comp of one input port
//The VC class bit controls the set of VC class to use
//  when VC class bit is 0, this flit is allocated to VC[0 : VC_NUM - 2]
//  when VC class bit is 1, this flit is allocated to VC[1 : VC_NUM - 1]
//
#ifndef VCS_H
#define VCS_H
#include "define.h"
#include "flit.h"
#include "fifo.h"
#include "crossbar_switch.h"
class VCs{
public:
    flit* in;
    flit in_latch[VC_NUM];
    int dir;
    int grant; // one-hot code indicate which VC is being allocated to the incoming flit
    bool in_avail;
    bool out_avail_latch[VC_NUM];
    
    crossbar_switch* sw;
    
    flit out[VC_NUM];
    fifo VC_array[VC_NUM];
    int VC_state[VC_NUM];
    void VCs_init(int Dir, flit* In, crossbar_switch* Sw);
    void consume();
    void produce();
	void VCs_free();

};

#endif
