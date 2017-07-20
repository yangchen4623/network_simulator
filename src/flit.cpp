#include "flit.h"

flit::flit(){
	valid = false;
}

flit::flit(bool Valid, char Type, int Payload, int Flit_id){
    valid = Valid;
    flit_type = Type;
    payload = Payload;
    flit_id = Flit_id;
}

flit::flit(bool Valid, char Type, bool Vc_class, char Dst_z, char Dst_y, char Dst_x, \
         int Priority_dist, int Priority_age, char Src_z, char Src_y, \
         char Src_x, int Packet_id, int Flit_id, int Payload, int Packet_size){
    valid = Valid;
    flit_type = Type;
    VC_class = Vc_class;
    dst_z = Dst_z;
    dst_y = Dst_y;
    dst_x = Dst_x;
    priority_dist = Priority_dist;
    priority_age = Priority_age;
    src_z = Src_z;
    src_y = Src_y;
    src_x = Src_x;
    packet_id = Packet_id;
    flit_id = Flit_id;
    payload = Payload;
    packet_size = Packet_size;
}

