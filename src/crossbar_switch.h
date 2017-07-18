#ifndef CROSSBAR_SWITCH_H
#define CROSSBAR_SWITCH_H

#define N_FAN_IN PORT_NUM * VC_NUM
#include "define.h"
#include "reduction_tree.h"

class crossbar_switch{
public:
   int N_fan_in = N_FAN_IN;
   int N_fan_out = PORT_NUM;
   int mode;
   flit* flit_in[N_FAN_IN];
   int level_num = 4;
   flit in_latch[PORT_NUM][N_FAN_IN];
   bool in_avail[PORT_NUM][N_FAN_IN];
   bool out_avail_latch[PORT_NUM];
   bool* out_avail[PORT_NUM];

   flit* in_list_to_tree[PORT_NUM][N_FAN_IN];//pointers passed to all the reduction tree
   bool* out_avail_to_tree[PORT_NUM];

   flit out[PORT_NUM];
   

   reduction_tree tree_list[PORT_NUM];

   void crossbar_switch_init(int Mode, flit** In_list, bool** Out_avail);
   void consume();
   void produce();
   bool lookup_in_avail(int port_id, int our_dir);
   void free();
};




#endif
