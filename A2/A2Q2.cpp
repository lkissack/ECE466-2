//#include "stdafx.h"
#include "systemc.h"
#include "lk_mem_ctrl.h"


int sc_main(int argc, char* argv[])
{
    // Memory module instance:
    lk_mem_ctrl MC("MC");
    
    // Your testbench module(s) instances:
	//Stimulus?
	stim Stimulus1("stimulus");
	//Monitors?

    // Internal signals:
    sc_signal <bool> reset, new_comm, complete;
    sc_signal < sc_uint<2> > comm;
    sc_signal < sc_uint<8> > addr;
    sc_signal_rv <8> data;
    // _rv needed because of multiple drivers
  

    // Interconnects:
    MC.clk(clk);
    MC.reset(reset);
    MC.data(data);
    MC.addr(addr);
    MC.comm(comm);
    MC.new_comm(new_comm);
    MC.complete(complete);

	//Interconnect stimulus with sc_signals?


    // Rest of sc_main ...

    return 0;
}

	

/*sc_signal <float> Xsig, Ysig;
	sc_signal <bool> Resetsig;
	sc_clock TestClock("TestClock", 10, SC_NS, 0.5, 1, SC_NS);

	//This will depend which structure is being tested
	//behave_filter DUT("behave_filter");
	structural_filter DUT("structural_filter");

	//Connect module signals with external signals
	DUT.X(Xsig);
	DUT.Y(Ysig);
	DUT.reset(Resetsig);
	DUT.clock(TestClock);	

	//Stimulus
	stim Stimulus1("stimulus");
	Stimulus1.X(Xsig);
	Stimulus1.reset(Resetsig);
	Stimulus1.clock(TestClock);

	//Monitor
	mon Monitor1("monitor");
	Monitor1.X(Xsig);
	Monitor1.Y(Ysig);
	Monitor1.Reset(Resetsig);
	Monitor1.clock(TestClock);

	//Waveform tracing
	sc_trace_file* Tf = sc_create_vcd_trace_file ("digital_filter_traces");
	Tf->set_time_unit(1, SC_NS);
	sc_trace(Tf, Xsig, "X");
	sc_trace(Tf, Ysig, "Y");
	sc_trace(Tf, Resetsig, "Reset");
	sc_trace(Tf, TestClock, "Clock");	

	//Each clk cycle is 10 ns - 20 samples are desired
	sc_start(200, SC_NS);

	//Close VCD trace file
	sc_close_vcd_trace_file(Tf);
	return 0;
}*/

