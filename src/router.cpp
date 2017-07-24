#include "router.h"

void router::router_init(int Cur_x, int Cur_y, int Cur_z, int SA_Mode, int Routing_mode, int Injection_mode, flit** In, flit** Inject){
    cur_x = Cur_x;
    cur_y = Cur_y;
    cur_z = Cur_z;

    SA_mode = SA_Mode;
    routing_mode = Routing_mode;
	injection_mode = Injection_mode;


    credit_period_counter = 0;

    for(int i = 0; i < PORT_NUM; ++i){
        in[i] = In[i];
        inject[i] = Inject[i];
        upstream_credits[i] = 0;
        downstream_credits[i] = IN_Q_SIZE - 1;
        out[i].valid = false;
        eject[i].valid = false;
        inject_avail[i] = true;
		inject_latch[i].valid = false;
		in_latch[i].valid = false;
    }

	for (int i = 0; i < PORT_NUM; ++i) {
		eject_ptrs[i] = &eject[i];
		inject_avail_ptrs[i] = &out_avail_for_inject[i];
	}

	app_core.local_unit_init(cur_x, cur_y, cur_z, injection_mode, INJECTION_GAP, PACKET_SIZE, PACKET_NUM, eject_ptrs, inject_avail_ptrs);

    //allocate space for those subentities need dynamic allocation

    //init and connect all the subentities
    for(int i = 0; i < PORT_NUM; ++i){
        in_avail_from_RC[i] = &(RC_list[i].in_avail);
        input_buffer_list[i].fifo_init(IN_Q_SIZE, &in_latch[i], in_avail_from_RC[i]);
    }

    for(int i = 0; i < PORT_NUM; ++i){
        flit_list_to_RC[i] = &(input_buffer_list[i].out);
        in_avail_from_VA[i] = &(VCs_list[i].in_avail);
        RC_list[i].routing_comp_init(cur_x, cur_y, cur_z, i + 1, routing_mode, flit_list_to_RC[i], in_avail_from_VA[i]);
    }

    for(int i = 0; i < PORT_NUM; ++i){
        flit_list_to_VA[i] = &(RC_list[i].out);
        VCs_list[i].VCs_init(i + 1, flit_list_to_VA[i], &xbar);
    }

    for(int i = 0; i < PORT_NUM * VC_NUM; ++i){
        flit_list_to_SA[i] = &(VCs_list[i / VC_NUM].out[i % VC_NUM]);
    }
    for(int i = 0; i < PORT_NUM; ++i){
        in_avail_from_ST[i] = &(out_avail_for_passthru[i]); //initially this is for injetion   
    }
	xbar.crossbar_switch_init(SA_mode, flit_list_to_SA, in_avail_from_ST);


    //init all avails
    for(int i = 0; i < PORT_NUM; ++i){
        occupy_by_inject[i] = false;
		occupy_by_passthru[i] = false;
        downstream_avail[i] = true;
        out_avail_for_passthru[i] = true;
        out_avail_for_inject[i] = true;
    }

}

void router::consume(){
    //latch in and inject
    for(int i = 0; i < PORT_NUM; ++i){
        if(in[i]->valid && in[i]->flit_type == CREDIT_FLIT){
            downstream_credits[i] = in[i]->payload;
            in_latch[i].valid = false;
        }
        else{
            in_latch[i] = *(in[i]);
        }
      //  inject_latch[i] = *(inject[i]);
    }
    



    //call all the consume for the subentities
    for(int i = 0; i < PORT_NUM; ++i){
        input_buffer_list[i].consume();
    }

    for(int i = 0; i < PORT_NUM; ++i){
        RC_list[i].consume();
    }

    for(int i = 0; i < PORT_NUM; ++i){
        VCs_list[i].consume();
    }

    xbar.consume();  
	app_core.consume();


}

