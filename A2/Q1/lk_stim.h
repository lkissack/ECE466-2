#pragma once
#include "systemc.h"
#include <iomanip>

#define RDBLK 0
#define RDBYT 1
#define WTBYT 2
#define WTBLK 3

SC_MODULE(stim) {
	sc_signal <bool> reset, new_comm, complete;
    	sc_signal < sc_uint<2> > comm;
    	sc_signal < sc_uint<8> > addr;
    	sc_signal_rv <8> data;

	void stimgen() {
		//Test writing byte			
		comm.write(WTBYT);
		//put address on bus
		addr.write(40);
		//put data on bus
		data.write(0xFF);

		//assert new-command line
		new_comm.write(true);

		//is complete deasserted initially?	
		while(!complete){
			wait();
		}
		//Memory is done, deassert
		new_comm.write(false);
		
		//Wait for memory to respond to deassert of new_comm
		while(complete){
			wait();
		}		

		//Test writing block
		comm.write(WTBLK);
		//put address on bus
		addr.write(100);
		//put data on bus
		data.write(0xAA);

		//assert new-command line
		new_comm.write(true);

		//Wait until memory responds to assertion - is complete deasserted initially?	
		while(!complete){
			wait();
		}

		
		//Memory is done, deassert
		new_comm.write(false);
		
		//Wait for memory to respond to deassert of new_comm
		while(complete){
			wait();
		}


		//Test reading byte

		//Test reading block
		
		
	}

	SC_CTOR(stim) {
		SC_CTHREAD(stimgen, clock.pos());
	}


};
