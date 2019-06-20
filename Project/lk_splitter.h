#pragma once

#include "systemc.h"

SC_MODULE(lk_splitter){
	
	sc_in <unsigned int> input;
	sc_out <unsigned short int> low;
	sc_out <unsigned short int> high;

	//Not sure if this needs to be implemented with an AND gate?
	void split(){
		low = input & 0xFFFF;
		high = (input & 0xFFFF0000)>>16;
	}

	//Does this need to be sensitive to the clock?
	SC_CTOR(lk_splitter) {
		SC_METHOD(split);
		//sensitivities
		sensitive << input;
	}

};