void router::produce(){
    //call all the produce fuctions for all the sub modules
    //
    for(int i = 0; i < PORT_NUM; ++i){
        input_buffer_list[i].produce();
    }
    for(int i = 0; i < PORT_NUM; ++i){
        RC_list[i].produce();
    }
    for(int i = 0; i < PORT_NUM; ++i){
        VCs_list[i].produce();
    }
    xbar.produce();
	app_core.produce();

    //increase the credit_period_counter
    
    if(credit_period_counter < CREDIT_BACK_PERIOD)
        credit_period_counter++;
    else
        credit_period_counter = 0;
    

    //decide all of the out_avail(s)
	for (int i = 0; i < PORT_NUM; ++i) {
		if (credit_period_counter != CREDIT_BACK_PERIOD - 1) {
			if (downstream_credits[i] >= CREDIT_THRESHOlD) {
				if (occupy_by_inject[i]) {
					out_avail_for_passthru[i] = false;
				}
				else if (xbar.out[i].valid || occupy_by_passthru[i]) {	
					out_avail_for_passthru[i] = true;
				}
				else {
					out_avail_for_passthru[i] = true;
				}
				//				if (!occupy_by_inject[i]){
				//					out_avail_for_passthru[i] = true;
				//					out_avail_for_inject[i] = false;
				//				}
			}
			else {
				out_avail_for_passthru[i] = false;
			}
		}
		else {
			//time to send back credit_period_counter
			out_avail_for_passthru[i] = false;
		}
	}

    for(int i = 0; i < PORT_NUM; ++i){
        if(credit_period_counter != CREDIT_BACK_PERIOD - 1){
			if (downstream_credits[i] >= CREDIT_THRESHOlD){
				if (occupy_by_inject[i]){
                    out_avail_for_inject[i] = true;
//					out_avail_for_passthru[i] = false;
                }
				else if (xbar.out[i].valid || occupy_by_passthru[i]){
					out_avail_for_inject[i] = false;
//					out_avail_for_passthru[i] = true;
				}
				else{
					out_avail_for_inject[i] = true;
//					out_avail_for_passthru[i] = true;
				}
//				if (!occupy_by_inject[i]){
//					out_avail_for_passthru[i] = true;
//					out_avail_for_inject[i] = false;
//				}
            }
            else{
//                out_avail_for_passthru[i] = false;
                out_avail_for_inject[i] = false;
            }
            
        }
        else{
            //time to send back credit_period_counter
 //           out_avail_for_passthru[i] = false;
            out_avail_for_inject[i] = false;
        }
    }

    //update the out
    for(int i = 0; i < PORT_NUM; ++i){
        upstream_credits[i] = IN_Q_SIZE - 1 - input_buffer_list[i].usedw;
        if(credit_period_counter == CREDIT_BACK_PERIOD - 1){
            out[i].valid = true;
            out[i].flit_type = CREDIT_FLIT;
            out[i].payload = upstream_credits[i];
        }
		else if (xbar.out[i].valid && (!(occupy_by_inject[i]))){
			out[i] = xbar.out[i];
		}
		else if ((!(xbar.out[i].valid)) && occupy_by_passthru[i]) {
			out[i] = xbar.out[i];
		}
//		else if ((!xbar.out[i].valid) && (!(occupy_by_inject[i])))
//			out[i].valid = false;
		else 
			out[i] = app_core.inject[i];

    }

	//update the eject
	for (int i = 0; i < PORT_NUM; ++i){
		eject[i] = RC_list[i].flit_eject;
	}
	//update occupy_by_passthru
	for (int i = 0; i < PORT_NUM; ++i){
		if (xbar.out[i].valid && (!occupy_by_inject[i]) && xbar.out[i].flit_type == HEAD_FLIT)
			occupy_by_passthru[i] = true;
		else if (xbar.out[i].valid && xbar.out[i].flit_type == TAIL_FLIT && credit_period_counter != CREDIT_BACK_PERIOD - 1)
			occupy_by_passthru[i] = false;
	}
	//update occupy_by_inject
	for (int i = 0; i < PORT_NUM; ++i){
		if (app_core.inject[i].valid && (!occupy_by_passthru[i]) && app_core.inject[i].flit_type == HEAD_FLIT){
			occupy_by_inject[i] = true;
		}
		else if (app_core.inject[i].valid && app_core.inject[i].flit_type == TAIL_FLIT && credit_period_counter != CREDIT_BACK_PERIOD - 1){
			occupy_by_inject[i] = false;
		}
	}





}

void router::router_free(){
    for(int i = 0; i < PORT_NUM; ++i){
        input_buffer_list[i].fifo_free();
		VCs_list[i].VCs_free();
    }

    xbar.crossbar_switch_free();
}

