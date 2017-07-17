#ifndef LOCAL_UNIT_H
#define LOCAL_UNIT_H
#include "flit.h"
class local_unit{
    flit inject[PORT_NUM];
    bool* inject_avail[PORT_NUM];
    flit* eject[PORT_NUM];
    
    bool inject_avail_latch[PORT_NUM];



    int mode;
    int injection_gap; // the number of clock cycles gap between 2 injection packets
    int packet_size;
    int packet_num;
    
    int inject_pckt_counter;
    int inject_flit_counter;

    int eject_pckt_counter;
    int eject_flit_counter;

    fifo inject_buffer[PORT_NUM];

    void local_unit_init(int Mode, int Injection_gap, int Packet_size, int Packet_num, flit** Eject, bool** Inject_avail);
    void produce();
    void consume();

};


#endif
