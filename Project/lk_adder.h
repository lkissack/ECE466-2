#pragma once
#include "systemc.h"
#include <iomanip>

SC_MODULE(lk_adder) {

	sc_in <NN_DIGIT> input1, input2;
	sc_out <NN_DIGIT> output;

	void add() {
		NN_DIGIT out = input1.read() + input2.read();
		output.write(out);
		/*
		cout<<"adder"<<endl;
		cout<<"in: "<<input1.read()<<" + "<<input2.read()<<endl;
		cout<<"out: "<<out<<endl;*/
	}
//should probably switch to clock
	SC_CTOR(lk_adder) {
		SC_METHOD(add);
		//sensitivities
		sensitive << input1 << input2;
	}

};
