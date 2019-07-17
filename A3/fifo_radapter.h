#pragma once
#include "systemc.h"
#include <iomanip>

template <class T> class FIFO_READ_HS: public sc_module, public sc_fifo_in_if <T> {
	public:    
		sc_in_clk clock;    
		sc_in <T> data;    
		sc_in <bool> valid;    
		sc_out <bool> ready;   
		
		virtual void read (T& x) {
			cout<<"read hw fifo"<<endl;
			// blocking read        
			ready.write (true); 
			// signal that ready to consume data        
			// Wait until data is valid 
			do {  wait (clock->posedge_event());  } while (valid.read() != true); 
			x = data.read();    
			ready.write (false); 
			// read data, stop consumption for now    
		}    
		
		virtual T read() {
        T tmp;
        read(tmp);
        return tmp;
   		}
		
		// sc_fifo_out_if<T> methods:
		bool nb_read(T&)
		{ assert(0); return false; }

		int num_available() const
		{ assert(0); return 0; }

		const sc_event& data_written_event() const
		{ static sc_event dummy; assert(0); return dummy; }

		// constructor
		SC_CTOR(FIFO_READ_HS) {
		    ready.initialize(false);
		}
		
};
