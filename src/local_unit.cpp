#include "define.h"
#include "local_unit.h"
#include "pattern.h"
#include<stdio.h>
#include<stdlib.h>
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
		inject_control_counter[i] = 0;
		inject_flit_counter[i] = 0;
		inject[i].valid = false;

    }
	credit_period_counter = 0;
    all_pckt_rcvd = false;
}

int local_unit::consume(){
	for (int i = 0; i < PORT_NUM; ++i){
		eject_latch[i] = *(eject[i]);
	}
	for (int i = 0; i < PORT_NUM; ++i){
		inject_avail_latch[i] = *(inject_avail[i]);
	}

        

//check all the rcvd data


    for(int i = 0; i < PORT_NUM; ++i){
        if(eject_latch[i].valid){
            if(eject_latch[i].flit_type == HEAD_FLIT){
                if(eject_state[i] != EJECT_IDLE){
                    printf("ejecting from dir: %d, error in local unit (%d,%d,%d): unexpected head flit from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d, whose age is %d\n",i, cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id, eject_latch[i].priority_age);
					return -1;
                }
                if(eject_latch[i].dst_x != cur_x || eject_latch[i].dst_y != cur_y || eject_latch[i].dst_z != cur_z){
                    printf("error in local unit (%d,%d,%d): head flit arrives wrong dst: from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(pattern[eject_latch[i].inject_dir - 1][eject_latch[i].src_z][eject_latch[i].src_y][eject_latch[i].src_x][eject_latch[i].packet_id].sent == false){
					printf("ejecting from dir: %d, error in local unit (%d,%d,%d): head flit has not been sent yet: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d, whose age is %d\n", i, cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id, eject_latch[i].priority_age);
					return -1;
                }
				if (eject_latch[i].flit_id != eject_flit_counter[i]){
					printf("error in local unit (%d,%d,%d): head flit id is not right: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d, expected id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id, eject_flit_counter[i]);
					return -1;
				}
                eject_flit_counter[i]++;
                eject_state[i] = EJECT_RECVING; 
                cur_eject_src_x[i] = eject_latch[i].src_x;
                cur_eject_src_y[i] = eject_latch[i].src_y;
                cur_eject_src_z[i] = eject_latch[i].src_z;
            }
            else if(eject_latch[i].flit_type == BODY_FLIT){
                if(eject_state[i] != EJECT_RECVING){
                    printf("ejecting from dir: %d, error in local unit (%d,%d,%d): unexpected body flit from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", i, cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(eject_latch[i].dst_x != cur_x || eject_latch[i].dst_y != cur_y || eject_latch[i].dst_z != cur_z){
                    printf("ejecting from dir: %d, error in local unit (%d,%d,%d): body flit arrives wrong dst: from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d, whose age is %d\n", i, cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id, eject_latch[i].priority_age);
					return -1;
                }
                if(eject_latch[i].src_x != cur_eject_src_x[i] || eject_latch[i].src_y != cur_eject_src_y[i] || eject_latch[i].src_z != cur_eject_src_z[i]){
                    printf("error in local unit (%d,%d,%d): body mismatch with cur head flit: from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(pattern[eject_latch[i].inject_dir - 1][eject_latch[i].src_z][eject_latch[i].src_y][eject_latch[i].src_x][eject_latch[i].packet_id].sent == false){
                    printf("error in local unit (%d,%d,%d): body flit has not been sent yet: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
				if (eject_latch[i].flit_id != eject_flit_counter[i]){
					printf("ejecting from dir: %d, error in local unit (%d,%d,%d): body flit id is not right: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d, expected id is %d, whose age is %d, inject_dir is %d\n\n", i, cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id, eject_flit_counter[i], eject_latch[i].priority_age, eject_latch[i].inject_dir);
					return -1;
				}
                eject_flit_counter[i]++;
            }
            else if(eject_latch[i].flit_type == TAIL_FLIT){
                if(eject_state[i] != EJECT_RECVING){
                    printf("error in local unit (%d,%d,%d): unexpected tail flit from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(eject_latch[i].dst_x != cur_x || eject_latch[i].dst_y != cur_y || eject_latch[i].dst_z != cur_z){
                    printf("error in local unit (%d,%d,%d): tail flit arrives wrong dst: from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(eject_latch[i].src_x != cur_eject_src_x[i] || eject_latch[i].src_y != cur_eject_src_y[i] || eject_latch[i].src_z != cur_eject_src_z[i]){
                    printf("error in local unit (%d,%d,%d): tail mismatch with cur head flit: from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(pattern[eject_latch[i].inject_dir - 1][eject_latch[i].src_z][eject_latch[i].src_y][eject_latch[i].src_x][eject_latch[i].packet_id].sent == false){
                    printf("error in local unit (%d,%d,%d): tail flit has not been sent yet: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(eject_flit_counter[i] + 1 != packet_size){
                    printf("error in local unit (%d,%d,%d): eject from %d, tail flit arrived but this packet is not complete: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d, arrived flits: %d, expected flits: %d, inject dir is %d\n", i, cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id, eject_flit_counter[i] + 1, packet_size, eject_latch[i].inject_dir);
					return -1;
                }
				if (eject_latch[i].flit_id != eject_flit_counter[i]){
					printf("error in local unit (%d,%d,%d): tail flit id is not right: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d, expected id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id, eject_flit_counter[i]);
					return -1;
				}
                eject_flit_counter[i] = 0;
                eject_pckt_counter[i]++;
                
                eject_state[i] = EJECT_IDLE;
                pattern[eject_latch[i].inject_dir - 1][eject_latch[i].src_z][eject_latch[i].src_y][eject_latch[i].src_x][eject_latch[i].packet_id].rcvd = true;
                pattern[eject_latch[i].inject_dir - 1][eject_latch[i].src_z][eject_latch[i].src_y][eject_latch[i].src_x][eject_latch[i].packet_id].recv_time_stamp = cycle_counter;
            }
            else if(eject_latch[i].flit_type == SINGLE_FLIT){
                if(eject_state[i] != EJECT_IDLE){
                    printf("error in local unit (%d,%d,%d): unexpected single flit from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(eject_latch[i].dst_x != cur_x || eject_latch[i].dst_y != cur_y || eject_latch[i].dst_z != cur_z){
                    printf("error in local unit (%d,%d,%d): single flit arrives wrong dst: from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(pattern[eject_latch[i].inject_dir - 1][eject_latch[i].src_z][eject_latch[i].src_y][eject_latch[i].src_x][eject_latch[i].packet_id].sent == false){
                    printf("error in local unit (%d,%d,%d): single flit has not been sent yet: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
                }
                if(packet_size != 1){
                    printf("error in local unit (%d,%d,%d): unexpected single flit, this pattern has not single flit, from (%d,%d,%d), whose dst is (%d,%d,%d), flit id is %d, packet id is %d\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
                }
				if (eject_latch[i].flit_id != 0){
					printf("error in local unit (%d,%d,%d): single flit id is not right: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d, expected id is 0\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);
					return -1;
				}
                eject_pckt_counter[i]++;
                pattern[eject_latch[i].inject_dir - 1][eject_latch[i].src_z][eject_latch[i].src_y][eject_latch[i].src_x][eject_latch[i].packet_id].rcvd = true;
                pattern[eject_latch[i].inject_dir - 1][eject_latch[i].src_z][eject_latch[i].src_y][eject_latch[i].src_x][eject_latch[i].packet_id].recv_time_stamp = cycle_counter;

            }
            else{
                printf("error in local unit (%d,%d,%d): wrong flit type: from (%d,%d,%d), whose dst is (%d,%d,%d), id is %d, packet id is %d,\n", cur_x, cur_y, cur_z, eject_latch[i].src_x, eject_latch[i].src_y, eject_latch[i].src_z, eject_latch[i].dst_x, eject_latch[i].dst_y, eject_latch[i].dst_z, eject_latch[i].flit_id, eject_latch[i].packet_id);    
            }
        }
    }

	return 0;

}

void local_unit::produce(){
	for (int i = 0; i < PORT_NUM; ++i){
		inject_avail_post_latch[i] = *(inject_avail[i]);
	}
	if (credit_period_counter < CREDIT_BACK_PERIOD)
		credit_period_counter++;
	else
		credit_period_counter = 0;

    for(int i = 0; i < PORT_NUM; ++i){

		//if (!inject_avail_latch[i] && inject_avail_post_latch[i] && (credit_period_counter != CREDIT_BACK_PERIOD)){
		if (inject_avail_latch[i]) {
			if (inject_control_counter[i] == packet_size && inject_avail_latch[i])
				inject_pckt_counter[i]++;
			inject_control_counter[i] = (inject_control_counter[i] <= injection_gap + packet_size) ? inject_control_counter[i] + 1 : 1;
		}
            if(inject_pckt_counter[i] < global_injection_packet_size[i][cur_z][cur_y][cur_x]){
				
                if(inject_control_counter[i] <= packet_size){ 
					if (inject_control_counter[i] == 1) {
						pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].sent = true;
						if (pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].rcvd){
							printf("this packet is already rcvd, error!: error in local unit (%d,%d,%d)\n", cur_x, cur_y, cur_z);
							exit(-1);
						}
						pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].rcvd = false;
						pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].send_time_stamp = cycle_counter;
					}

                    if(packet_size == 1){
						
                        inject[i].flit_type = SINGLE_FLIT;
                        inject[i].flit_id = 0;
						inject[i].packet_id = inject_pckt_counter[i];
						inject[i].dst_z = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_z;
						inject[i].dst_y = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_y;
						inject[i].dst_x = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_x;
						inject[i].priority_dist = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].mahattan_dist;
						inject[i].payload = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].payload;
						inject[i].O1TURN_id = -1;
             //           inject_pckt_counter[i]++;
        
                    }
                    else{
					//	pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].sent = true;
					//	pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].rcvd = false;
						
                        if(inject_control_counter[i] == 1){
                            inject[i].flit_type = HEAD_FLIT;
                            inject[i].flit_id = 0;
							inject[i].packet_id = inject_pckt_counter[i];
							inject[i].dst_z = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_z;
							inject[i].dst_y = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_y;
							inject[i].dst_x = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_x;
							inject[i].priority_dist = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].mahattan_dist;
							inject[i].payload = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].payload;
					//		pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].send_time_stamp = cycle_counter;
                        }
                        else if(inject_control_counter[i] > 1 && inject_control_counter[i] < packet_size){
                            inject[i].flit_type = BODY_FLIT;
                            inject[i].flit_id = inject_control_counter[i] - 1;
							inject[i].packet_id = inject_pckt_counter[i];
							inject[i].dst_z = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_z;
							inject[i].dst_y = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_y;
							inject[i].dst_x = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_x;
							inject[i].priority_dist = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].mahattan_dist;
							inject[i].payload = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].payload;
                        }
                        else if(inject_control_counter[i] == packet_size){
                            inject[i].flit_type = TAIL_FLIT;
                            inject[i].flit_id = inject_control_counter[i] - 1;      
							inject[i].packet_id = inject_pckt_counter[i];
							inject[i].dst_z = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_z;
							inject[i].dst_y = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_y;
							inject[i].dst_x = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].dst_x;
							inject[i].priority_dist = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].mahattan_dist;
							inject[i].payload = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].payload;
			//				inject_pckt_counter[i]++;

                        }

                    }
                    inject[i].valid = true;
                    inject[i].VC_class = (i >= 3);
                    
                    inject[i].inject_dir = i + 1;
                    inject[i].dir_out = i + 1;
                   
                    inject[i].priority_age = cycle_counter;

                    inject[i].src_x = cur_x;
                    inject[i].src_y = cur_y;
                    inject[i].src_z = cur_z;
					inject[i].O1TURN_id = -1;
                    //inject[i].packet_size = pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].packet_size;

                //log the injected packet

                    

                }
                else{
                    inject[i].valid = false;
                    
                }
				//if ((inject_avail_post_latch[i] || inject_avail_latch[i]) && (credit_period_counter != (CREDIT_BACK_PERIOD-2))){
//				if ((inject_avail_latch[i])) {
//					if (inject[i].valid  && (inject[i].flit_type == SINGLE_FLIT || inject[i].flit_type == HEAD_FLIT)){
//						pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].sent = true;
//						pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].rcvd = false;
//						pattern[i][cur_z][cur_y][cur_x][inject_pckt_counter[i]].send_time_stamp = cycle_counter;
//					}
//					if (inject[i].valid && (inject[i].flit_type == SINGLE_FLIT || inject[i].flit_type == TAIL_FLIT))
//						inject_pckt_counter[i]++;
//					inject_control_counter[i] = (inject_control_counter[i] <= injection_gap + packet_size - 1) ? inject_control_counter[i] + 1 : 0;
//				}



            
				
			}
			else
				inject[i].valid = false;
    }




    cycle_counter++;
}
