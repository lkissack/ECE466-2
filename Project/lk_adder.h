#pragma once
#include "systemc.h"

SC_MODULE(lk_adder) {

	sc_in <float_t> input1, input2;
	sc_out <float_t> output;

	void add() {
		output.write(input1.read() + input2.read());
	}

	SC_CTOR(lk_adder) {
		SC_METHOD(add);
		//sensitivities
		sensitive << input1 << input2;
	}

};