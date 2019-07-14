#include "systemc.h"
#include "digit.h"
#include "lk_datapath.h"

#include "lk_register.h"
#include "lk_multiplier.h"
#include "lk_adder.h"
#include "lk_splitter.h"

#ifndef _DH_HW_MULT_H_
#define _DH_HW_MULT_H_ 1

enum ctrl_state {WAIT, EXECUTE, OUTPUT, FINISH};

SC_MODULE (dh_hw_mult)
{
	sc_in<bool> hw_mult_enable; 
	sc_in<NN_DIGIT> in_data_1;
	sc_in<NN_DIGIT> in_data_2;
	sc_out<NN_DIGIT> out_data_low;
	sc_out<NN_DIGIT> out_data_high;
	sc_out<bool> hw_mult_done;

	sc_in_clk hw_clock;

	sc_signal<ctrl_state> state, next_state;

	//lk_datapath datapath;
	//Content required for data path
	lk_register b_reg, c_reg;
	lk_splitter b_split, c_split;
	//blow*chigh corresponds to mult 0b01
	lk_multiplier mult0, mult1, mult2, mult3;

	sc_signal <NN_DIGIT> b_sig, c_sig;
	sc_signal <NN_HALF_DIGIT> blow, bhigh, clow, chigh;
	sc_signal <NN_HALF_DIGIT> t, alow, ahigh, u;

	//internal to hw_mult module - does not need to interact with demo
	sc_signal <bool> reset;
	sc_signal <bool> reg_load_enable;

  	void process_hw_mult();

	//function that simulates datapath - might need to be a thread for wait()s
	//Can you have a wait within a function that is not a thread?
	void hardware_mult();

	void fsm();

	//temporary function that performs original software multiplication
	void temp_mult();
  
  	SC_CTOR (dh_hw_mult): 	b_reg("b_reg"), c_reg("c_reg"), b_split("b_split"), c_split("c_split"),
							mult0("mult0"), mult1("mult1"), mult2("mult2"), mult3("mult3")
  	{ 
		b_reg.input(in_data_1);
		b_reg.output(b_sig);
		b_reg.clock(hw_clock);
		b_reg.reset(reset);
		b_reg.load_enable(reg_load_enable);

		c_reg.input(in_data_2);
		c_reg.output(c_sig);
		c_reg.clock(hw_clock);
		c_reg.reset(reset);
		c_reg.load_enable(reg_load_enable);

		b_split.input(b_sig);
		b_split.low(blow);
		b_split.high(bhigh);

		c_split.input(c_sig);
		c_split.low(clow);
		c_split.high(chigh);

		mult0.input1(blow);
		mult0.input2(clow);
		mult0.output(alow);

		mult1.input1(blow);
		mult1.input2(chigh);
		mult1.output(t);

		mult2.input1(bhigh);
		mult2.input2(clow);
		mult2.output(u);

		mult3.input1(bhigh);
		mult3.input2(chigh);
		mult3.output(ahigh);
		
		reg_load_enable = false;

		SC_CTHREAD (fsm, hw_clock.pos());	
		SC_THREAD(process_hw_mult);
    	sensitive<<state;
 	}
  
};

#endif /* end _DH_HW_MULT_H_ */
