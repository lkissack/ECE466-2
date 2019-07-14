#pragma once

#include "systemc.h"
#include "digit.h"
#include <iomanip>

SC_MODULE(lk_mux){

	sc_in <bool> sel;//should this be sc_logic?
	sc_out <NN_DIGIT> out;

	void mux_process(){
	//SC_LOGIC_1
		if(sel.read()==true){
			out.write(1);
		}
		else{
		out.write(0);		
		}
		cout<<"mux ran"<<endl;
	}

	SC_CTOR(lk_mux){
		SC_METHOD(mux_process);
		sensitive << sel;
	}

};
