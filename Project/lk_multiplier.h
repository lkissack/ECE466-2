#pragma once

#include "systemc.h"

SC_MODULE(lk_multiplier) {
	
	sc_in <unsigned short int> input1;
	sc_in <unsigned short int> input2;
	sc_out <unsigned short int> output;

	void multiply() {
		output.write(input1.read()*input2.read());
	}

	//should probably switch to clock
	SC_CTOR(lk_multiplier) {
		SC_METHOD(multiply);
		sensitive << input1<<input2;
	}

};
