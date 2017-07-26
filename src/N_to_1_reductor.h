#ifndef N_TO_1_REDUCTOR_H
#define N_TO_1_REDUCTOR_H
#include "flit.h"
#include "fifo.h"
class N_to_1_reductor{
public:
    int N_fan_in;
    int out_dir;
    int level;
    int id;
    int mode;
    int selector;
    bool occupy;
    flit** flit_in; //will allocate by N_fan_in slots after init
    flit* in_latch; // will allocate by N_fan_in slots after init
    bool* in_avail; // will allocate by N_fan_in slots after init
    bool out_avail_latch;
	bool* out_avail_latch_to_fifos; // will allcoate by N_fan_in slots after init
    bool* out_avail;
    
    flit out;
	flit* in_slot;
    fifo* in_Q_inst; //will allocate by N_fan_in fifo after init
	int cycle_counter;

    void alloc(int N_Fan_in);
    void N_to_1_reductor_init(int Out_dir, int Level, int Id, int Mode, flit** In_list, bool* Out_avail);
    void consume();
    int produce();
	void N_to_1_reductor_free();
};




#endif
