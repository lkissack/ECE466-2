#pragma once

#include "systemc.h"
#include <iomanip>

enum ctrl_state {WAIT, EXECUTE, OUTPUT, FINISH}

SC_MODULE(lk_moore_fsm){
	//sc_out <sc_logic> tmux_sel, amux_sel;

	//signals to tmux
	//sc_in <sc_logic> t_EQ, t_GT, t_LT;

	//signals to amux
	//sc_in <sc_logic> a_EQ, a_GT, a_LT;

	//clock
	sc_in_clk clock;

	//Done and Enable signals for handshaking
	sc_in <bool> enable;
	sc_out <bool> done;

	sc_signal<ctrl_state> state, next_state;

	void state_reg();
	void state_diagram();

	SC_CTOR(lk_moore_fsm){
		SC_CTHREAD(state_reg, clock.pos());
		SC_METHOD(state_diagram);
		sensitive<<enable<<done;
		//This does not need to be sensitive to all of these just the important ones (tbd)
		//

	}

	void state_reg(){
		while(1){
			//Not sure about this
			if(enable && !done){
				state.write(EXECUTE);
			}
			else{
				state.write(next_state.read());			
			}
		
		}
	}

	void state_diagram(){
		//perform default activities
		//deassert muxes?
		next_state.write(state.read());
		

		switch(state.read()){
			case WAIT:
				//don't do anything
				cout<<"Wait state"<<endl;
				break;
			case EXECUTE:
				//perform multiplication
				cout<<"Execute state"<<endl;

				//assert done signal?
				done.write(true);
				//set next state to output?
				next_state.write(OUTPUT);
				break;

			case OUPUT:
			cout<<"Output state" <<endl;
				//set next state to FINISH
				next_state.write(FINISH);
				break;
			case FINISH:
				cout<<"Finish state"<<endl;
				//set next state to WAIT
				next_state.write(WAIT);
				break;
			default:
				break;

		}

	}

};
