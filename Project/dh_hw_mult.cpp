#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"
#include <iomanip>

void dh_hw_mult::fsm()
{
	while(1){	
		state.write(next_state.read());
		wait();
	}
}

void dh_hw_mult::fsm_transition()
{
	//cout<<"FSM TRANSITION"<<endl;
		next_state.write(state.read());
		switch(state.read()){
			case WAIT:
				//cout<<"WAIT transition"<<endl;
				//Software asserts enable
				if(hw_mult_enable.read() == true){
					next_state.write(EXECUTE);
					//next_state.write(LOAD_IN);
				}
				break;
				
			case EXECUTE:				
				//cout<<"EXECUTE transition"<<endl;
				next_state.write(LOAD_IN);								
				break;
			
			case LOAD_IN:			
				//cout<<"LOAD_IN transition"<<endl;
				next_state.write(SELECT);
							
				break;				
					
			case SELECT:			
				next_state.write(LOAD_OUT);
				break;
				
			
			//wait for the output registers to be enabled
			case LOAD_OUT:			
				//cout<<"LOAD_OUT"<<endl;
				next_state.write(OUTPUT);
				break;
			
			//wait for software to deassert the enable	
			case OUTPUT:			
				//cout<<"OUTPUT transition" <<endl;
				if(hw_mult_enable.read()==false){
					next_state.write(FINISH);
				}
				break;
						
			case FINISH:
				//cout<<"FINISH transition"<<endl;
				next_state.write(WAIT);
				break;
				
			default:
				break;
		}//end of switch
		
}

//This function implements the behaviour on the datapath based on the FSM
//It does NOT determine the next state
void dh_hw_mult::fsm_out()
{	
	//perform default activities
	reg_load_in_enable.write(SC_LOGIC_0);
	reg_load_out_enable.write(SC_LOGIC_0);
	hw_mult_done.write(false);
	reset.write(false);
	
		switch(state.read()){
			case WAIT:
				//make sure reset is false
				//reset.write(false);
				//don't do anything				
				break;
			
			case EXECUTE:
				//perform multiplication
				reg_load_in_enable.write(SC_LOGIC_1);				
				break;

			case LOAD_IN:
				//cout<<"LOAD_IN output"<<endl;		
				break;

			case SELECT:
				//cout<<"SELECT output"<<endl;					
				break;
				
			case LOAD_OUT:
				//cout<<"LOAD_OUT output"<<endl;
				reg_load_out_enable.write(SC_LOGIC_1);
				break;

			case OUTPUT:
				//cout<<"OUTPUT output"<<endl;		
				hw_mult_done.write(true);
				//reg_load_out_enable.write(SC_LOGIC_1);
				break;

			case FINISH:
				//cout<<"finish output"<<endl;
				hw_mult_done.write(false);
				//clean everything up
				//reset.write(true);
				break;

			default:
				break;

		}
}



