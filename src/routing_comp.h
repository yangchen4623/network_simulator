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
	void routing_comp_init(char Cur_x, char Cur_y, char Cur_z, char Dir_in, char Mode, flit* In, bool* Out_avail);
    void consume();
    void produce();
	int dir;

	int ROMM(int input_dir, int cx, int cy, int cz, int dx, int dy, int dz);

};




#endif
