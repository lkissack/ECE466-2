#pragma once

#include "systemc.h"

SC_MODULE(lk_multiplier) {
	
	sc_in <NN_HALF_DIGIT> input1;
	sc_in <NN_HALF_DIGIT> input2;
	sc_out <NN_DIGIT> output;

	void multiply() {
		output.write(((NN_DIGIT)input1.read())*((NN_DIGIT)input2.read()));
	}

	//should probably switch to clock
	SC_CTOR(lk_multiplier) {
		SC_METHOD(multiply);
		sensitive << input1<<input2;
	}

};
