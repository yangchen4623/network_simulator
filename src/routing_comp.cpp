#include "routing_comp.h"
#include "define.h"
#include "flit.h"
#include<stdio.h>
#include<stdlib.h>
void routing_comp::routing_comp_init(char Cur_x, char Cur_y, char Cur_z, char Dir_in, char Mode, flit* In, bool* Out_avail, int* Xpos_credits, int* Ypos_credits, int* Zpos_credits, int* Xneg_credits, int* Yneg_credits, int* Zneg_credits){
    cur_x = Cur_x;
    cur_y = Cur_y;
    cur_z = Cur_z;
    dir_in = Dir_in;
    mode = Mode;
    in_avail = true;
	in_latch.valid = false;
    out.valid = false;
    out_avail = Out_avail;
	out_avail_latch = false;
	out.valid = false;
    in = In;
    eject_enable = false;
    flit_after_RC.valid = false;
    flit_eject.valid = false;
	cycle_counter = 0;
	xpos_credits = Xpos_credits;
	ypos_credits = Ypos_credits;
	zpos_credits = Zpos_credits;
	xneg_credits = Xneg_credits;
	yneg_credits = Yneg_credits;
	zneg_credits = Zneg_credits;
    return;
}

void routing_comp::consume(){
    in_latch = *in;
    out_avail_latch = *out_avail;
    return;
}

