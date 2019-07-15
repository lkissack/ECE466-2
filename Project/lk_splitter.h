#pragma once

#include "systemc.h"
#include "digit.h"
#include <iomanip>

SC_MODULE(lk_splitter){
	
	sc_in <NN_DIGIT> input;
	sc_out <NN_HALF_DIGIT> low;
	sc_out <NN_HALF_DIGIT> high;

	//Not sure if this needs to be implemented with an AND gate?
	void split(){
		NN_DIGIT in = input.read();
		NN_HALF_DIGIT lo = (NN_HALF_DIGIT) (in & 0xFFFF);
		NN_HALF_DIGIT hi = (NN_HALF_DIGIT) ((in & 0xFFFF0000)>>16);
		low.write(lo);
		high.write(hi);
		//debug with lo and hi, since other values aren't updated yet
		//cout<<"split input: "<<in<<endl;
		//cout<<"low: " << lo<< " high: "<<hi<<endl;
	}

	SC_CTOR(lk_splitter) {
		SC_METHOD(split);
		sensitive << input;
	}

};
