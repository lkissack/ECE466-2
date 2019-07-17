#include "systemc.h"

#include "DF_Const.h"
#include "DF_Fork.h"
#include "DF_Printer.h"
#include "DF_Adder.h"
#include "Adder.h"
#include "hw_fifo.h"
#include "fifo_radapter.h"
#include <iomanip>

int sc_main (int argc, char* argv[]) {     
	DF_Const <int> constant ("constant", 1);    // constant = 1
	//DF_Adder <int> adder ("adder");  
	Adder <int> m_adder("m_adder");  
	DF_Fork	<int> fork("fork");    
	DF_Printer <int> printer ("printer", 10);    // 10 interations
	
	sc_fifo	<int> const_out ("const_out", 1);  // FIFO of size 1   
	//sc_fifo <int> adder_out ("adder_out", 1);  // FIFO of size 1
	FIFO_READ_HS <int> read_adapter("read_adapter");

	hw_fifo <int> hw_adder_out("hw_adder_out", 1);	
	sc_fifo	<int> printer_in ("printer_in", 1);    // FIFO of size 1
	sc_fifo	<int> feedback ("feedback", 1);    // FIFO of size 1

	sc_clock clock("clock", 10, SC_NS);
	
	//need signals to connect everything
	sc_signal <int> adder_data("adder_data");
	sc_signal <int> fork_data("fork_data");

	sc_signal <bool> adder_ready("adder_ready");
	sc_signal <bool> adder_valid("adder_valid");
	sc_signal <bool> fork_ready, fork_valid;
	
	feedback.write(40); //This is not working and is behaving as if this is not written to
	// do not forget about initialization!  	
	  
	constant.output(const_out);
	printer.input(printer_in);
	m_adder.input1(const_out);
	m_adder.input2(feedback);
	m_adder.clock(clock);
	m_adder.ready(adder_ready);
	m_adder.valid(adder_valid);
	
	//Need to modify to use correct adapters

	read_adapter.clock(clock);
	read_adapter.valid(fork_valid);
	read_adapter.ready(fork_ready);
	read_adapter.data(fork_data);
	
	//Original content
	//adder.output(adder_out);    
	//fork.input(adder_out);

	m_adder.output(adder_data);

	hw_adder_out.clk(clock);
	hw_adder_out.data_in(adder_data);
	hw_adder_out.data_out(fork_data); 
	hw_adder_out.valid_in(adder_valid);
	hw_adder_out.valid_out(fork_valid);
	hw_adder_out.ready_in(fork_ready);
	hw_adder_out.ready_out(adder_ready);

	fork.input(read_adapter);
	
	fork.output1(feedback);
	fork.output2(printer_in);  
	//feedback.write(40);  
	sc_start(); 
	//sc_start (5000, SC_NS);    
	
	return 0; 
}
