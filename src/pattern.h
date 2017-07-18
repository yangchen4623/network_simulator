#ifndef PATTERN_H
#define PATTERN_H
#include "flit.h"


struct packet{
    int src_x;
    int src_y;
    int src_z;
    int id;
    int dst_x;
    int dst_y;
    int dst_z;
    bool sent;
    bool rcvd;
    int send_time_stamp;
    int recv_time_stamp;
}

pacekt* xpos_pattern[ZSIZE][YSIZE][XSIZE];
packet* ypos_pattern[ZSIZE][YSIZE][XSIZE];
packet* zpos_pattern[ZSIZE][YSIZE][XSIZE];
packet* xneg_pattern[ZSIZE][YSIZE][XSIZE];
packet* yneg_pattern[ZSIZE][YSIZE][XSIZE];
packet* zneg_pattern[ZSIZE][YSIZE][XSIZE];








#endif
