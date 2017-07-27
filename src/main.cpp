#include "network.h"
#include "pattern.h"
#include "define.h"
#include "packet.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

int global_injection_packet_size[PORT_NUM][ZSIZE][YSIZE][XSIZE];
packet* pattern[PORT_NUM][ZSIZE][YSIZE][XSIZE];

int pos_or_neg(int src, int dst, int x_or_y_or_z){
	int size;
	int ret = 2; //1 is pos, -1 is neg, 0 is both fine, 2 is error
	if (x_or_y_or_z == 0)//x dim
		size = XSIZE;
	else if (x_or_y_or_z == 1)
		size = YSIZE;
	else if (x_or_y_or_z == 2)
		size = ZSIZE;
	else
		return 2;

	if (src >= size || src < 0 || dst >= size || dst < 0)
		return 2;

	if (src > dst){
		if (size % 2 == 0){
			if (src - dst < size / 2)
				ret = -1;
			else if (src - dst > size / 2)
				ret = 1;
			else
				ret = 0;
		}
		else{
			if (src - dst <= size / 2)
				ret = -1;
			else
				ret = 1;
		}
	}
	else if (src < dst){
		if (size % 2 == 0){
			if (dst - src < size / 2)
				ret = 1;
			else if (dst - src > size / 2)
				ret = -1;
			else
				ret = 0;
		}
		else{
			if (dst - src <= size / 2)
				ret = 1;
			else
				ret = -1;
		}
	}
	else
		ret = 2;
	return ret;
}

char comp_inject_dir(int src_x, int src_y, int src_z, int dst_x, int dst_y, int dst_z){
//compute which direction to inject, the injection direction must not violate the ristricted turning rules. 
//the six turning is forbidden
//zneg->xpos
//zneg->xneg
//zneg->ypos
//zneg->yneg
//yneg->xpos
//yneg->xneg
	char ret;
	int x_dir;
	int y_dir;
	int z_dir;
	if (src_x == dst_x && src_y == dst_y && src_z == dst_z){
		ret = DIR_EJECT;
	}
	else if (src_x == dst_x && src_y == dst_y){
		z_dir = pos_or_neg(src_z, dst_z, 2);
		if (z_dir == -1)
			ret = DIR_ZNEG;
		else if (z_dir == 1)
			ret = DIR_ZPOS;
		else{
			int i = 0;// rand() % 2;
			if (i == 0)
				ret = DIR_ZPOS;
			else
				ret = DIR_ZNEG;
		}

	}
	else if (src_x == dst_x){
		y_dir = pos_or_neg(src_y, dst_y, 1);
		if (y_dir == -1)
			ret = DIR_YNEG;
		else if (y_dir == 1)
			ret = DIR_YPOS;
		else{
			int i = 1;// rand() % 2;
			if (i == 0)
				ret = DIR_YPOS;
			else
				ret = DIR_YNEG;
		}
	}
	else{//in this case, all the DIR_XPOS, DIR_YPOS, DIR_ZPOS, DIR_XNEG are allowed to use, DIR_YNEG and DIR_ZNEG are not allowed
		x_dir = pos_or_neg(src_x, dst_x, 0);
		y_dir = pos_or_neg(src_y, dst_y, 1);
		z_dir = pos_or_neg(src_z, dst_z, 2);
		if (z_dir == -1 || z_dir == 2){ //zpos is banned also
			if (y_dir == -1 || y_dir == 2){//ypos is banned also
				if (x_dir == -1)
					ret = DIR_XNEG;
				else if (x_dir == 1)
					ret = DIR_XPOS;
				else{
					int i = 0;// rand() % 2;
					if (i == 0)
						ret = DIR_XPOS;
					else
						ret = DIR_XNEG;
				}
			}
			else if (y_dir == 1 || y_dir == 0){//ypos is allowed
				if (x_dir == -1){
					int i = 1;// rand() % 2;
					if (i == 0)
						ret = DIR_YPOS;
					else
						ret = DIR_XNEG;
				}
				else if (x_dir == 1){
					int i = 0;// rand() % 2;
					if (i == 0)
						ret = DIR_YPOS;
					else
						ret = DIR_XPOS;
				}
				else{
					int i = 1;// rand() % 3;
					if (i == 0)
						ret = DIR_YPOS;
					else if (i == 1)
						ret = DIR_XPOS;
					else
						ret = DIR_XNEG;
				}
			}

		}
	
		else if (z_dir == 1 || z_dir == 0){//zpos is allowed
			if (y_dir == -1 || y_dir == 2){//ypos is banned also
				if (x_dir == -1){
					int i = 0;// rand() % 2;
					if (i == 0)
						ret = DIR_ZPOS;
					else
						ret = DIR_XNEG;
				}
				else if (x_dir == 1){
					int i = 1;// rand() % 2;
					if (i == 0)
						ret = DIR_ZPOS;
					else
						ret = DIR_XPOS;
				}
				else{
					int i = 0;// rand() % 3;
					if (i == 0)
						ret = DIR_ZPOS;
					else if (i == 1)
						ret = DIR_XPOS;
					else
						ret = DIR_XNEG;
				}
			}
			else if (y_dir == 1 || y_dir == 0){//ypos is allowed
				if (x_dir == -1){
					int i = 1;// rand() % 3;
					if (i == 0)
						ret = DIR_ZPOS;
					else if (i == 1)
						ret = DIR_YPOS;
					else
						ret = DIR_XNEG;
				}
				else if (x_dir == 1){
					int i = 2;// rand() % 3;
					if (i == 0)
						ret = DIR_ZPOS;
					else if (i == 1)
						ret = DIR_YPOS;
					else
						ret = DIR_XPOS;
				}
				else{
					int i = 3;// rand() % 4;
					if (i == 0)
						ret = DIR_YPOS;
					else if (i == 1)
						ret = DIR_XPOS;
					else if (i == 2)
						ret = DIR_XNEG;
					else
						ret = DIR_ZPOS;
				}

			}
		}

	}

	return ret;
}


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

