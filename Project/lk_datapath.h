#pragma once
/*
#include "systemc.h"
#include "lk_register.h"
#include "lk_multiplier.h"
#include "lk_adder.h"
#include "lk_splitter.h"

SC_MODULE(lk_datapath){

	//Registers for inputs b, c
	lk_register b_reg, c_reg;

	//Registers for outputs a[0], a[1]
	lk_register a_low, a_high;

	//Clock for hardware
	sc_in_clk hw_clock;

	lk_splitter b_splitter, c_splitter;

	//[b,c] mult 01 = blow*chigh
	lk_multiplier mult0, mult1, mult2, mult3;
	lk_adder add0, add1, add2, add3;


	SC_CTOR(lk_datapath): 	b_reg("b_reg"), c_reg("c_reg"),a_low("a_low"), 					a_high("a_high"),b_splitter("b_splitter"), 					c_splitter("c_splitter"),
				mult0("mult0"), mult1("mult1"), mult2("mult2"), mult3("mult3"),
				add0("add0"), add1("add1"), add2("add2"), add3("add3")
				
	{
	//connect everything appropriately
	
	}

};*/
