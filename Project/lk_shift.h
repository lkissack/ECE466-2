#pragma once
#include "systemc.h"
#include "digit.h"
#include <iomanip>

SC_MODULE(lk_shift) {

	sc_in <NN_DIGIT> input;
	sc_in <bool> direction;//1 = left, 0 = right
	sc_out <NN_DIGIT> output;

	void shift() {
		NN_DIGIT in = input.read();
		NN_DIGIT out;
		if(direction.read() ==true){
			out = (in << 16);
		}
		else{
			out = in >>16;
		}
		output.write(out);
		//cout<<"Shift: "<<in<<" direction: "<<direction.read()<<endl;
		//cout<<"output: "<< out<<endl;
	}
//should probably switch to clock
	SC_CTOR(lk_shift) {
		SC_METHOD(shift);
		//sensitivities
		sensitive << input;
	}

};