void gen_pattern_cube_nearest_neighbor(int pattern_size){ //each node multicast to 26 nearest neighbors
	total_packet_sent = 0;
	char cur_inject_dir;
	for (int z = 0; z < ZSIZE; ++z){
		for (int y = 0; y < YSIZE; ++y){
			for (int x = 0; x < XSIZE; ++x){
				for (int i = 0; i < PORT_NUM; ++i){
					global_injection_packet_size[i][z][y][x] = 0;
					if (!(pattern[i][z][y][x] = (packet*)malloc(26 * pattern_size * sizeof(packet)))){//worst case, all the packets are injected in a single injection port
						printf("error when allocating space for pattern\n");
						exit(-1);
					}
					for (int j = 0; j < 26 * pattern_size; ++j){
						pattern[i][z][y][x][j].valid = false;
						pattern[i][z][y][x][j].rcvd = false;
						pattern[i][z][y][x][j].sent = false;
					}
				}

				for (int dst_z = -1; dst_z <= 1; ++dst_z){
					for (int dst_y = -1; dst_y <= 1; ++dst_y){
						for (int dst_x = -1; dst_x <= 1; ++dst_x){
							for (int j = 0; j < pattern_size; ++j){
								int real_dst_x = (x + dst_x < 0) ? (x + dst_x + XSIZE) : ((x + dst_x >= XSIZE) ? (x + dst_x - XSIZE) : (x + dst_x));
								int real_dst_y = (y + dst_y < 0) ? (y + dst_y + YSIZE) : ((y + dst_y >= YSIZE) ? (y + dst_y - YSIZE) : (y + dst_y));
								int real_dst_z = (z + dst_z < 0) ? (z + dst_z + ZSIZE) : ((z + dst_z >= ZSIZE) ? (z + dst_z - ZSIZE) : (z + dst_z));
								

								cur_inject_dir = comp_inject_dir(x, y, z, real_dst_x, real_dst_y, real_dst_z);
								if (cur_inject_dir != DIR_EJECT){
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].inject_dir = cur_inject_dir;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].src_x = x;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].src_y = y;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].src_z = z;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].id = global_injection_packet_size[cur_inject_dir - 1][z][y][x];
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].dst_x = real_dst_x;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].dst_y = real_dst_y;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].dst_z = real_dst_z;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].mahattan_dist = abs(dst_z) + abs(dst_y) + abs(dst_x);
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].sent = false;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].rcvd = false;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].payload = global_injection_packet_size[cur_inject_dir - 1][z][y][x];
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].valid = true;
									global_injection_packet_size[cur_inject_dir - 1][z][y][x]++;
									total_packet_sent++;
								}

							}
						}
					}
				}
			}
		}
	}

}

