#include "link.h"
#include "node.h"
#include<stdlib.h>

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
        //constructor
        network(int x, int y, int z);
};

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
            for(int k = 0; k < size_z; ++k){
                link_list_xpos[i][j][k]a
                }
        }
    }
}
