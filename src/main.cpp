#include "network.h"
#include "pattern.h"
#include "define.h"
#include "packet.h"
#include <stdlib.h>
#include <stdio.h>

int global_injection_packet_size[PORT_NUM][ZSIZE][YSIZE][XSIZE];
packet* pattern[PORT_NUM][ZSIZE][YSIZE][XSIZE];

int total_packet_sent = 0;
void gen_pattern_nearest_neighbor(int pattern_size){
    //pattern size means the number of packets of each pair of nodes in the pattern
    total_packet_sent = 0;
    for(int i = 0; i < PORT_NUM; ++i){
        for(int j = 0; j < ZSIZE; ++j){
            for(int k = 0; k < YSIZE; ++k){
                for(int m = 0; m < XSIZE; ++m){
                    if(!(pattern[i][j][k][m] = (packet*)malloc(pattern_size * sizeof(packet)))){
                        printf("error when allocating space for pattern\n");
                        exit(-1);
                    }
                    global_injection_packet_size[i][j][k][m] = pattern_size;
                    for(int n = 0; n < pattern_size; ++n){
                        pattern[i][j][k][m][n].valid = true;
						total_packet_sent++;
                        pattern[i][j][k][m][n].src_x = m;
                        pattern[i][j][k][m][n].src_y = k;
                        pattern[i][j][k][m][n].src_z = j;
                        pattern[i][j][k][m][n].id = n;
                        if(i == 0){
                            pattern[i][j][k][m][n].dst_x = m + 1 >= XSIZE ? 0 : m + 1;
                            pattern[i][j][k][m][n].dst_y = k;
                            pattern[i][j][k][m][n].dst_z = j;
                        }
                        else if(i == 1){
                            pattern[i][j][k][m][n].dst_x = m;
                            pattern[i][j][k][m][n].dst_y = k + 1 >= YSIZE ? 0 : k + 1;
                            pattern[i][j][k][m][n].dst_z = j;
                        }
                        else if(i == 2){
                            pattern[i][j][k][m][n].dst_x = m;
                            pattern[i][j][k][m][n].dst_y = k;
                            pattern[i][j][k][m][n].dst_z = j + 1 >= ZSIZE ? 0 : j + 1;
                        }
                        else if(i == 3){
                            pattern[i][j][k][m][n].dst_x = m != 0 ? m - 1 : XSIZE - 1;
                            pattern[i][j][k][m][n].dst_y = k;
                            pattern[i][j][k][m][n].dst_z = j;
                        }
                        else if(i == 4){
                            pattern[i][j][k][m][n].dst_x = m;
                            pattern[i][j][k][m][n].dst_y = k != 0 ? k - 1 : YSIZE - 1;
                            pattern[i][j][k][m][n].dst_z = j;
                        }
                        else if(i == 5){
                            pattern[i][j][k][m][n].dst_x = m;
                            pattern[i][j][k][m][n].dst_y = k;
                            pattern[i][j][k][m][n].dst_z = j != 0 ? j - 1 : ZSIZE - 1;
                        }
                        pattern[i][j][k][m][n].mahattan_dist = 1;
                        pattern[i][j][k][m][n].sent = false;
                        pattern[i][j][k][m][n].rcvd = false;
                        pattern[i][j][k][m][n].payload = n;
                        
                    }
                }
            }
        }
        
    }
}

