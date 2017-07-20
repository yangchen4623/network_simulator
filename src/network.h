#ifndef NETWORK_H
#define NETWORK_H

#include "link.h"
#include "node.h"
class network{
private:
    int size_x;
    int size_y;
    int size_z;
    node*** node_list;
    link*** link_list_xpos;
    link*** link_list_xneg;
    link*** link_list_ypos;
    link*** link_list_yneg;
    link*** link_list_zpos;
    link*** link_list_zneg;
public:
    void network_init(int x, int y, int z);
	void consume();
	void produce();
	void network_free();
};

#endif
