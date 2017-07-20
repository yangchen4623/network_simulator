#include "routing_comp.h"
#include "flit.h"
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

    return;
}

void routing_comp::consume(){
    in_latch = *in;
    out_avail_latch = *out_avail;
    return;
}

void routing_comp::produce(){
    //compute the routing decision first no matter stall or not
    int dir = 0;
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
    if(out_avail){
        flit_after_RC = in_latch;
        if(in_latch.valid && (in_latch.flit_type == HEAD_FLIT || in_latch.flit_type == SINGLE_FLIT)){
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


