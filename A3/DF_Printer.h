#include "systemc.h"
#include <iomanip>

template <class T> SC_MODULE (DF_Printer) {     
	sc_fifo_in <T> input;    	
	     
	SC_HAS_PROCESS (DF_Printer); 

	void process() {         
		cout<<"in printer process"<<endl;
		for (unsigned i = 0; i < n_iterations_; i++) {             
			T value = input.read();
			cout << name() << " " << value << endl;         
		}         
		sc_stop(); 
	// terminate after a given number of iterations     
	}
	
	DF_Printer(sc_module_name NAME, unsigned N_ITERATIONS) :  sc_module(NAME), n_iterations_(N_ITERATIONS) { 
		SC_THREAD(process); 
	}    
	 
	unsigned n_iterations_; 
};
