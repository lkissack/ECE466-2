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
			
			//not sure if this should contain anything or just drop in load in?	
			case EXECUTE:				
				//cout<<"EXECUTE transition"<<endl;
				next_state.write(LOAD_IN);								
				break;
			
			case LOAD_IN:			
				//cout<<"LOAD_IN transition"<<endl;
				next_state.write(SELECT);
							
				break;				
					
			case SELECT:
			{
				sc_logic a_IN = a_LTE.read();
				sc_logic t_IN = t_LTE.read();						
				//cout<<"SELECT transition"<<endl;
				if(a_IN==SC_LOGIC_0 &&	t_IN==SC_LOGIC_0){
					next_state.write(A0T0);
				}
				else if(a_IN==SC_LOGIC_0 &&	t_IN==SC_LOGIC_1){
					next_state.write(A0T1);
				}
				else if(a_IN==SC_LOGIC_1 &&	t_IN==SC_LOGIC_0){
					next_state.write(A1T0);
				}
				else if(a_IN==SC_LOGIC_1 &&	t_IN==SC_LOGIC_1){
					next_state.write(A1T1);
				}
			
				break;
				}//BRACKETS FOR SWITCH SO VARIABLES CAN BE DEFINED
				
			case A0T0:
			case A0T1:
			case A1T0:
			case A1T1:
				
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
	tmux_sel.write(SC_LOGIC_0);
	amux_sel.write(SC_LOGIC_0);
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
				//reg_load_in_enable.write(SC_LOGIC_1);
				break;

			case SELECT:{
				//cout<<"SELECT output"<<endl;	
				/*sc_logic a_IN = a_LTE.read();
				sc_logic t_IN = t_LTE.read();	
				if(a_IN==SC_LOGIC_0 &&	t_IN==SC_LOGIC_0){
					tmux_sel.write(SC_LOGIC_0);
					amux_sel.write(SC_LOGIC_0);
				}
				else if(a_IN==SC_LOGIC_0 &&	t_IN==SC_LOGIC_1){
					amux_sel.write(SC_LOGIC_0);
					tmux_sel.write(SC_LOGIC_1);
				}
				else if(a_IN==SC_LOGIC_1 &&	t_IN==SC_LOGIC_0){
					amux_sel.write(SC_LOGIC_1);
					tmux_sel.write(SC_LOGIC_0);
				}
				else if(a_IN==SC_LOGIC_1 &&	t_IN==SC_LOGIC_1){
					amux_sel.write(SC_LOGIC_1);
					tmux_sel.write(SC_LOGIC_1);
				}*/

				break;}
				
			case A0T0:
				//cout<<"A0T0"<<endl;
				tmux_sel.write(SC_LOGIC_0);
				amux_sel.write(SC_LOGIC_0);
				break;
			case A0T1:
				//cout<<"A0T1"<<endl;
				amux_sel.write(SC_LOGIC_0);
				tmux_sel.write(SC_LOGIC_1);
				break;
			case A1T0:
				//cout<<"A1T0"<<endl;
				amux_sel.write(SC_LOGIC_1);
				tmux_sel.write(SC_LOGIC_0);
				break;
			case A1T1:
				//cout<<"A1T1"<<endl;
				amux_sel.write(SC_LOGIC_1);
				tmux_sel.write(SC_LOGIC_1);
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