void gen_pattern_all_to_all(int pattern_size){ //each node multicast to 26 nearest neighbors
	total_packet_sent = 0;
	char cur_inject_dir;
	for (int z = 0; z < ZSIZE; ++z){
		for (int y = 0; y < YSIZE; ++y){
			for (int x = 0; x < XSIZE; ++x){
				for (int i = 0; i < PORT_NUM; ++i){
					global_injection_packet_size[i][z][y][x] = 0;
					if (!(pattern[i][z][y][x] = (packet*)malloc(XSIZE * YSIZE * ZSIZE * pattern_size * sizeof(packet)))){//worst case, all the packets are injected in a single injection port
						printf("error when allocating space for pattern\n");
						exit(-1);
					}
					for (int j = 0; j < XSIZE * YSIZE * ZSIZE * pattern_size; ++j){
						pattern[i][z][y][x][j].valid = false;
						pattern[i][z][y][x][j].rcvd = false;
						pattern[i][z][y][x][j].sent = false;
					}
				}

				for (int dst_z = - ZSIZE / 2; dst_z <= ceil(double(ZSIZE / 2)) - 1; ++dst_z){
					for (int dst_y = -YSIZE / 2; dst_y <= ceil(double(YSIZE / 2)) - 1; ++dst_y){
						for (int dst_x = -XSIZE / 2; dst_x <= ceil(double(XSIZE / 2)) - 1; ++dst_x){
							for (int j = 0; j < pattern_size; ++j){
								int real_dst_x = (x + dst_x < 0) ? (x + dst_x + XSIZE) : ((x + dst_x >= XSIZE) ? (x + dst_x - XSIZE) : (x + dst_x));
								int real_dst_y = (y + dst_y < 0) ? (y + dst_y + YSIZE) : ((y + dst_y >= YSIZE) ? (y + dst_y - YSIZE) : (y + dst_y));
								int real_dst_z = (z + dst_z < 0) ? (z + dst_z + ZSIZE) : ((z + dst_z >= ZSIZE) ? (z + dst_z - ZSIZE) : (z + dst_z));


								cur_inject_dir = comp_inject_dir(x, y, z, real_dst_x, real_dst_y, real_dst_z);
								if (cur_inject_dir != DIR_EJECT){
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].inject_dir = cur_inject_dir;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].src_x = x;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].src_y = y;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].src_z = z;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].id = global_injection_packet_size[cur_inject_dir - 1][z][y][x];
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].dst_x = real_dst_x;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].dst_y = real_dst_y;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].dst_z = real_dst_z;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].mahattan_dist = abs(dst_z) + abs(dst_y) + abs(dst_x);
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].sent = false;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].rcvd = false;
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].payload = global_injection_packet_size[cur_inject_dir - 1][z][y][x];
									pattern[cur_inject_dir - 1][z][y][x][global_injection_packet_size[cur_inject_dir - 1][z][y][x]].valid = true;
									global_injection_packet_size[cur_inject_dir - 1][z][y][x]++;
									total_packet_sent++;
								}

							}
						}
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

bool print_unrcvd() {
	int cur_sent_num = 0;
	int cur_rcvd_num = 0;
	for (int i = 0; i < PORT_NUM; ++i) {
		for (int j = 0; j < ZSIZE; ++j) {
			for (int k = 0; k < YSIZE; ++k) {
				for (int m = 0; m < XSIZE; ++m) {
					for (int n = 0; n < global_injection_packet_size[i][j][k][m]; ++n) {
						if (pattern[i][j][k][m][n].valid && pattern[i][j][k][m][n].sent)
							cur_sent_num++;
						if (pattern[i][j][k][m][n].valid && pattern[i][j][k][m][n].rcvd)
							cur_rcvd_num++;
						if (pattern[i][j][k][m][n].valid && pattern[i][j][k][m][n].sent && !pattern[i][j][k][m][n].rcvd)
							printf("(%d,%d,%d) to (%d,%d,%d), packet id %d,sent at %dth cycle, injection dir %d, is not rcvd\n", pattern[i][j][k][m][n].src_x, pattern[i][j][k][m][n].src_y, pattern[i][j][k][m][n].src_z, \
								pattern[i][j][k][m][n].dst_x, pattern[i][j][k][m][n].dst_y, pattern[i][j][k][m][n].dst_z, pattern[i][j][k][m][n].id, pattern[i][j][k][m][n].send_time_stamp, pattern[i][j][k][m][n].inject_dir);
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
	srand((unsigned)time(NULL));
    int cycle_counter = 0;
    int pattern_size = 5;
	int packet_size = 10; //has to be smaller than VC_SIZE
	int injection_gap = 1;
	gen_pattern_all_to_all(pattern_size);

    network network_UUT;
	int max_VCs = 0;
	network_UUT.network_init(XSIZE, YSIZE, ZSIZE, 0, ROUTING_O1TURN, SA_OLDEST_FIRST, injection_gap, packet_size);
    while(1){
		if (network_UUT.consume() == -1){
			break;
		}
		if (network_UUT.produce() == -1){
			break;
		}
        cycle_counter++;
        if(cycle_counter % 1 ==0){
            printf("%dth cycle:\n",cycle_counter);
            if(count_sent_and_rcvd()){
                break;
            }
        }
		if (cycle_counter % 100 == 0) {
			int tmp = network_UUT.network_max_busy_VC_num();
			if (tmp > max_VCs)
				max_VCs = tmp;
			printf("%dth cycle: max_VCs is %d \n", cycle_counter, tmp);
		}
        
    }
    print_stats();
	printf("overall max_VCs is %d\n", max_VCs);
    network_UUT.network_free();
}
