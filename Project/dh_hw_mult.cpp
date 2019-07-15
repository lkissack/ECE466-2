#include "systemc.h"
#include "digit.h"
#include "dh_hw_mult.h"
#include <iomanip>

void dh_hw_mult::fsm()
{
	while(1){
		//cout<<"FSM state: "<< sc_time_stamp()<<" @ "<< state.read()<<endl;
		if(hw_mult_enable.read()==true && state.read()==WAIT){
			//cout<<"Entering Execute @ "<<sc_time_stamp()<<endl;			
			state.write(EXECUTE);
		}else{
			state.write(next_state);
		}
		//The clock will wake this thread up again		
		wait();
	}
}

//This is where FSM will be implemented
void dh_hw_mult::process_hw_mult()
{	//Need while loop, otherwise thread will die :(
	while(1){
	//perform default activities
	
	next_state.write(state.read());

		switch(state.read()){
			case WAIT:
				//don't do anything - fsm() will move to execute once enabled
				cout<<"WAIT"<<endl;
				break;

			case EXECUTE:
				//perform multiplication
				cout<<"EXECUTE"<<endl;
				reg_load_in_enable = true;
				next_state.write(LOAD_IN);
				break;

			case LOAD_IN:
				cout<<"LOAD_IN"<<endl;				
				cout<<"b: "<< b_sig.read() << " c: " <<c_sig.read()<<endl;
				next_state.write(SELECT);
				break;

			case SELECT:				
				cout<<"SELECT"<<endl;
				cout<<"t: "<<t<<" a[0]: "<<alow<<" a[1]: "<<ahigh0<<" u: "<<u<<endl;
				next_state.write(LOAD_OUT);
				//wait one clock cycle for the muxes to have to right values
				// wait another cycle before outputting the outputs
				break;

			case LOAD_OUT:
				cout<<"LOAD_OUT"<<endl;
				reg_load_out_enable = true;
				//hw_mult_done.write(true);
				
				//set next state to output?
				next_state.write(OUTPUT);
				break;

			case OUTPUT:
				cout<<"OUTPUT" <<endl;
				hw_mult_done.write(true);
				//set next state to FINISH
				while(hw_mult_enable.read()==true){
					wait();
				}
				next_state.write(FINISH);
				break;

			case FINISH:
				cout<<"FINISH"<<endl;
				//set next state to WAIT
				hw_mult_done.write(false);
				next_state.write(WAIT);
				break;

			default:
				break;

		}
		cout<<"Wait() for next state change"<<endl;
		wait();
	}

}

void dh_hw_mult::hardware_mult(){

	
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

