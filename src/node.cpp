#include "node.h"

void node::node_init(int Cur_x, int Cur_y, int Cur_z, flit* In_xpos, flit* In_ypos, flit* In_zpos, flit* In_xneg, flit* In_yneg, flit* In_zneg, int Injection_mode, int Routing_mode, int Sa_mode, int Injection_gap, int Packet_size){
    cur_x = Cur_x;
    cur_y = Cur_y;
    cur_z = Cur_z;

    in_xpos = In_xpos;
    in_ypos = In_ypos;
    in_zpos = In_zpos;
    in_xneg = In_xneg;
    in_yneg = In_yneg;
    in_zneg = In_zneg;

	out_xpos.valid = false;
	out_ypos.valid = false; 
	out_zpos.valid = false;
	out_xneg.valid = false;
	out_yneg.valid = false;
	out_zneg.valid = false;

	injection_mode = Injection_mode;
	routing_mode = Routing_mode;
	SA_mode = Sa_mode;

    for(int i = 0; i < PORT_NUM; ++i){
		in_latch[i].valid = false;
        in_latch_ptrs[i] = &(in_latch[i]);
//        inject_latch_ptrs[i] = &(app_core.inject[i]);
        inject_avail_ptrs[i] = &(internal_router.out_avail_for_inject[i]);
        eject_ptrs[i] = &(internal_router.eject[i]);
    }

	internal_router.router_init(cur_x, cur_y, cur_z, SA_mode, routing_mode, injection_mode, in_latch_ptrs, inject_latch_ptrs, Injection_gap, Packet_size);

//    app_core.local_unit_init(cur_x, cur_y, cur_z, injection_mode, INJECTION_GAP, PACKET_SIZE, PACKET_NUM, eject_ptrs, inject_avail_ptrs);

}
int node::consume(){
    in_latch[0] = *(in_xpos);
    in_latch[1] = *(in_ypos);
    in_latch[2] = *(in_zpos);
    in_latch[3] = *(in_xneg);
    in_latch[4] = *(in_yneg);
    in_latch[5] = *(in_zneg); 
//    app_core.consume();
    if(internal_router.consume() == -1)
		return -1;
	return 0;
}
int node::produce(){
	if (internal_router.produce() == -1)
		return -1;
//    app_core.produce();

	out_xpos = internal_router.out[0]; 
	out_ypos = internal_router.out[1];
	out_zpos = internal_router.out[2];
	out_xneg = internal_router.out[3];
	out_yneg = internal_router.out[4];
	out_zneg = internal_router.out[5];
	return 0;
}

void node::node_free(){
	internal_router.router_free();
}
