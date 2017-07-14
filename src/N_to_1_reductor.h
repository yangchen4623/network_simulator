#ifndef N_TO_1_REDUCTOR_H
#define N_TO_1_REDUCTOR_H
#include "flit.h"
class N_to_1_reductor{
public:
    int N_fan_in;
    int out_dir;
    int level;
    int id;
    int mode;
    flit** flit_in; //will allocate by N_fan_in slots after init
    flit* in_latch; // will allocate by N_fan_in slots after init
    bool* in_avail; // will allocate by N_fan_in slots after init
    bool out_avail_latch;
    bool* out_avail;
    
    flit out;
    flit* in_slot; //will allocate by N_fan_in slots after init

    void N_to_1_reductor_init(int N_Fan_in, int Out_dir, int Level, int Id, flit** In_list, bool* Out_avail);
    void consume();
    void produce();
    void free();
};




#endif
