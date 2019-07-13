#include "systemc.h"

#include "DF_Const.h"
#include "DF_Fork.h"
#include "DF_Printer.h"
#include "DF_Adder.h"
#include "hw_fifo.h"
#include 

int sc_main (int argc, char* argv[]) {     
	DF_Const <int> constant ("constant", 1);    // constant = 1
	DF_Adder <int> adder ("adder");    
	DF_Fork	<int> fork("fork");    
	DF_Printer <int> printer ("printer", 10);    // 10 interations
	
	sc_fifo	<int> const_out ("const_out", 1);  // FIFO of size 1   
	//sc_fifo	<int> adder_out ("adder_out", 1);  // FIFO of size 1
	FIFO_READ_HS <int> read_adapter("read_adapter");
	hw_fifo <int> hw_adder_out("hw_adder_out", 1);
	
	sc_fifo	<int> printer_in ("printer_in", 1);    // FIFO of size 1
	sc_fifo	<int> feedback ("feedback", 1);    // FIFO of size 1

	sc_clock clock("clock", 10, SC_NS);
	
	//need signals to connect everything
	sc_signal <int> fifo_in("fifo_in");
	sc_signal <int> fifo_out("fifo_out");

	sc_signal <bool> source_ready, source_valid, sink_ready, sink_valid;
	
	feedback.write(40); 
	// do not forget about initialization!  	
	  
	constant.output (const_out);
	printer.input (printer_in);
	adder.input1 (const_out);
	adder.input2 (feedback);
	
	//Need to modify to use correct adapters

	read_adapter.clock(clock);
	read_adapter.valid(sink_valid);
	read_adapter.ready(sink_ready);
	read_adapter.data(fifo_out);
	
	//Original content
	//adder.output (adder_out);    
	//fork.input (adder_out);

	adder.output (fifo_in);

	hw_fifo.clk(clock);
	hw_fifo.data_in(fifo_in);
	hw_fifo.data_out(fifo_out); 
	hw_fifo.
	
	fork.output1 (feedback);
	fork.output2 (printer_in);    
	// Start simulation without time limit
	// The printer module is responsible for stopping the simulation
	//originally sc_start(-1) but this is not valid
	sc_start (5000, SC_NS);     
	
	return 0; 
}
