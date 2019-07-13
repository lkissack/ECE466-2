#include "systemc.h"

#include "DF_Const.h"
#include "DF_Fork.h"
#include "DF_Printer.h"
#include "DF_Adder.h"
#include "Adder.h"
#include "hw_fifo.h"
#include "fifo_radapter.h"

int sc_main (int argc, char* argv[]) {     
	DF_Const <int> constant ("constant", 1);    // constant = 1
	//DF_Adder <int> adder ("adder");  
	Adder <int> m_adder("adder");  
	DF_Fork	<int> fork("fork");    
	DF_Printer <int> printer ("printer", 10);    // 10 interations
	
	sc_fifo	<int> const_out ("const_out", 1);  // FIFO of size 1   
	//sc_fifo	<int> adder_out ("adder_out", 1);  // FIFO of size 1
	FIFO_READ_HS <int> read_adapter("read_adapter");
	hw_fifo <int> hw_adder_out("fifo_item", 1);
	
	sc_fifo	<int> printer_in ("printer_in", 1);    // FIFO of size 1
	sc_fifo	<int> feedback ("feedback", 1);    // FIFO of size 1

	sc_clock clock("clock", 10, SC_NS);
	
	//need signals to connect everything
	sc_signal <int> adder_data("adder_data");
	sc_signal <int> fork_data("fork_data");

	sc_signal <bool> source_ready, source_valid, sink_ready, sink_valid;
	
	feedback.write(40); 
	// do not forget about initialization!  	
	  
	constant.output (const_out);
	printer.input (printer_in);
	m_adder.input1 (const_out);
	m_adder.input2 (feedback);
	
	//Need to modify to use correct adapters

	read_adapter.clock(clock);
	read_adapter.valid(sink_valid);
	read_adapter.ready(sink_ready);
	read_adapter.data(fork_data);
	
	//Original content
	//adder.output (adder_out);    
	//fork.input (adder_out);

	m_adder.output (adder_data);

	hw_adder_out.clk(clock);
	hw_adder_out.data_in(adder_data);
	hw_adder_out.data_out(fork_data); 
	hw_adder_out.valid_in(source_valid);
	hw_adder_out.valid_out(sink_valid);
	//seems backwards
	hw_adder_out.ready_in(sink_ready);
	hw_adder_out.ready_out(source_ready);

	fork.input(fork_data);
	
	fork.output1 (feedback);
	fork.output2 (printer_in);    
	// Start simulation without time limit
	// The printer module is responsible for stopping the simulation
	//originally sc_start(-1) but this is not valid
	sc_start (5000, SC_NS);     
	
	return 0; 
}
