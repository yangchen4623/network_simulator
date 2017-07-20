#ifndef ROUTER_H
#define ROUTER_H
#include "flit.h"
#include "define.h"
#include  "fifo.h"
#include "routing_comp.h"
#include "VCs.h"
class router{
public:

    int cur_x;
    int cur_y;
    int cur_z;

    flit* in[PORT_NUM];
    flit* inject[PORT_NUM];

    flit in_latch[PORT_NUM];
    flit inject_latch[PORT_NUM];

    flit out[PORT_NUM];
    flit eject[PORT_NUM];

    bool inject_avail[PORT_NUM];

    int upstream_credits[PORT_NUM];
    int downstream_credits[PORT_NUM];
    int credit_period_counter;

    //input buffers
    fifo input_buffer_list[PORT_NUM];

    flit* flit_list_to_RC[PORT_NUM];
    bool* in_avail_from_RC[PORT_NUM];
    //route compute
    routing_comp RC_list[PORT_NUM];

    flit* flit_list_to_VA[PORT_NUM];
    bool* in_avail_from_VA[PORT_NUM];
    //VCs
    VCs VCs_list[PORT_NUM];

    flit* flit_list_to_SA[PORT_NUM * VC_NUM];
    bool* in_avail_from_SA[PORT_NUM * VC_NUM];
    //crossbar switch
    crossbar_switch xbar;
    
    flit* flit_list_to_ST[PORT_NUM * VC_NUM];
    bool* in_avail_from_ST[PORT_NUM];
    
    bool out_avail_for_passthru[PORT_NUM];
    bool out_avail_for_inject[PORT_NUM];
    bool downstream_avail[PORT_NUM];
    //the availability for bypass traffic to use the out link
    
    int SA_mode;
    int routing_mode;

    //
    bool occupy_by_inject[PORT_NUM]; //bool value denoting the out port is occupy the inject traffic
    
    void router_init(int Cur_x, int Cur_y, int Cur_z, int SA_Mode, int Routing_mode, flit** In, flit** Inject);

    void consume();
    void produce();
    void router_free();

    

};

    


#endif
