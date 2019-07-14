#include "systemc.h"
#include "digit.h"
#include "lk_datapath.h"

#include "lk_register.h"
#include "lk_multiplier.h"
#include "lk_adder.h"
#include "lk_splitter.h"
#include "lk_lessthan.h"
#include "lk_mux.h"
#include "lk_shift.h"

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

	lk_adder add0, add1, add2, add3;
	lk_lessthan comp0, comp1;
	lk_mux tmux, amux;
	lk_shift tmux_shift, t_shift;

	sc_signal <NN_DIGIT> b_sig, c_sig;
	sc_signal <NN_HALF_DIGIT> blow, bhigh, clow, chigh;
	sc_signal <NN_DIGIT> t, alow, ahigh0, ahigh1, ahigh2, u;
	sc_signal <NN_DIGIT> t_plus_u, t_plus_alow;
	sc_signal <NN_DIGIT> tmux_out, amux_out;
	sc_signal <NN_DIGIT> tmux_shifted, t_shifted;

	//internal to hw_mult module - does not need to interact with demo
	sc_signal <bool> reset;
	sc_signal <bool> reg_load_enable;
	//not sure if this should be bool or sc_logic?
	sc_signal <bool> tmux_sel, amux_sel;

  	void process_hw_mult();

	//function that simulates datapath - might need to be a thread for wait()s
	//Can you have a wait within a function that is not a thread?
	void hardware_mult();

	void fsm();

	//temporary function that performs original software multiplication
	void temp_mult();
  
  	SC_CTOR (dh_hw_mult): 	b_reg("b_reg"), c_reg("c_reg"), b_split("b_split"), c_split("c_split"),
							mult0("mult0"), mult1("mult1"), mult2("mult2"), mult3("mult3"),
							add0("add0"), add1("add1"),add2("add2"),add3("add3"),
							comp0("comp0"), comp1("comp1"),
							tmux("tmux"), amux("amux"),
							tmux_shift("tmux_shift"), t_shift("t_shift")
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
		mult3.output(ahigh0);

		add0.input1(t);
		add0.input2(u);
		add0.output(t_plus_u);

		//order of inputs matters
		comp0.input1(t_plus_u);
		comp0.input2(u);
		comp0.output(tmux_sel);

		tmux.sel(tmux_sel);
		tmux.out(tmux_out);
		
		tmux_shift.input(tmux_out);
		tmux_shift.output(tmux_shifted);

		add1.input1(ahigh0);
		add1.input2(tmux_shifted);
		add1.output(ahigh1);

		t_shift.input(t);
		t_shift.output(t_shifted);

		add2.input1(t_shifted);
		add2.input2(alow);
		add2.output(t_plus_alow);

		comp1.input1(t_plus_alow);
		comp1.input2(t_shifted);
		comp1.output(amux_sel);
		
		amux.sel(amux_sel);
		amux.out(amux_out);

		add3.input1(ahigh1);
		add3.input2(amux_out);
		add3.output(ahigh2);

		//need to implement add4
		//need to get the high half of t for it

		reg_load_enable = false;
		//need to figure out clocks on adders and such

		SC_CTHREAD (fsm, hw_clock.pos());	
		SC_THREAD(process_hw_mult);
    	sensitive<<state;
 	}
  
};

#endif /* end _DH_HW_MULT_H_ */
