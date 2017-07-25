#ifndef PACKET_H
#define PACKET_H
struct packet{
	int valid;
	int src_x;
	int src_y;
	int src_z;
	int id;
	int dst_x;
	int dst_y;
	int dst_z;
	int mahattan_dist;
	int inject_dir;
	bool sent;
	bool rcvd;
	int payload;
	int send_time_stamp;
	int recv_time_stamp;
};
#endif