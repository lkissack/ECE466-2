#pragma once
#include "systemc.h"

SC_MODULE(lk_adder) {

	sc_in <NN_DIGIT> input1, input2;
	sc_out <NN_DIGIT> output;

	void add() {
		output.write(input1.read() + input2.read());
	}
//should probably switch to clock
	SC_CTOR(lk_adder) {
		SC_METHOD(add);
		//sensitivities
		sensitive << input1 << input2;
	}

};
