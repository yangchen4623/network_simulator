#include "node.h"

void node::node_init(int Cur_x, int Cur_y, int Cur_z, flit* In_xpos, flit* In_ypos, flit* In_zpos, flit* In_xneg, flit* In_yneg, flit* In_zneg){
    cur_x = Cur_x;
    cur_y = Cur_y;
    cur_z = Cur_z;

    in_xpos = In_xpos;
    in_ypos = In_ypos;
    in_zpos = In_zpos;
    in_xneg = In_xneg;
    in_yneg = In_yneg;
    in_zneg = In_zneg;

    for(int i = 0; i < PORT_NUM; ++i){
        in_latch_ptrs[i] = &(in_latch[i]);
        inject_latch_ptrs[i] = &(app_core.inject[i]);
        inject_avail_ptrs[i] = &(internal_router.out_avail_for_inject[i]);
        eject_ptrs[i] = &(internal_router.eject[i]);
    }

    internal_router.router_init(cur_x, cur_y, cur_z, SA_mode, routing_mode, in_latch_ptrs, inject_latch_ptrs);

    app_core.local_unit_init(injection_mode, INJECTION_GAP, PACKET_SIZE, PACKET_NUM, eject_ptrs, inject_avail_ptrs);

}
void node::consume(){
    in_latch[0] = *(in_xpos);
    in_latch[1] = *(in_ypos);
    in_latch[2] = *(in_zpos);
    in_latch[3] = *(in_xneg);
    in_latch[4] = *(in_yneg);
    in_latch[5] = *(in_zneg); 
    app_core.consume();
    router.consume();
}
void node::produce(){
    app_core.produce();
    router.produce();
}
