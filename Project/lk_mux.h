#pragma once

#include "systemc.h"

SC_MODULE(lk_mux){

	sc_in <sc_logic> sel;
	sc_out <unsigned short int> out;

	void mux_process(){
		if(sel.read()==SC_LOGIC_1){
			out.write(1);
		}
		else{
		out.write(0)		
		}
	}

	SC_CTOR(lk_mux){
		SC_METHOD(mux_process);
		sensitive << sel;
	}

};
