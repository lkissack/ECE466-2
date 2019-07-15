#pragma once

#include "systemc.h"
#include "digit.h"
#include <iomanip>

SC_MODULE(lk_mux){

	sc_in <bool> sel;//should this be sc_logic?
	sc_out <NN_DIGIT> out;
	sc_in_clk clock;

	void mux_process(){
	//SC_LOGIC_1
		if(sel.read()==true){
			out.write(1);
		}
		else{
		out.write(0);		
		}
	}

	SC_CTOR(lk_mux){
		SC_CTHREAD(mux_process, clock.pos());
	}

};
