#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"
#include <iomanip>

void dh_hw_mult::fsm()
{
	while(1){
	//add some condition for a reset
		//cout<<"FSM"<<endl;		
		state.write(next_state.read());
		//mux_state.write(next_mux_state.read());
		wait();
	}
}

//This might need to be a clock thread
void dh_hw_mult::fsm_transition()
{
	//cout<<"FSM TRANSITION"<<endl;
		next_state.write(state.read());
		//next_mux_state.write(mux_state.read());
		switch(state.read()){
			case WAIT:
				cout<<"WAIT transition"<<endl;
				//Software asserts enable
				if(hw_mult_enable.read() == true){
					next_state.write(EXECUTE);
				}
				break;
			
			//not sure if this should contain anything or just drop in load in?	
			case EXECUTE:				
				cout<<"EXECUTE transition"<<endl;
				next_state.write(LOAD_IN);								
				break;
			
			case LOAD_IN:			
				cout<<"LOAD_IN transition"<<endl;
				next_state.write(SELECT);
				break;
			
			//Do some stuff with the mulitplexors
			case SELECT:						
				cout<<"SELECT transition"<<endl;
				if(a_LTE.read()==SC_LOGIC_0 &&	t_LTE.read()==SC_LOGIC_0){
					//next_mux_state.write(A0T0);
					next_state.write(A0T0);
				}
				if(a_LTE.read()==SC_LOGIC_0 &&	t_LTE.read()==SC_LOGIC_1){
					//next_mux_state.write(A0T1);
					next_state.write(A0T1);
				}
				if(a_LTE.read()==SC_LOGIC_1 &&	t_LTE.read()==SC_LOGIC_0){
					//next_mux_state.write(A1T0);
					next_state.write(A1T0);
				}
				if(a_LTE.read()==SC_LOGIC_1 &&	t_LTE.read()==SC_LOGIC_1){
					//next_mux_state.write(A1T1);
					next_state.write(A1T1);
				}
			
				//next_state.write(LOAD_OUT);
				break;
				
			case A0T0:
			case A0T1:
			case A1T0:
			case A1T1:
				next_state.write(LOAD_OUT);
				break;
			
			//wait for the output registers to be enabled
			case LOAD_OUT:			
				cout<<"LOAD_OUT"<<endl;
				next_state.write(OUTPUT);
				break;
			
			//wait for software to deassert the enable	
			case OUTPUT:			
				cout<<"OUTPUT transition" <<endl;
				if(hw_mult_enable.read()==false){
					next_state.write(FINISH);
				}
				break;
			
			//wait for hardware to deassert done
			case FINISH:
				cout<<"FINISH transition"<<endl;
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
	
	//currently handled by its own module - GCD shows mealy as controlling
	//tmux_sel.write(SC_LOGIC_0);
	//amux_sel.write(SC_LOGIC_0);
	reg_load_in_enable.write(SC_LOGIC_0);
	reg_load_out_enable.write(SC_LOGIC_0);
	//reset.write(false);

		switch(state.read()){
			case WAIT:
				//don't do anything
				
				break;

			case EXECUTE:
				//perform multiplication
				reg_load_in_enable.write(SC_LOGIC_1);				
				break;

			case LOAD_IN:
				cout<<"LOAD_IN output"<<endl;				
				cout<<"b: "<< b_sig.read() << " c: " <<c_sig.read()<<endl;
				
				//cout<<"t+u: "<< t_plus_u<< " u: "<<u<<endl;
				break;

			case SELECT:
				cout<<"SELECT output"<<endl;	
				cout<<"t + u: "<<t_plus_u<<" t shifted up:"<<t_shifted_up<<" a[0]: "<<alow<<" a[1]: "<<ahigh0<<" u: "<<u<<endl;
				cout<<"t_shifted_up plus alow: "<< t_plus_alow<<endl;
				cout<<"data out low: "<<out_data_low<<endl;
				/*switch(mux_state.read()){
					case A0T0:
						cout<<"A0T0"<<endl;
						tmux_sel.write(SC_LOGIC_0);
						amux_sel.write(SC_LOGIC_0);
						break;
					case A0T1:
						cout<<"A0T1"<<endl;
						amux_sel.write(SC_LOGIC_0);
						tmux_sel.write(SC_LOGIC_1);
						break;
					case A1T0:
						cout<<"A1T0"<<endl;
						amux_sel.write(SC_LOGIC_1);
						tmux_sel.write(SC_LOGIC_0);
						break;
					case A1T1:
						cout<<"A1T1"<<endl;
						amux_sel.write(SC_LOGIC_1);
						tmux_sel.write(SC_LOGIC_1);
						break;
					default:
						break;
				}		*/	

				break;
				
			case A0T0:
						cout<<"A0T0"<<endl;
						tmux_sel.write(SC_LOGIC_0);
						amux_sel.write(SC_LOGIC_0);
						break;
					case A0T1:
						cout<<"A0T1"<<endl;
						amux_sel.write(SC_LOGIC_0);
						tmux_sel.write(SC_LOGIC_1);
						break;
					case A1T0:
						cout<<"A1T0"<<endl;
						amux_sel.write(SC_LOGIC_1);
						tmux_sel.write(SC_LOGIC_0);
						break;
					case A1T1:
						cout<<"A1T1"<<endl;
						amux_sel.write(SC_LOGIC_1);
						tmux_sel.write(SC_LOGIC_1);
						break;

			case LOAD_OUT:
				cout<<"LOAD_OUT output"<<endl;
				//cout<<"data out low: "<<out_data_low<<endl;
				reg_load_out_enable.write(SC_LOGIC_1);
				
				break;

			case OUTPUT:
				cout<<"OUTPUT output"<<endl;
				cout<<"data out low: "<<out_data_low.read()<<endl;
				//cout<<"t shifted up: "<<t_shifted_up<<endl;
				//cout<<"t plus alow: "<<t_plus_alow<<endl;
				reg_load_out_enable.write(SC_LOGIC_1);
				hw_mult_done.write(true);
				
				break;

			case FINISH:
				cout<<"finish output"<<endl;
				hw_mult_done.write(false);
				//clean everything up
				//reset.write(true);
				break;

			default:
				break;

		}
}

void dh_hw_mult::temp_mult(){

//ORIGINAL CODE
	
  NN_DIGIT a[2], b, c, t, u;
  NN_HALF_DIGIT bHigh, bLow, cHigh, cLow;
  
    cout<<"Multiplication method"<<endl;
	// Read inputs	
	b = in_data_1.read();
	c = in_data_2.read();
		
	// Original code from NN_DigitMult()...		
  	bHigh = (NN_HALF_DIGIT)HIGH_HALF (b);
  	bLow = (NN_HALF_DIGIT)LOW_HALF (b);
  	cHigh = (NN_HALF_DIGIT)HIGH_HALF (c);
  	cLow = (NN_HALF_DIGIT)LOW_HALF (c);

  	a[0] = (NN_DIGIT)bLow * (NN_DIGIT)cLow;
  	t = (NN_DIGIT)bLow * (NN_DIGIT)cHigh;
  	u = (NN_DIGIT)bHigh * (NN_DIGIT)cLow;
  	a[1] = (NN_DIGIT)bHigh * (NN_DIGIT)cHigh;
  
  	if ((t += u) < u) a[1] += TO_HIGH_HALF (1);
  	u = TO_HIGH_HALF (t);
  
  	if ((a[0] += u) < u) a[1]++;
  	a[1] += HIGH_HALF (t);
	
	// Write outputs
	out_data_low.write(a[0]);
	out_data_high.write(a[1]);

	//Handled by other states
	//hw_mult_done.write(true);
	//wait for enable to deassert
	/*while(hw_mult_enable.read() == true){
		wait();
	}
	hw_mult_done.write(false);*/
	  	  
}

