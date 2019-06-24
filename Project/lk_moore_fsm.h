#pragma once

#include "systemc.h"

SC_MODULE(lk_moore_fsm){
	sc_out <sc_logic> tmux_sel, amux_sel;

	//signals from tmux
	sc_in <sc_logic> t_EQ, t_GT, t_LT;

	//signals from amux
	sc_in <sc_logic> a_EQ, a_GT, a_LT;

};
