#ifndef FLIT_H
#define FLIT_H

class flit{
public:
    bool valid;
    char flit_type;
    bool VC_class;
    char dst_z;
    char dst_y;
    char dst_x;
    char inject_dir;
    char dir_out;
    int priority_dist;
    int priority_age;
    char src_z;
    char src_y;
    char src_x;
    int packet_id;
    int flit_id;
    int payload;
    int packet_size;
	flit();
    flit(bool valid, char Type, int Payload, int Flit_id);
	flit(bool Valid, char Type, bool Vc_class, char Dst_z, char Dst_y, char Dst_x, int Priority_dist, int Priority_age, char Src_z, char Src_y, char Src_x, int Packet_id, int Flit_id, int Payload, int Packet_size);
        
};


#endif
