#pragma once

#include "systemc.h"
#include "lk_splitter.h"
#include "lk_multiplier.h"
#include "lk_adder.h"

SC_MODULE(DigitMult){

	//Done and Enable signals for handshaking
	sc_in <bool> enable;
	sc_out <bool> done;

	sc_in <unsigned int> b,c;
	sc_out <unsigned int> a;
	sc_in_clk hardware_clock;

	//sc_signal since they are used by the modules?
	sc_signal<unsigned short int> bhigh, blow, chigh, clow, t, u;

	//modules
	lk_splitter b_splitter, c_splitter;
	//[b,c] mult 0 = blow*clow
	lk_multiplier mult0, mult1, mult2, mult3;
	lk_adder add0, add1, add2, add3;

	//comparator X2
	//multiplexer to implement FSM

	//another module for FSM?


	SC_CTOR(DigitMult): 	b_splitter("b_splitter"), c_splitter("c_splitter"),
				mult0("mult0"), mult1("mult1"), mult2("mult2"), mult3("mult3")
			{
		
		b_splitter.in(b);
		b_splitter.high(bhigh);
		b_splitter.low(blow);

		c_splitter.in(c);
		c_splitter.high(chigh);
		c_splitter.low(clow);

		mult0.input1(blow);
		mult0.input2(clow);
		//mult0.output = a[0];

		mult1.input1(blow);
		mult1.input2(chigh);
		mult1.output(t);

		mult2.input1(bhigh);
		mult2.input2(clow);
		mult2.output(u);

		mult3.input1(bhigh);
		mult3.input2(chigh);
		//mult3.output(u); = a[1];

	}


	

};
