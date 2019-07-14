#pragma once

#include "systemc.h"
#include "digit.h"

SC_MODULE(lk_splitter){
	
	sc_in <NN_DIGIT> input;
	sc_out <NN_HALF_DIGIT> low;
	sc_out <NN_HALF_DIGIT> high;

	//Not sure if this needs to be implemented with an AND gate?
	void split(){
		low = input & 0xFFFF;
		high = (input & 0xFFFF0000)>>16;
	}

	SC_CTOR(lk_splitter) {
		SC_METHOD(split);
		sensitive << input;
	}

};
