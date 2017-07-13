#ifndef NODE_H
#define NODE_H
#include "flit.h"
class node{
public:
    char cur_x;
    char cur_y;
    char cur_z;

    link* link_xpos;
    link* link_ypos;
    link* link_zpos;
    link* link_xneg;
    link* link_yneg;
    link* link_zneg;


    flit out_xpos;
    flit out_ypos;
    flit out_zpos;
    flit out_xneg;
    flit out_yneg;
    flit out_zneg;
        
    node(int x, int y, int z){
        
    }



};

#endif
