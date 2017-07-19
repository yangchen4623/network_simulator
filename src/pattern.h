#ifndef PATTERN_H
#define PATTERN_H
#include "flit.h"


struct packet{
    int valid;
    int src_x;
    int src_y;
    int src_z;
    int id;
    int dst_x;
    int dst_y;
    int dst_z;
    int mahattan_dst;
    bool sent;
    bool rcvd;
    int payload;
    int send_time_stamp;
    int recv_time_stamp;
}

int global_injection_packt_size[PORT_NUM][ZSIZE][YSIZE][XSIZE];
packet* pattern[PORT_NUM][ZSIZE][YSIZE][XSIZE];

pacekt* xpos_pattern[ZSIZE][YSIZE][XSIZE];
packet* ypos_pattern[ZSIZE][YSIZE][XSIZE];
packet* zpos_pattern[ZSIZE][YSIZE][XSIZE];
packet* xneg_pattern[ZSIZE][YSIZE][XSIZE];
packet* yneg_pattern[ZSIZE][YSIZE][XSIZE];
packet* zneg_pattern[ZSIZE][YSIZE][XSIZE];








#endif
