#pragma once

#include "systemc.h"
#include "digit.h"
#include <iomanip>

SC_MODULE(lk_mux){

	sc_in <sc_logic> sel;
	sc_in <NN_DIGIT> in;
	sc_out <NN_DIGIT> out;
	sc_in_clk clock;

	void mux_process(){
		if(sel.read()==SC_LOGIC_1){
			out.write(in.read());
		}
		else{
		out.write(0);		
		}
	}

	SC_CTOR(lk_mux){
		SC_CTHREAD(mux_process, clock.pos());
	}

};
