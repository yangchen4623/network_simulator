#ifdef NETWORK_H
#define NETWORK_H

#include "link.h"
#include "node.h"
class network{
private:
    int size_x = XSIZE;
    int size_y = YSIZE;
    int size_z = ZSIZE;
    node*** node_list;
    link*** link_list_xpos;
    link*** link_list_xneg;
    link*** link_list_ypos;
    link*** link_list_yneg;
    link*** link_list_zpos;
    link*** link_list_zneg;
public:
    void network_init(int XSIZ, int YSIZE, int ZSIZE);
};

#endif