int RC_pos_or_neg(int src, int dst, int x_or_y_or_z){
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

int routing_comp::ROMM(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz){//random select the best 
	//the six turning is forbidden
	//zneg->xpos
	//zneg->xneg
	//zneg->ypos
	//zneg->yneg
	//yneg->xpos
	//yneg->xneg
	if (input_dir >= 4)
		input_dir = input_dir - 3;
	else
		input_dir = input_dir + 3;
	int ret = 0;
	if (cx == dx && cy == dy && cz == dz)
		return DIR_EJECT;
	int x_dir = RC_pos_or_neg(cx, dx, 0);
	int y_dir = RC_pos_or_neg(cy, dy, 1);
	int z_dir = RC_pos_or_neg(cz, dz, 2);
	bool avail[6];
	for (int i = 0; i < 6; ++i)
		avail[i] = false;
	if (input_dir == DIR_ZNEG){
		if (cx != dx || cy != dy)
			printf("error! in traffic from zneg, but stil has xy offset\n");
		avail[DIR_ZNEG - 1] = true;
	}
	else if (input_dir == DIR_YNEG){
		if (cx != dx)
			printf("error! in traffic from yneg, but still has x offset\n");
		if (y_dir == 1 || y_dir == 0)
			printf("error! on yneg, cant go ypos\n");
		//zneg and zpos and yneg are possible
		if (cz == dz){
			avail[DIR_YNEG - 1] = true;
		}
		else if (z_dir == -1){
			if (cy == dy)
				avail[DIR_ZNEG - 1] = true;
			else if (y_dir == -1){
				avail[DIR_YNEG - 1] = true;
			}
		}
		else if (z_dir == 0){
			if (cy == dy){
				avail[DIR_ZNEG - 1] = true;
				avail[DIR_ZPOS - 1] = true;
			}
			else{
				avail[DIR_YNEG - 1] = true;
				avail[DIR_ZPOS - 1] = true;
			}

		}
		else if(z_dir == 1){
			if (cy == dy)
				avail[DIR_ZPOS - 1] = true;
			else{
				avail[DIR_YNEG - 1] = true;
				avail[DIR_ZPOS - 1] = true;
			}
		}
	}
	else{

		if (x_dir == 1){
			avail[DIR_XPOS - 1] = true;
		}
		else if (x_dir == 0){
			avail[DIR_XPOS - 1] = true;
			avail[DIR_XNEG - 1] = true;
		}
		else if (x_dir == -1){
			avail[DIR_XNEG - 1] = true;
		}
		
		
		if (y_dir == 1){
			avail[DIR_YPOS - 1] = true;
		}
		else if (y_dir == 0){
			avail[DIR_YPOS - 1] = true;
			if (cx == dx)
				avail[DIR_YNEG - 1] = true;
		}
		else if (y_dir == -1){
			if (cx == dx)
				avail[DIR_YNEG - 1] = true;
		} 

		if (z_dir == 1){
			avail[DIR_ZPOS - 1] = true;
		}
		else if (z_dir == 0){
			avail[DIR_ZPOS - 1] = true;
			if (cx == dx && cy == dy)
				avail[DIR_ZNEG - 1] = true;
		}
		else if (z_dir == -1){
			if (cx == dx && cy == dy)
				avail[DIR_ZNEG - 1] = true;
		}
	
	}
	int avail_port_num = 0;
	for (int i = 0; i < PORT_NUM; ++i){
		if (avail[i])
			avail_port_num++;
	}
	if (avail_port_num == 0){
		printf("error, no avail ports in ROMM algorithm \n");
		exit(-1);
	}
	int rand_port_id = rand() % avail_port_num;
//	if (avail_port_num > 2 )
	//	rand_port_id = 0;
	int counter = 0;
	for (int i = 0; i < PORT_NUM; ++i){
		if (avail[i]){
			if (counter == rand_port_id){
				ret = i + 1;
				break;
			}
			else
				counter++;

		}
	}
	if (ret < 1 || ret > 6)
		printf("error of ROMM, exiting\n");
	if (!avail[ret - 1]){
		printf("error\n");
		exit(-1);
	}
	return ret;


}



int routing_comp::DOR_XYZ(int cx, int cy, int cz, int dx, int dy, int dz){
	int x_dir = RC_pos_or_neg(cx, dx, 0);
	int y_dir = RC_pos_or_neg(cy, dy, 1);
	int z_dir = RC_pos_or_neg(cz, dz, 2);
	if (cx == dx && cy == dy && cz == dz){
		return DIR_EJECT; 
	}
	if (cx != dx){
		if (x_dir >= 0)
			return DIR_XPOS;
		else
			return DIR_XNEG;
	}
	else if (cy != dy){
		if (y_dir >= 0)
			return DIR_YPOS;
		else
			return DIR_YNEG;
	}
	else{
		if (z_dir >= 0)
			return DIR_ZPOS;
		else
			return DIR_ZNEG;
	}

}

int routing_comp::DOR_XZY(int cx, int cy, int cz, int dx, int dy, int dz){
	int x_dir = RC_pos_or_neg(cx, dx, 0);
	int y_dir = RC_pos_or_neg(cy, dy, 1);
	int z_dir = RC_pos_or_neg(cz, dz, 2);
	if (cx == dx && cy == dy && cz == dz){
		return DIR_EJECT;
	}
	if (cx != dx){
		if (x_dir >= 0)
			return DIR_XPOS;
		else
			return DIR_XNEG;
	}
	else if (cz != dz){
		if (z_dir >= 0)
			return DIR_ZPOS;
		else
			return DIR_ZNEG;
	}
	else{
		if (y_dir >= 0)
			return DIR_YPOS;
		else
			return DIR_YNEG;
	}
}

int routing_comp::DOR_YXZ(int cx, int cy, int cz, int dx, int dy, int dz){
	int x_dir = RC_pos_or_neg(cx, dx, 0);
	int y_dir = RC_pos_or_neg(cy, dy, 1);
	int z_dir = RC_pos_or_neg(cz, dz, 2);
	if (cx == dx && cy == dy && cz == dz){
		return DIR_EJECT;
	}
	if (cy != dy){
		if (y_dir >= 0)
			return DIR_YPOS;
		else
			return DIR_YNEG;
	}
	else if (cx != dx){
		if (x_dir >= 0)
			return DIR_XPOS;
		else
			return DIR_XNEG;
	}
	else{
		if (z_dir >= 0)
			return DIR_ZPOS;
		else
			return DIR_ZNEG;
	}
}

int routing_comp::DOR_YZX(int cx, int cy, int cz, int dx, int dy, int dz){
	int x_dir = RC_pos_or_neg(cx, dx, 0);
	int y_dir = RC_pos_or_neg(cy, dy, 1);
	int z_dir = RC_pos_or_neg(cz, dz, 2);
	if (cx == dx && cy == dy && cz == dz){
		return DIR_EJECT;
	}
	if (cy != dy){
		if (y_dir >= 0)
			return DIR_YPOS;
		else
			return DIR_YNEG;
	}
	else if (cz != dz){
		if (z_dir >= 0)
			return DIR_ZPOS;
		else
			return DIR_ZNEG;
	}
	else{
		if (x_dir >= 0)
			return DIR_XPOS;
		else
			return DIR_XNEG;
	}
}

int routing_comp::DOR_ZXY(int cx, int cy, int cz, int dx, int dy, int dz){
	int x_dir = RC_pos_or_neg(cx, dx, 0);
	int y_dir = RC_pos_or_neg(cy, dy, 1);
	int z_dir = RC_pos_or_neg(cz, dz, 2);
	if (cx == dx && cy == dy && cz == dz){
		return DIR_EJECT;
	}
	if (cz != dz){
		if (z_dir >= 0)
			return DIR_ZPOS;
		else
			return DIR_ZNEG;
	}
	else if (cx != dx){
		if (x_dir >= 0)
			return DIR_XPOS;
		else
			return DIR_XNEG;
	}
	else{
		if (y_dir >= 0)
			return DIR_YPOS;
		else
			return DIR_YNEG;
	}
}

int routing_comp::DOR_ZYX(int cx, int cy, int cz, int dx, int dy, int dz){
	int x_dir = RC_pos_or_neg(cx, dx, 0);
	int y_dir = RC_pos_or_neg(cy, dy, 1);
	int z_dir = RC_pos_or_neg(cz, dz, 2);
	if (cx == dx && cy == dy && cz == dz){
		return DIR_EJECT;
	}
	if (cz != dz){
		if (z_dir >= 0)
			return DIR_ZPOS;
		else
			return DIR_ZNEG;
	}
	else if (cy != dy){
		if (y_dir >= 0)
			return DIR_YPOS;
		else
			return DIR_YNEG;
	}
	else{
		if (x_dir >= 0)
			return DIR_XPOS;
		else
			return DIR_XNEG;
	}
}



int routing_comp::O1TURN(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz, int O1TURN_id, int* init_id){
	//encoding 8 possible path
	//XYZ 0
	//XZY 1
	//YXZ 2
	//YZX 3
	//ZXY 4
	//ZYX 5
	if (cx == dx && cy == dy && cz == dz)
		return DIR_EJECT;
	if (O1TURN_id == -1){
		//the packed has just been injected from a neighbor node, has not been assigned an ID
		//determine possible turning direction
		//the six turning is forbidden
		//zneg->xpos
		//zneg->xneg
		//zneg->ypos
		//zneg->yneg
		//yneg->xpos
		//yneg->xneg
		if (input_dir >= 4)
			input_dir = input_dir - 3;
		else
			input_dir = input_dir + 3;
		int ret = 0;
		if (cx == dx && cy == dy && cz == dz)
			return DIR_EJECT;
		int x_dir = RC_pos_or_neg(cx, dx, 0);
		int y_dir = RC_pos_or_neg(cy, dy, 1);
		int z_dir = RC_pos_or_neg(cz, dz, 2);
		bool avail[6];
		for (int i = 0; i < 6; ++i)
			avail[i] = false;
		if (input_dir == DIR_ZNEG){
			if (cx != dx || cy != dy)
				printf("error! in traffic from zneg, but stil has xy offset\n");
			avail[DIR_ZNEG - 1] = true;
		}
		else if (input_dir == DIR_YNEG){
			if (cx != dx)
				printf("error! in traffic from yneg, but still has x offset\n");
			if (y_dir == 1 || y_dir == 0)
				printf("error! on yneg, cant go ypos\n");
			//zneg and zpos and yneg are possible
			if (cz == dz){
				avail[DIR_YNEG - 1] = true;
			}
			else if (z_dir == -1){
				if (cy == dy)
					avail[DIR_ZNEG - 1] = true;
				else if (y_dir == -1){
					avail[DIR_YNEG - 1] = true;
				}
			}
			else if (z_dir == 0){
				if (cy == dy){
					avail[DIR_ZNEG - 1] = true;
					avail[DIR_ZPOS - 1] = true;
				}
				else{
					avail[DIR_YNEG - 1] = true;
					avail[DIR_ZPOS - 1] = true;
				}

			}
			else if (z_dir == 1){
				if (cy == dy)
					avail[DIR_ZPOS - 1] = true;
				else{
					avail[DIR_YNEG - 1] = true;
					avail[DIR_ZPOS - 1] = true;
				}
			}
		}
		else{

			if (x_dir == 1){
				avail[DIR_XPOS - 1] = true;
			}
			else if (x_dir == 0){
				avail[DIR_XPOS - 1] = true;
				avail[DIR_XNEG - 1] = true;
			}
			else if (x_dir == -1){
				avail[DIR_XNEG - 1] = true;
			}


			if (y_dir == 1){
				avail[DIR_YPOS - 1] = true;
			}
			else if (y_dir == 0){
				avail[DIR_YPOS - 1] = true;
				if (cx == dx)
					avail[DIR_YNEG - 1] = true;
			}
			else if (y_dir == -1){
				if (cx == dx)
					avail[DIR_YNEG - 1] = true;
			}

			if (z_dir == 1){
				avail[DIR_ZPOS - 1] = true;
			}
			else if (z_dir == 0){
				avail[DIR_ZPOS - 1] = true;
				if (cx == dx && cy == dy)
					avail[DIR_ZNEG - 1] = true;
			}
			else if (z_dir == -1){
				if (cx == dx && cy == dy)
					avail[DIR_ZNEG - 1] = true;
			}
		}
		bool path_avail[PORT_NUM];
		for (int i = 0; i < PORT_NUM; ++i)
			path_avail[i] = false;
		if (avail[0] || avail[3]){
			path_avail[0] = true;
			if (z_dir == 1) //zneg is not allowed infront of Y
				path_avail[1] = true;
		}
		if (avail[1] || avail[4]){
			if (cx == dx){ //yzx and yxz are both allowed because of no x now
				path_avail[2] = true;
				path_avail[3] = true;
			}
			if (y_dir == 1){ //yneg is not allowed infornt of X
				path_avail[2] = true;
				if (z_dir == 1){
					path_avail[3] = true;
				}
			}
			else{
				if (z_dir == 1){
					path_avail[3] = true;
				}
			}
		}
		if (avail[2] || avail[5]){
			if (cx == dx && cy == dy){
				path_avail[4] = true;
				path_avail[5] = true;
			}
			if (z_dir == 1){
				path_avail[4] = true;
				if (y_dir == 1)
					path_avail[5] = true;
			}
		}
		int avail_path_num = 0;
		for (int i = 0; i < PORT_NUM; ++i){
			if (path_avail[i])
				avail_path_num++;
		}
		if (avail_path_num == 0){
			printf("error, no avail path in O1TURN algorithm \n");
			exit(-1);
		}
		int rand_path_id = rand() % avail_path_num;
		//	if (avail_port_num > 2 )
		//	rand_port_id = 0;
		int counter = 0;
		int selected_path;
		for (int i = 0; i < PORT_NUM; ++i){
			if (path_avail[i]){
				if (counter == rand_path_id){
					selected_path = i;
					break;
				}
				else
					counter++;

			}
		}
		*init_id = selected_path;
		if (selected_path == 0)
			return DOR_XYZ(cx, cy, cz, dx, dy, dz);
		else if (selected_path == 1)
			return DOR_XZY(cx, cy, cz, dx, dy, dz);
		else if (selected_path == 2)
			return DOR_YXZ(cx, cy, cz, dx, dy, dz);
		else if (selected_path == 3)
			return DOR_YZX(cx, cy, cz, dx, dy, dz);
		else if (selected_path == 4)
			return DOR_ZXY(cx, cy, cz, dx, dy, dz);
		else if (selected_path == 5)
			return DOR_ZYX(cx, cy, cz, dx, dy, dz);
		else{
			printf("error\n");
			exit(-1);
		}


	}
	else{
		if (O1TURN_id == 0)
			return DOR_XYZ(cx, cy, cz, dx, dy, dz);
		else if (O1TURN_id == 1)
			return DOR_XZY(cx, cy, cz, dx, dy, dz);
		else if (O1TURN_id == 2)
			return DOR_YXZ(cx, cy, cz, dx, dy, dz);
		else if (O1TURN_id == 3)
			return DOR_YZX(cx, cy, cz, dx, dy, dz);
		else if (O1TURN_id == 4)
			return DOR_ZXY(cx, cy, cz, dx, dy, dz);
		else if (O1TURN_id == 5)
			return DOR_ZYX(cx, cy, cz, dx, dy, dz);
		else{
			printf("error\n");
			exit(-1);
		}

	}
}

int routing_comp::RCA(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz){
	//the six turning is forbidden
	//zneg->xpos
	//zneg->xneg
	//zneg->ypos
	//zneg->yneg
	//yneg->xpos
	//yneg->xneg
	if (input_dir >= 4)
		input_dir = input_dir - 3;
	else
		input_dir = input_dir + 3;
	int ret = 0;
	if (cx == dx && cy == dy && cz == dz)
		return DIR_EJECT;
	int x_dir = RC_pos_or_neg(cx, dx, 0);
	int y_dir = RC_pos_or_neg(cy, dy, 1);
	int z_dir = RC_pos_or_neg(cz, dz, 2);
	bool avail[6];
	for (int i = 0; i < 6; ++i)
		avail[i] = false;
	if (input_dir == DIR_ZNEG){
		if (cx != dx || cy != dy)
			printf("error! in traffic from zneg, but stil has xy offset\n");
		avail[DIR_ZNEG - 1] = true;
	}
	else if (input_dir == DIR_YNEG){
		if (cx != dx)
			printf("error! in traffic from yneg, but still has x offset\n");
		if (y_dir == 1 || y_dir == 0)
			printf("error! on yneg, cant go ypos\n");
		//zneg and zpos and yneg are possible
		if (cz == dz){
			avail[DIR_YNEG - 1] = true;
		}
		else if (z_dir == -1){
			if (cy == dy)
				avail[DIR_ZNEG - 1] = true;
			else if (y_dir == -1){
				avail[DIR_YNEG - 1] = true;
			}
		}
		else if (z_dir == 0){
			if (cy == dy){
				avail[DIR_ZNEG - 1] = true;
				avail[DIR_ZPOS - 1] = true;
			}
			else{
				avail[DIR_YNEG - 1] = true;
				avail[DIR_ZPOS - 1] = true;
			}

		}
		else if (z_dir == 1){
			if (cy == dy)
				avail[DIR_ZPOS - 1] = true;
			else{
				avail[DIR_YNEG - 1] = true;
				avail[DIR_ZPOS - 1] = true;
			}
		}
	}
	else{

		if (x_dir == 1){
			avail[DIR_XPOS - 1] = true;
		}
		else if (x_dir == 0){
			avail[DIR_XPOS - 1] = true;
			avail[DIR_XNEG - 1] = true;
		}
		else if (x_dir == -1){
			avail[DIR_XNEG - 1] = true;
		}


		if (y_dir == 1){
			avail[DIR_YPOS - 1] = true;
		}
		else if (y_dir == 0){
			avail[DIR_YPOS - 1] = true;
			if (cx == dx)
				avail[DIR_YNEG - 1] = true;
		}
		else if (y_dir == -1){
			if (cx == dx)
				avail[DIR_YNEG - 1] = true;
		}

		if (z_dir == 1){
			avail[DIR_ZPOS - 1] = true;
		}
		else if (z_dir == 0){
			avail[DIR_ZPOS - 1] = true;
			if (cx == dx && cy == dy)
				avail[DIR_ZNEG - 1] = true;
		}
		else if (z_dir == -1){
			if (cx == dx && cy == dy)
				avail[DIR_ZNEG - 1] = true;
		}

	}
	int avail_port_num = 0;
	int max_credits = 0;
	int credits[6];
	credits[0] = *xpos_credits;
	credits[1] = *ypos_credits;
	credits[2] = *zpos_credits;
	credits[3] = *xneg_credits;
	credits[4] = *yneg_credits;
	credits[5] = *zneg_credits;
	for (int i = PORT_NUM - 1; i >= 0; --i){
		if (avail[i]){
			// find the direction who has the most credist among available link
			if(credits[i] > max_credits)
				ret = i + 1;
		}
	}

	return ret;

}

int pos_dist(int src, int dst, int size) {
	if (src >= dst)
		return size - src + dst;
	else
		return dst - src;
}

int neg_dist(int src, int dst, int size) {
	if (src < dst)
		return dst - src;
	else
		return size - dst + src;
}

int routing_comp::RLB(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz) {
	if (cx == dx && cy == dy && cz == dz) {
		return DIR_EJECT;
	}
	if (cx != dx) {
		if (input_dir != DIR_XPOS && input_dir != DIR_XNEG) {
			int pos_distance = pos_dist(cx, dx, XSIZE);
			int neg_distance = neg_dist(cx, dx, XSIZE);
			int randnum = rand() % XSIZE;
			if (randnum <= pos_distance) {
				return DIR_XNEG;
			}
			else
				return DIR_XPOS;

		}
		else {
			return input_dir > 3 ? input_dir - 3 : input_dir + 3;
		}
	}
	else if (cy != dy) {
		if (input_dir != DIR_YPOS && input_dir != DIR_YNEG) {
			int pos_distance = pos_dist(cy, dy, YSIZE);
			int neg_distance = neg_dist(cy, dy, YSIZE);
			int randnum = rand() % YSIZE;
			if (randnum <= pos_distance) {
				return DIR_YNEG;
			}
			else
				return DIR_YPOS;
		}
		else {
			return input_dir > 3 ? input_dir - 3 : input_dir + 3;
		}
	}
	else {
		if (input_dir != DIR_ZPOS && input_dir != DIR_ZNEG) {
			int pos_distance = pos_dist(cz, dz, ZSIZE);
			int neg_distance = neg_dist(cz, dz, ZSIZE);
			int randnum = rand() % ZSIZE;
			if (randnum <= pos_distance) {
				return DIR_ZNEG;
			}
			else
				return DIR_ZPOS;
		}
		else {
			return input_dir > 3 ? input_dir - 3 : input_dir + 3;
		}
	}

}


void routing_comp::produce(){
    //compute the routing decision first no matter stall or not
 
	cycle_counter++;
    if(mode == ROUTING_DOR_XYZ){
		if (in_latch.valid && (in_latch.flit_type == HEAD_FLIT || in_latch.flit_type == SINGLE_FLIT))
			dir = DOR_XYZ(cur_x, cur_y, cur_z, in_latch.dst_x, in_latch.dst_y, in_latch.dst_z);

    }
	else if (mode == ROUTING_RCA){
		if (in_latch.valid && (in_latch.flit_type == HEAD_FLIT || in_latch.flit_type == SINGLE_FLIT))
			dir = RCA(dir_in, cur_x, cur_y, cur_z, in_latch.dst_x, in_latch.dst_y, in_latch.dst_z);
	}
	else if (mode == ROUTING_ROMM){
		if (in_latch.valid && (in_latch.flit_type == HEAD_FLIT || in_latch.flit_type == SINGLE_FLIT))
			dir = ROMM(dir_in, cur_x, cur_y, cur_z, in_latch.dst_x, in_latch.dst_y, in_latch.dst_z);
	}
	else if (mode == ROUTING_O1TURN){
		if (in_latch.valid && (in_latch.flit_type == HEAD_FLIT || in_latch.flit_type == SINGLE_FLIT)){
			int new_path_id;
			dir = O1TURN(dir_in, cur_x, cur_y, cur_z, in_latch.dst_x, in_latch.dst_y, in_latch.dst_z, in_latch.O1TURN_id, &new_path_id);
			if (in_latch.O1TURN_id == -1)
				in_latch.O1TURN_id = new_path_id;
		}
	}
	else if (mode == ROUTING_RLB_XYZ){
		if (in_latch.valid && (in_latch.flit_type == HEAD_FLIT || in_latch.flit_type == SINGLE_FLIT))
			dir = RLB(dir_in, cur_x, cur_y, cur_z, in_latch.dst_x, in_latch.dst_y, in_latch.dst_z);
	}

    bool new_VC_class; 
    //the VC_class needs to be changed when either crossing the dateline or changing dimension
    bool old_VC_class = in_latch.VC_class;
	if (dir_in != dir + 3 && dir != dir_in + 3){
		if (dir == DIR_XPOS || dir == DIR_YPOS || dir == DIR_ZPOS)
			new_VC_class = 0;
		else
			new_VC_class = 1;
		
	}
    else{
        switch(dir){
            case DIR_XPOS:
                if(cur_x == 0)
                    new_VC_class = 1;
                else
                    new_VC_class = old_VC_class;
                break;
            case DIR_XNEG:
                if(cur_x == XSIZE - 1)
                    new_VC_class = 0;
                else
                    new_VC_class = old_VC_class;
                break;
            case DIR_YPOS:
                if(cur_y == 0)
                    new_VC_class = 1;
                else
                    new_VC_class = old_VC_class;
                break;
            case DIR_YNEG:
                if(cur_y == YSIZE - 1)
                    new_VC_class = 0;
                else
                    new_VC_class = old_VC_class;
				break;            
            case DIR_ZPOS:
                if(cur_z == 0)
                    new_VC_class = 1;
                else 
                    new_VC_class = old_VC_class;
                break;
            case DIR_ZNEG:
                if(cur_z == ZSIZE - 1)
                    new_VC_class = 0;
                else
                    new_VC_class = old_VC_class;
                break;     
            default:
                new_VC_class = old_VC_class;
        }
    }
    if(eject_enable == 0 && in_latch.valid && dir == DIR_EJECT && (in_latch.flit_type == HEAD_FLIT || in_latch.flit_type == SINGLE_FLIT)){
        eject_enable = true;
    }
	if (eject_enable){

			flit_eject = in_latch;
			in_avail = true;
			flit_after_RC.valid = false;
			out.valid = false;
			if (in_latch.valid && (in_latch.flit_type == SINGLE_FLIT || in_latch.flit_type == TAIL_FLIT))
				eject_enable = false;

			if (!out_avail_latch)
				flit_eject.valid = false;
			return;
    }
//not eject_enable, the traffic is bypass
	flit_eject.valid = false;
    if(out_avail_latch){
        flit_after_RC = in_latch;
        if(in_latch.valid){
            flit_after_RC.dir_out = dir;
            flit_after_RC.VC_class = new_VC_class;
            flit_after_RC.priority_dist = flit_after_RC.priority_dist - 1;
        }
        in_avail = true;
        out = flit_after_RC;
    }
    else{
        in_avail = false;
        out = flit_after_RC;
    }

    return;
}


