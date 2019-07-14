#include "systemc.h"
#include "digit.h"

SC_MODULE(lk_lessthan){
	sc_in <NN_DIGIT> input1;
	sc_in <NN_DIGIT> input2;
	sc_out <bool> output;
	
	//sets output to true if input1 is lessthan input2
	void lessthan(){
		if(input1 < input2){
			output.write(true);
		}
		else{
			output.write(false);
		}
	}
	
	SC_CTOR(lk_lessthan){
		SC_METHOD(lessthan);
		sensitive << input1 <<input2;
	}
	
};

