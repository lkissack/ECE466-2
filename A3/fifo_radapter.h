#include "systemc.h"

template <class T> class FIFO_READ_HS: public sc_module, public sc_fifo_in_if <T> {
	public:    
		sc_in_clk clock;    
		sc_in <T> data;    
		sc_in <bool> valid;    
		sc_out <bool> ready;   
		void read (T& x) {
			// blocking read        
			ready.write (true); 
			// signal that ready to consume data        
			// Wait until data is valid 
			do {  wait (clock->posedge_event());  } while (valid.read() != true); 
			x = data.read();    
			ready.write (false); 
			// read data, stop consumption for now    
		}    
		
		SC_CTOR (FIFO_READ_HS) {  ready.initialize (false);  }
		// Here, provide dummy implementations for unneeded sc_fifo_in_if <T> methods
		
		T read(){
		}
		
		void data_written_event(){
		}
		
		void nb_read(T& x){
		}
		
		int num_available(){
		}
		
};
