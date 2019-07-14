#pragma once
#include "systemc.h"
#include "digit.h"

SC_MODULE(lk_shift) {

	sc_in <NN_DIGIT> input;
	sc_out <NN_DIGIT> output;

	void shift() {
		output.write(input.read() << 16);
	}
//should probably switch to clock
	SC_CTOR(lk_shift) {
		SC_METHOD(shift);
		//sensitivities
		sensitive << input;
	}

};
