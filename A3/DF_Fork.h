#include "systemc.h"

#include "fifo_radapter.h"
#include "iomanip"

template <class T> SC_MODULE (DF_Fork) {    
	sc_fifo_in <T> input;    
	sc_fifo_out <T> output1, output2;    
	
	void process() {
		cout<<"fork process "<<endl;        
		while(1) {            
			T value = input.read();            
			output1.write(value);            
			output2.write(value);
			cout<<"fork process - input: "<<value<<endl;        
		}    
	}
	    
	SC_CTOR (DF_Fork) { 
		SC_THREAD(process); 
	}
};