void gen_pattern_three_hop_diagonal(int pattern_size){
	total_packet_sent = 0;
	for (int i = 0; i < PORT_NUM; ++i){
		for (int j = 0; j < ZSIZE; ++j){
			for (int k = 0; k < YSIZE; ++k){
				for (int m = 0; m < XSIZE; ++m){
					if (!(pattern[i][j][k][m] = (packet*)malloc(2 * pattern_size * sizeof(packet)))){
						printf("error when allocating space for pattern\n");
						exit(-1);
					}
					if (i <= 3)
						global_injection_packet_size[i][j][k][m] = 2 * pattern_size;
					else
						global_injection_packet_size[i][j][k][m] = 0;
					for (int n = 0; n < 2 * pattern_size; ++n){
						
						pattern[i][j][k][m][n].src_x = m;
						pattern[i][j][k][m][n].src_y = k;
						pattern[i][j][k][m][n].src_z = j;
						pattern[i][j][k][m][n].id = n;
						if (i == 0){
							pattern[i][j][k][m][n].valid = true;
							total_packet_sent++;
							if (n % 2 == 0){
								pattern[i][j][k][m][n].dst_x = m + 1 >= XSIZE ? 0 : m + 1;
								pattern[i][j][k][m][n].dst_y = k + 1 >= YSIZE ? 0 : k + 1;
								pattern[i][j][k][m][n].dst_z = j + 1 >= ZSIZE ? 0 : j + 1;
							}
							else{
								pattern[i][j][k][m][n].dst_x = m + 1 >= XSIZE ? 0 : m + 1;
								pattern[i][j][k][m][n].dst_y = k != 0 ? k - 1 : YSIZE - 1;
								pattern[i][j][k][m][n].dst_z = j != 0 ? j - 1 : ZSIZE - 1;
							}
						}
						else if (i == 1){
							pattern[i][j][k][m][n].valid = true;
							total_packet_sent++;
							if (n % 2 == 0){
								pattern[i][j][k][m][n].dst_x = m + 1 >= XSIZE ? 0 : m + 1;
								pattern[i][j][k][m][n].dst_y = k + 1 >= YSIZE ? 0 : k + 1;
								pattern[i][j][k][m][n].dst_z = j != 0 ? j - 1 : ZSIZE - 1;
							}
							else{
								pattern[i][j][k][m][n].dst_x = m != 0 ? m - 1 : XSIZE - 1;
								pattern[i][j][k][m][n].dst_y = k + 1 >= YSIZE ? 0 : k + 1;
								pattern[i][j][k][m][n].dst_z = j != 0 ? j - 1 : ZSIZE - 1;
							}
						}
						else if (i == 2){
							pattern[i][j][k][m][n].valid = true;
							total_packet_sent++;
							if (n % 2 == 0){
								pattern[i][j][k][m][n].dst_x = m != 0 ? m - 1 : XSIZE - 1;
								pattern[i][j][k][m][n].dst_y = k != 0 ? k - 1 : YSIZE - 1;
								pattern[i][j][k][m][n].dst_z = j + 1 >= ZSIZE ? 0 : j + 1;
							}
							else{
								pattern[i][j][k][m][n].dst_x = m + 1 >= XSIZE ? 0 : m + 1;
								pattern[i][j][k][m][n].dst_y = k != 0 ? k - 1 : YSIZE - 1;
								pattern[i][j][k][m][n].dst_z = j + 1 >= ZSIZE ? 0 : j + 1;
							}
						}
						else if (i == 3){
							pattern[i][j][k][m][n].valid = true;
							total_packet_sent++;
							if (n % 2 == 0){
								pattern[i][j][k][m][n].dst_x = m != 0 ? m - 1 : XSIZE - 1;
								pattern[i][j][k][m][n].dst_y = k != 0 ? k - 1 : YSIZE - 1;
								pattern[i][j][k][m][n].dst_z = j != 0 ? j - 1 : ZSIZE - 1;
							}
							else{
								pattern[i][j][k][m][n].dst_x = m != 0 ? m - 1 : XSIZE - 1;
								pattern[i][j][k][m][n].dst_y = k + 1 >= YSIZE ? 0 : k + 1;
								pattern[i][j][k][m][n].dst_z = j + 1 >= ZSIZE ? 0 : j + 1;
							}
						}
						else{
							pattern[i][j][k][m][n].valid = false;
						}
						pattern[i][j][k][m][n].mahattan_dist = 3;
						pattern[i][j][k][m][n].sent = false;
						pattern[i][j][k][m][n].rcvd = false;
						pattern[i][j][k][m][n].payload = n;

					}
				}
			}
		}

	}

}


bool count_sent_and_rcvd(){
    int cur_sent_num = 0;
    int cur_rcvd_num = 0;
    for(int i = 0; i < PORT_NUM; ++i){
		for (int j = 0; j < ZSIZE; ++j){
            for(int k = 0; k < YSIZE; ++k){
                for(int m = 0; m < XSIZE; ++m){
                    for(int n = 0; n < global_injection_packet_size[i][j][k][m]; ++n){
                        if(pattern[i][j][k][m][n].valid && pattern[i][j][k][m][n].sent)
                            cur_sent_num++;
                        if(pattern[i][j][k][m][n].valid && pattern[i][j][k][m][n].rcvd)
                            cur_rcvd_num++;
                    }
                }
            }
        }    
    }
    printf("total %d pckts, sent %d pckts, rcvd %d pckts\n", total_packet_sent, cur_sent_num, cur_rcvd_num);
    return cur_rcvd_num == total_packet_sent;
}

void print_stats(){
    int packet_counter = 0;
    float avg_latency = 0;
    int worst_case_latency = 0;
	packet worst_case_packet;
    for(int i = 0; i < PORT_NUM; ++i){
		for (int j = 0; j < ZSIZE; ++j){
            for(int k = 0; k < YSIZE; ++k){
                for(int m = 0; m < XSIZE; ++m){
                    for(int n = 0; n < global_injection_packet_size[i][j][k][m]; ++n){
                        if(pattern[i][j][k][m][n].valid && pattern[i][j][k][m][n].sent && pattern[i][j][k][m][n].rcvd){
                            avg_latency = (avg_latency * packet_counter + pattern[i][j][k][m][n].recv_time_stamp - pattern[i][j][k][m][n].send_time_stamp) / (float)(packet_counter + 1);
							if (worst_case_latency < pattern[i][j][k][m][n].recv_time_stamp - pattern[i][j][k][m][n].send_time_stamp){
								worst_case_latency = pattern[i][j][k][m][n].recv_time_stamp - pattern[i][j][k][m][n].send_time_stamp;
								worst_case_packet = pattern[i][j][k][m][n];
							}
                            packet_counter++;
                        }
                    }
                }
            }
        }    
    }
	printf("among %d packets, avg latency is %f cycles, worst case packet takes %d cycles\n", packet_counter, avg_latency, worst_case_latency);
	printf("worst packet from (%d, %d, %d) to (%d, %d, %d), packet id is %d\n", worst_case_packet.src_x, worst_case_packet.src_y, worst_case_packet.src_z, worst_case_packet.dst_x, worst_case_packet.dst_y, worst_case_packet.dst_z, worst_case_packet.id);
}


int main(){
    int cycle_counter = 0;
    int pattern_size = 10;
	gen_pattern_three_hop_diagonal(pattern_size);

    network network_UUT;
    network_UUT.network_init(XSIZE, YSIZE, ZSIZE);
    while(1){
        network_UUT.consume();
		network_UUT.produce();
        cycle_counter++;
        if(cycle_counter % 1 ==0){
            printf("%dth cycle:\n",cycle_counter);
            if(count_sent_and_rcvd()){
                break;
            }
        }
        
    }
    print_stats();

    network_UUT.network_free();
}
