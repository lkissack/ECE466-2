#pragma once
#include "systemc.h"
#include "digit.h"
#include <iomanip>

SC_MODULE(lk_register) {
	sc_in <NN_DIGIT> input;
	sc_in <bool> reset;
	sc_out <NN_DIGIT> output;
	sc_in_clk clock;
	sc_in <sc_logic> load_enable;

	//Updates with clock pulse - CTHREAD
	void update_register() {
		while (1) {
			if (reset == true) {
				output.write(0);
			}
			else if(load_enable == false){
				//maintain current output
			}
			else{
				output.write(input.read());
				//cout << "Reg: " << input.read()<<endl;
			}
			wait();
		}
	}


	SC_CTOR(lk_register) {
		SC_CTHREAD(update_register, clock.pos());
	}

};
