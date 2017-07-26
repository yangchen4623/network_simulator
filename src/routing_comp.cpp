#include "routing_comp.h"
#include "define.h"
#include "flit.h"
#include<stdio.h>
#include<stdlib.h>
void routing_comp::routing_comp_init(char Cur_x, char Cur_y, char Cur_z, char Dir_in, char Mode, flit* In, bool* Out_avail){
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


void routing_comp::produce(){
    //compute the routing decision first no matter stall or not
 
	cycle_counter++;
    if(mode == ROUTING_DOR_XYZ){
        if(cur_x != in_latch.dst_x){
            if(cur_x > in_latch.dst_x){
                if(cur_x - in_latch.dst_x >= XSIZE / 2){
                    dir = DIR_XPOS;
                }
                else{
                    dir = DIR_XNEG;
                }
            }
            else{
                if(in_latch.dst_x - cur_x <= XSIZE / 2){
                    dir = DIR_XPOS;
                }
                else{
                    dir = DIR_XNEG;
                }
            }
        }
        else if(cur_y != in_latch.dst_y){
            if(cur_y > in_latch.dst_y){
                if(cur_y - in_latch.dst_y >= YSIZE / 2){
                    dir = DIR_YPOS;
                }
                else{
                    dir = DIR_YNEG;
                }
            }
            else{
                if(in_latch.dst_y - cur_y <= YSIZE / 2){
                    dir = DIR_YPOS;
                }
                else{
                    dir = DIR_YNEG;
                }
            }
        }
        else if(cur_z != in_latch.dst_z){
            if(cur_z > in_latch.dst_z){
                if(cur_z - in_latch.dst_z >= ZSIZE / 2){
                    dir = DIR_ZPOS;
                }
                else{
                    dir = DIR_ZNEG;
                }
            }
            else{
                if(in_latch.dst_z - cur_z <= ZSIZE / 2){
                    dir = DIR_ZPOS;
                }
                else{
                    dir = DIR_ZNEG;
                }
            }
        }
        else{
            dir = DIR_EJECT;
        }

    }
	else if (mode == ROUTING_HEUR){

	}
	else if (mode == ROUTING_ROMM){
		if (in_latch.valid && (in_latch.flit_type == HEAD_FLIT || in_latch.flit_type == SINGLE_FLIT))
			dir = ROMM(dir_in, cur_x, cur_y, cur_z, in_latch.dst_x, in_latch.dst_y, in_latch.dst_z);
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
                if(cur_x == XSIZE - 1)
                    new_VC_class = 1;
                else
                    new_VC_class = old_VC_class;
                break;
            case DIR_XNEG:
                if(cur_x == 0)
                    new_VC_class = 0;
                else
                    new_VC_class = old_VC_class;
                break;
            case DIR_YPOS:
                if(cur_y == YSIZE - 1)
                    new_VC_class = 1;
                else
                    new_VC_class = old_VC_class;
                break;
            case DIR_YNEG:
                if(cur_y == 0)
                    new_VC_class = 0;
                else
                    new_VC_class = old_VC_class;
				break;            
            case DIR_ZPOS:
                if(cur_z == ZSIZE - 1)
                    new_VC_class = 1;
                else 
                    new_VC_class = old_VC_class;
                break;
            case DIR_ZNEG:
                if(cur_z == 0)
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
    if(eject_enable){
        flit_eject = in_latch;
        in_avail = true;
        flit_after_RC.valid = false;
        out.valid = false;
        if(in_latch.valid && (in_latch.flit_type == SINGLE_FLIT || in_latch.flit_type == TAIL_FLIT))
            eject_enable = false;
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


