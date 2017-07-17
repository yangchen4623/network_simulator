#include "local_unit.h"
void local_unit::local_unit_init(int Mode, int Injection_gap, int Packet_size, int Packet_num){
    mode = Mode;
    injection_gap = Injection_gap;
    packet_size = Packet_size;
    packet_num = Packet_num;
    for(int i = 0; i < PORT_NUM; ++i){
        eject[i] = Eject[i];
        inject_avail[i] = Inject_avail[i];
    }
}

void produce(){
    if(mode == INJECTION_NEAREST_NEIGHBOR){
        
    }
}
