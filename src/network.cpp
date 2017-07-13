#include "link.h"
#include "node.h"
#include "network.h"
#include<stdlib.h>



network::network(int x, int y, int z){
    size_x = x;
    size_y = y;
    size_z = z;

    if(!(node_list = (node***)malloc(size_z * sizeof(node**)))){
        printf("no space for %d node** linked by node_list***\n", size_z);
        exit(-1);
    }
    
    for(int i = 0; i < size_y; ++i){
        if(!(node_list[i] != (node**)malloc(size_y * sizeof(node*)))){
            printf("no space for %d node* linked by node_list[%d]\n", size_y, i);
            exit(-1);
        }
    }

    for(int i = 0; i< size_z; ++i){
        for(int j = 0 ; i < size_y; ++j){
            if(!(node_list[i][j] != (node*)malloc(size_x * sizeof(node)))){
                printf("no space for %d node linked by node_list[%d][%d]\n", size_x, i, j);
            }
        }
    }

    if(!(link_list_xpos = (link***)malloc(size_z * sizeof(link**)))){
        printf("no space for %d link** linked by link_list_xpos***\n", size_z);
        exit(-1);
    }
    
    for(int i = 0; i < size_y; ++i){
        if(!(link_list_xpos[i] != (link**)malloc(size_y * sizeof(link*)))){
            printf("no space for %d link* linked by link_list_xpos[%d]\n", size_y, i);
            exit(-1);
        }
    }

    for(int i = 0; i< size_z; ++i){
        for(int j = 0 ; i < size_y; ++j){
            if(!(link_list_xpos[i][j] != (link*)malloc(size_x * sizeof(link)))){
                printf("no space for %d link linked by link_list_xpos[%d][%d]\n", size_x, i, j);
            }
        }
    }

    if(!(link_list_ypos = (link***)malloc(size_z * sizeof(link**)))){
        printf("no space for %d link** linked by link_list_ypos***\n", size_z);
        exit(-1);
    }
    
    for(int i = 0; i < size_y; ++i){
        if(!(link_list_ypos[i] != (link**)malloc(size_y * sizeof(link*)))){
            printf("no space for %d link* linked by link_list_ypos[%d]\n", size_y, i);
            exit(-1);
        }
    }

    for(int i = 0; i< size_z; ++i){
        for(int j = 0 ; i < size_y; ++j){
            if(!(link_list_ypos[i][j] != (link*)malloc(size_x * sizeof(link)))){
                printf("no space for %d link linked by link_list_ypos[%d][%d]\n", size_x, i, j);
            }
        }
    }


    if(!(link_list_zpos = (link***)malloc(size_z * sizeof(link**)))){
        printf("no space for %d link** linked by link_list_zpos***\n", size_z);
        exit(-1);
    }
    
    for(int i = 0; i < size_y; ++i){
        if(!(link_list_zpos[i] != (link**)malloc(size_y * sizeof(link*)))){
            printf("no space for %d link* linked by link_list_zpos[%d]\n", size_y, i);
            exit(-1);
        }
    }

    for(int i = 0; i< size_z; ++i){
        for(int j = 0 ; i < size_y; ++j){
            if(!(link_list_zpos[i][j] != (link*)malloc(size_x * sizeof(link)))){
                printf("no space for %d link linked by link_list_zpos[%d][%d]\n", size_x, i, j);
            }
        }
    }

    if(!(link_list_xneg = (link***)malloc(size_z * sizeof(link**)))){
        printf("no space for %d link** linked by link_list_xneg***\n", size_z);
        exit(-1);
    }
    
    for(int i = 0; i < size_y; ++i){
        if(!(link_list_xneg[i] != (link**)malloc(size_y * sizeof(link*)))){
            printf("no space for %d link* linked by link_list_xneg[%d]\n", size_y, i);
            exit(-1);
        }
    }

    for(int i = 0; i< size_z; ++i){
        for(int j = 0 ; i < size_y; ++j){
            if(!(link_list_xneg[i][j] != (link*)malloc(size_x * sizeof(link)))){
                printf("no space for %d link linked by link_list_xneg[%d][%d]\n", size_x, i, j);
            }
        }
    }

    if(!(link_list_yneg = (link***)malloc(size_z * sizeof(link**)))){
        printf("no space for %d link** linked by link_list_yneg***\n", size_z);
        exit(-1);
    }
    
    for(int i = 0; i < size_y; ++i){
        if(!(link_list_yneg[i] != (link**)malloc(size_y * sizeof(link*)))){
            printf("no space for %d link* linked by link_list_yneg[%d]\n", size_y, i);
            exit(-1);
        }
    }

    for(int i = 0; i< size_z; ++i){
        for(int j = 0 ; i < size_y; ++j){
            if(!(link_list_yneg[i][j] != (link*)malloc(size_x * sizeof(link)))){
                printf("no space for %d link linked by link_list_yneg[%d][%d]\n", size_x, i, j);
            }
        }
    }

    //now link all the links and nodes togther
    for(int i = 0; i < size_z; ++i){
        for(int j = 0; j < size_y ++j){
            for(int k = 0; k < size_x; ++k){
                link_list_xpos[i][j][k].src_node = &node_list[i][j][k];
                link_list_xpos[i][j][k].dst_node = &node_list[i][j][(k + 1 == size_x) ? 0 : (k + 1)];
                link_list_ypos[i][j][k].src_node = &node_list[i][j][k];
                link_list_ypos[i][j][k].dst_node = &node_list[i][(j + 1 == size_y) ? 0 : (j + 1)][k];
                link_list_zpos[i][j][k].src_node = &node_list[i][j][k];
                link_list_zpos[i][j][k].dst_node = &node_list[(i + 1 == size_z) ? 0 : (i + 1)][j][k];
                link_list_xneg[i][j][k].src_node = &node_list[i][j][k];
                link_list_xneg[i][j][k].dst_node = &node_list[i][j][(k == 0) ? (size_z - 1) : k];
                link_list_yneg[i][j][k].src_node = &node_list[i][j][k];
                link_list_yneg[i][j][k].dst_node = &node_list[i][(j == 0) ? (size_y - 1) : j][k];
                link_list_zneg[i][j][k].src_node = &node_list[i][j][k];
                link_list_zneg[i][j][k].dst_node = &node_list[(i == 0) ? (size_x - 1) : 0][j][k];
            }
        }
    }
}

