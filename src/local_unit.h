#ifndef LOCAL_UNIT_H
#define LOCAL_UNIT_H
#include "flit.h"
class local_unit{

    int cycle_counter = 0;
    flit inject[PORT_NUM];
    bool* inject_avail[PORT_NUM];
    flit* eject[PORT_NUM];
    
    bool inject_avail_latch[PORT_NUM];
    flit eject_latch[PORT_NUM];


    int cur_x;
    int cur_y;
    int cur_z;
    
    int mode;
    int injection_gap; // the number of clock cycles gap between 2 injection packets
    int packet_size;
    int packet_num;
    
    int inject_pckt_counter[i];
    int inject_flit_counter[i];
    int inject_control_counter[i];

    int eject_pckt_counter[i];
    int eject_flit_counter[i];
    int cur_eject_src_x[i];
    int cur_eject_src_y[i];
    int cur_eject_src_z[i];
    int cur_eject_pckt_id[i];

    int eject_state[i];

    bool all_pckt_rcvd = false;


    void local_unit_init(int Mode, int Injection_gap, int Packet_size, int Packet_num, flit** Eject, bool** Inject_avail);
    void produce();
    void consume();

};


#endif
