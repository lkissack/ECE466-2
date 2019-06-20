#pragma once
#include "systemc.h"

SC_MODULE(lk_register) {
	//Moves input to output at clock cycle

	sc_in <float> input;
	sc_in <bool> reset;
	sc_out <float> output;
	sc_in_clk clock;

	//Updates with clock pulse - CTHREAD
	void update_register() {
		while (1) {
			if (reset == true) {
				output.write(0);
			}else{
				output.write(input.read());
				//std::cout << "Reg:" << input.read()<<endl;
			}
			
			wait();
		}
	}


	SC_CTOR(lk_register) {
		SC_CTHREAD(update_register, clock.pos());
	}

};
