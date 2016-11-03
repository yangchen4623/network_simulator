#include "link.h"
#include "node.h"


class network{
    private:
        int size_x;
        int size_y;
        int size_z;
        node* node_list;
        link* link_list_xpos;
        link* link_list_xneg;
        link* link_list_ypos;
        link* link_list_yneg;
        link* link_list_zpos;
        link* link_list_zneg;
    public:
        //constructor
        network(int x, int y, int z);
        int get_size_x(){
            return size_x;
        }
        int get_size_y(){
            return size_y;
        }
        int get_size_z(){
            return size_z;
        }
        
};

network::network(int x, int y, int z){
    size_x=x;
    size_y=y;
    size_z=z;
    node_list=new node[z*y*x];
    link_list_xpos=new link[z*y*x];
    

}
