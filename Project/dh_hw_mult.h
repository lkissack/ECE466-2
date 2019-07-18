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

enum ctrl_state {WAIT, EXECUTE, LOAD_IN, SELECT,A0T0, A0T1, A1T0, A1T1, LOAD_OUT, OUTPUT, FINISH};
//enum ctrl_mux_state {A0T0, A0T1, A1T0, A1T1};

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
	//sc_signal<ctrl_mux_state> mux_state, next_mux_state;

	//lk_datapath datapath;
	//Content required for data path
	lk_register b_reg, c_reg, alow_reg, ahigh_reg;
	lk_splitter b_split, c_split;
	//blow*chigh corresponds to mult 0b01
	lk_multiplier mult0, mult1, mult2, mult3;

	lk_adder add0, add1, add2, add3, add4;
	lk_lessthan comp0, comp1;
	//multiplexors probably need a clock	
	lk_mux tmux, amux;
	lk_shift tmux_shift, t_shift_up, t_shift_down;

	sc_signal <NN_DIGIT> b_sig, c_sig;
	sc_signal <NN_HALF_DIGIT> blow, bhigh, clow, chigh;
	sc_signal <NN_DIGIT> t, alow, ahigh0, ahigh1, ahigh2, u;
	sc_signal <NN_DIGIT> ahigh_out;//The output of alow is alow
	sc_signal <NN_DIGIT> t_plus_u, t_plus_alow;
	sc_signal <NN_DIGIT> tmux_out, amux_out;
	sc_signal <NN_DIGIT> tmux_shifted, t_shifted_up;
	sc_signal <NN_DIGIT> t_shifted_down;

	//internal to hw_mult module - does not need to interact with demo
	sc_signal <bool> reset;
	sc_signal <sc_logic> reg_load_in_enable;
	sc_signal <sc_logic> reg_load_out_enable;
	//not sure if this should be bool or sc_logic?
	sc_signal <sc_logic> a_GT, a_LTE, t_GT, t_LTE;
	
	sc_signal <sc_logic> tmux_sel, amux_sel;
	sc_signal <bool> left, right;

  	void fsm_out();

	//function that simulates datapath - might need to be a thread for wait()s

	void fsm();
	
	void fsm_transition();

	//temporary function that performs original software multiplication
	void temp_mult();
  
  	SC_CTOR (dh_hw_mult): 	b_reg("b_reg"), c_reg("c_reg"), alow_reg("alow_reg"), ahigh_reg("ahigh_reg"),
  				b_split("b_split"), c_split("c_split"),
				mult0("mult0"), mult1("mult1"), mult2("mult2"), mult3("mult3"),
				add0("add0"), add1("add1"),add2("add2"),add3("add3"),add4("add4"),
				comp0("comp0"), comp1("comp1"),
				tmux("tmux"), amux("amux"),
				tmux_shift("tmux_shift"), t_shift_up("t_shift_up"), t_shift_down("t_shift_down")
  	{ 
		b_reg.input(in_data_1);
		b_reg.output(b_sig);
		b_reg.clock(hw_clock);
		b_reg.reset(reset);
		b_reg.load_enable(reg_load_in_enable);

		c_reg.input(in_data_2);
		c_reg.output(c_sig);
		c_reg.clock(hw_clock);
		c_reg.reset(reset);
		c_reg.load_enable(reg_load_in_enable);

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
		comp0.GT(t_GT);
		comp0.LTE(t_LTE);

		tmux.sel(tmux_sel);
		tmux.out(tmux_out);
		tmux.clock(hw_clock);
		
		tmux_shift.input(tmux_out);
		//Not sure if this is a reasonable hardware module
		tmux_shift.direction(left);
		tmux_shift.output(tmux_shifted);

		add1.input1(ahigh0);
		add1.input2(tmux_shifted);
		add1.output(ahigh1);

		t_shift_up.input(t_plus_u);
		t_shift_up.direction(left);
		t_shift_up.output(t_shifted_up);

		add2.input1(t_shifted_up);
		add2.input2(alow);
		add2.output(t_plus_alow);

		comp1.input1(t_plus_alow);
		comp1.input2(t_shifted_up);
		comp1.GT(a_GT);
		comp1.LTE(a_LTE);
		
		amux.sel(amux_sel);
		amux.out(amux_out);
		amux.clock(hw_clock);

		add3.input1(ahigh1);
		add3.input2(amux_out);
		add3.output(ahigh2);
		
		t_shift_down.input(t_plus_u);
		t_shift_down.direction(right);
		t_shift_down.output(t_shifted_down);
		
		add4.input1(ahigh2);
		add4.input2(t_shifted_down);
		add4.output(ahigh_out);
		
		ahigh_reg.input(ahigh_out);
		ahigh_reg.output(out_data_high);
		ahigh_reg.reset(reset);
		ahigh_reg.clock(hw_clock);
		ahigh_reg.load_enable(reg_load_out_enable);
		
		alow_reg.input(t_plus_alow);
		alow_reg.output(out_data_low);
		alow_reg.reset(reset);
		alow_reg.clock(hw_clock);
		alow_reg.load_enable(reg_load_out_enable);

		reg_load_in_enable.write(SC_LOGIC_0);
		reg_load_out_enable.write(SC_LOGIC_0);
		left = true;
		right = false;
		reset.write(false);
		//need to figure out clocks on adders and such

		SC_CTHREAD (fsm, hw_clock.pos());	
		//SC_CTHREAD(fsm_transition, hw_clock.pos());
		SC_METHOD(fsm_transition);
		//Sensitive to state, enable, done (should this be here?),
		sensitive << state << hw_mult_enable<< hw_mult_done<<a_GT << a_LTE<< t_GT<< t_LTE;
		SC_METHOD(fsm_out);
    		sensitive << state;
 	}
  
};

#endif /* end _DH_HW_MULT_H_ */
