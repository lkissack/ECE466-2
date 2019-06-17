
//#include "stdafx.h"
#include "systemc.h"
#include "lk_mon.h"
#include "lk_stim.h"

//Files relevant to modules
#include "lk_mem_ctrl.h"

int sc_main(int argc, char** argv)
{
	sc_signal <bool> reset, new_comm, complete;
    	sc_signal < sc_uint<2> > comm;
    	sc_signal < sc_uint<8> > addr;
    	sc_signal_rv <8> data;	
	sc_clock TestClock("TestClock", 10, SC_NS, 0.5, 1, SC_NS);

	lk_mem_ctrl MC("Memory Device");	

	//Connect module signals with external signals
	MC.data(data);
	MC.addr(addr);
	MC.comm(comm);
	MC.new_comm(new_comm);
	MC.complete(complete);
	MC.reset(reset);
	MC.clk(TestClock);	

	//Stimulus
	stim Stimulus1("stimulus");
	Stimulus1.data(data);
	Stimulus1.addr(addr);
	Stimulus1.comm(comm);
	Stimulus1.complete(complete);
	Stimulus1.new_comm(new_comm);
	Stimulus1.reset(reset);
	Stimulus1.clock(TestClock);

	//Monitor
	//mon Monitor1("monitor");

	//Waveform tracing
	//sc_trace_file* Tf = sc_create_vcd_trace_file ("digital_filter_traces");
	//Tf->set_time_unit(1, SC_NS);
	//sc_trace(Tf, Xsig, "X");
	//sc_trace(Tf, Ysig, "Y");
	//sc_trace(Tf, Resetsig, "Reset");
	//sc_trace(Tf, TestClock, "Clock");	

	//Each clk cycle is 10 ns - 20 samples are desired
	sc_start(400, SC_NS);

	//Close VCD trace file
	//sc_close_vcd_trace_file(Tf);
	return 0;
}

