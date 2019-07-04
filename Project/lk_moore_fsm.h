#pragma once

#include "systemc.h"

enum ctrl_state {WAIT, EXECUTE, OUTPUT, FINISH}

SC_MODULE(lk_moore_fsm){
	sc_out <sc_logic> tmux_sel, amux_sel;

	//signals to tmux
	sc_in <sc_logic> t_EQ, t_GT, t_LT;

	//signals to amux
	sc_in <sc_logic> a_EQ, a_GT, a_LT;

	//clock
	sc_in_clk clock;

	sc_signal<ctrl_state> state, next_state;

	void state_reg();
	void state_diagram();

	SC_CTOR(lk_moore_fsm){
		SC_CTHREAD(state_reg, clock.pos());
		SC_METHOD(state_diagram);
		sensitive<<t_EQ<<t_GT<<t_LT<<a_EQ<<a_GT<<a_LT;
		//This does not need to be sensitive to all of these just the important ones (tbd)

	}

	void state_diagram(){
		//deassert mux selects

		switch(state.read()){
			case WAIT:

				break;
			case EXECUTE:

				break;

			case OUPUT:
				break;
			case FINISH:
				break;
			default:
				break;

		}

	}

};
