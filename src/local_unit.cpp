#include "define.h"
#include "local_unit.h"
#include "pattern.h"
void local_unit::local_unit_init(int Cur_x, int Cur_y, int Cur_z, int Mode, int Injection_gap, int Packet_size, int Packet_num, flit** Eject, bool** Inject_avail){
    cycle_counter = 0;

    cur_x = Cur_x;
    cur_y = Cur_y;
    cur_z = Cur_z;

    mode = Mode;
    injection_gap = Injection_gap;
    packet_size = Packet_size;
    packet_num = Packet_num;
    for(int i = 0; i < PORT_NUM; ++i){
        eject[i] = Eject[i];
        inject_avail[i] = Inject_avail[i];
        eject_pckt_counter[i] = 0;
        eject_flit_counter[i] = 0;
        inject_pckt_counter[i] = 0;
        eject_flit_counter[i] = 0;
        eject_state[i] = EJECT_IDLE; 
    }
    all_pckt_rcvd = false;
}

void consume(){
    for(int i = 0; i < PORT_NUM; ++i){
        eject_latch[i] = *(eject[i]);
        inject_avail_latch[i] = *(inject_avail[i]);
    }

//check all the rcvd data


    for(int i = 0; i < PORT_NUM; ++i){
        if(eject_latch[i].valid){
            if(eject_latch[i].flit_type == HEAD_FLIT){
                if(eject_state[i] != EJECT_IDLE){
                    printf("error in local unit (%d,%d,%d): unexpected head flit from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x,
                }
                eject_flit_counter[i]++;
                cur_eject_src_x[i] = eject_latch[i].src_x;
                cur_eject_src_y[i] = eject_latch[i].src_y;

            }
            eject_flit_counter[i]++;
            if(eject_latch[i].flit_type == TAIL_FLIT || eject_latch[i].flit_type == SINGLE_FLIT){
                eject_pckt_counter[i]++;
    
            }
        }
    }

    int pckt_sum = 0;
    if(mode == INJECTION_NEAREST_NEIGHBOR){
        //check the eject
        for(int i = 0; i < PORT_NUM; ++i){
            
            pckt_sum += eject_pckt_counter[i];
        }
        if(pckt_sum == packet_num * 6){
            all_pckt_rcvd = true;
        }

    }
}

void produce(){
    for(int i = 0; i < PORT_NUM; ++i){
        if(inject_avail_latch[i]){
            inject_flit_counter[i] = (inject_flit_counter[i] <= injection_gap + packet_size - 1) ? inject_flit_counter[i] + 1 : 0;
            iF(inject_flit_counter[i] == injection_gap + packet_size){
                inject_pckt_counter[i]++;
            }
        }
    }
    for(int i = 0; i < PORT_NUM; ++i){
        if(packet_size == 1){
            inject[i].valid = true;
            inject[i].flit_type = SINGLE_FLIT;
            inject[i].packet_id = inject_pckt_counter[i];
            inject[i].src_x = cur_x;
            inject[i].src_y = cur_y;
            inject[i].src_z = cur_z;
        }    
    }
    cycle_counter++;
}
