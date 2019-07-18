#include "systemc.h"
#include "digit.h"

SC_MODULE(lk_lessthan){
	sc_in <NN_DIGIT> input1;
	sc_in <NN_DIGIT> input2;
	sc_out <sc_logic> GT;
	sc_out <sc_logic> LTE;
	
	//sets output to true if input1 is lessthan input2
	void lessthan(){
		if(input1 < input2){
			LTE.write(SC_LOGIC_1);
			GT.write(SC_LOGIC_0);
		}
		else{
			LTE.write(SC_LOGIC_0);
			GT.write(SC_LOGIC_1);
		}
	}
	
	SC_CTOR(lk_lessthan){
		SC_METHOD(lessthan);
		sensitive << input1 <<input2;
	}
	
};

