#pragma once
#include "systemc.h"
#include <iomanip>

using std::setw;

SC_MODULE (mon) {
	sc_in <float> X, Y;
	sc_in <bool> Reset;
	sc_in_clk clock;

	void monitor() {
		cout << setw(10) << "Time";
		cout << setw(2) << "X" << setw(15) << "Y" << setw(10) << "Reset" << endl;
		while (true) {
			cout << setw(10) << sc_time_stamp();
			cout << setw(2) << X.read() << setw(15) << Y.read();
			cout << setw(10) << Reset.read() << endl;
			wait();// wait for 1 clock cycle
		}
	} 

	SC_CTOR (mon) {  
		SC_CTHREAD (monitor, clock.pos());  
	}
};
