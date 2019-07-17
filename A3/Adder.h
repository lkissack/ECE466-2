#include "systemc.h"
#include <iomanip>

template <class T> SC_MODULE (Adder) {     
	sc_fifo_in <T> input1, input2;    
	sc_out <T> output;  
	sc_in_clk clock;
	sc_out <bool> valid;
	sc_in <bool>ready;
	  
	void process() { 
		while (1) {
			
			output.write(input1.read() + input2.read()); 
			cout<<"adder process"<<endl;
			cout<<"in1: "<<input1.read()<<endl;
			cout<<" in2: "<<input2.read()<<endl;
		}
	}   

	void write(const int& x){
		output.write(x);
		valid.write(true);
		
		do{ wait(clock->posedge_event());} while(ready.read()!=true);

		valid.write(false);

	}  
	
	SC_CTOR (Adder) { 
		SC_THREAD(process); 
	} 
}; 
