#include "node.h"
#include "link.h"

#define PHIT_SIZE 8 //8 bytes
class phit{
    private:
        bool valid;
        int link_or_node;
        node* cur_node;
        node* src_node;
        node* dst_node;
        link* cur_link;
        PacketHeader header;
        char payload[PHIT_SIZE];
    public:
        phit();
};

phit::phit(){
    
}
