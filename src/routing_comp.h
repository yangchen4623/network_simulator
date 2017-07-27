#ifndef ROUTING_CONP_H
#define ROUTING_COMP_H
#include "define.h"
#include "flit.h"
class routing_comp{
public: 
    char mode; 
    char cur_x;
    char cur_y;
    char cur_z;
    char dir_in;
    flit* in;
    flit in_latch;
    bool in_avail;
    bool out_avail_latch;
    bool* out_avail;
    flit out;
    flit flit_after_RC;
    flit flit_eject;
    bool eject_enable;
	int cycle_counter = 0;
	int *xpos_credits;
	int *ypos_credits;
	int *zpos_credits;
	int *xneg_credits;
	int *yneg_credits;
	int *zneg_credits;

	void routing_comp_init(char Cur_x, char Cur_y, char Cur_z, char Dir_in, char Mode, flit* In, bool* Out_avail, int* Xpos_credits, int* Ypos_credits, int* Zpos_credits, int* Xneg_credits, int* Yneg_credits, int* Zneg_credits);
    void consume();
    void produce();
	int dir;
	int DOR_XYZ(int cx, int cy, int cz, int dx, int dy, int dz);
	int DOR_XZY(int cx, int cy, int cz, int dx, int dy, int dz);
	int DOR_YXZ(int cx, int cy, int cz, int dx, int dy, int dz);
	int DOR_YZX(int cx, int cy, int cz, int dx, int dy, int dz);
	int DOR_ZXY(int cx, int cy, int cz, int dx, int dy, int dz);
	int DOR_ZYX(int cx, int cy, int cz, int dx, int dy, int dz);
	int ROMM(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz);
	int RCA(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz);
	int O1TURN(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz, int O1TURN_id, int* init_id);
	int RLB(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz);

};




#endif
