#pragma once
#include "systemc.h"
#include "lk_mem_ctrl.h"
#include <iomanip>

#define RDBLK 0
#define RDBYT 1
#define WTBYT 2
#define WTBLK 3


SC_MODULE(stim) {
	sc_out <bool> reset, new_comm;//Module writing to memory asserts new_comm
	sc_in <bool> complete; //Memory asserts complete
    	sc_out < sc_uint<2> > comm;
    	sc_out < sc_uint<8> > addr;
    	sc_inout <sc_lv <8> > data;
	sc_in_clk clock;

	void stimgen() {
		cout<<"Verify Memory is empty before writing to it."<<endl;
		cout<<"Reading Byte"<<endl;
		comm.write(RDBYT);
		addr.write(40);
		
		//assert new-command line
		new_comm.write(true);

		//wait for memory to respond to assertion
		while(!complete){
			wait();
		}
		//Memory is done, deassert
		new_comm.write(false);
		
		//Wait for memory to respond to deassert of new_comm
		while(complete){
			wait();
		}
		cout<<"Check lines are Z:"<<data.read() <<endl;

		cout<<"Reading Block"<<endl;
		comm.write(RDBLK);
		addr.write(100);
		
		//assert new-command line
		new_comm.write(true);

		//wait for memory to respond to assertion
		while(!complete){
			wait();
		}
		//Memory is done, deassert
		new_comm.write(false);
		
		//Wait for memory to respond to deassert of new_comm
		while(complete){
			wait();
		}
		cout<<"Check lines are Z:"<<data.read() <<endl;


		cout<<"Test Writing Byte"<<std::endl;		
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

		cout<<"Test Writing Block"<<endl;
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

		cout<<"Test Reading Byte"<<endl;
		comm.write(RDBYT);
		addr.write(40);
		
		//assert new-command line
		new_comm.write(true);

		//wait for memory to respond to assertion
		while(!complete){
			wait();
		}
		//Memory is done, deassert
		new_comm.write(false);
		
		//Wait for memory to respond to deassert of new_comm
		while(complete){
			wait();
		}

		cout<<"Check lines are Z:"<<data.read() <<endl;
		
		cout<<"Test Reading Block"<<endl;
		comm.write(RDBLK);
		addr.write(100);
		
		//assert new-command line
		new_comm.write(true);

		//wait for memory to respond to assertion
		while(!complete){
			wait();
		}
		//Memory is done, deassert
		new_comm.write(false);
		
		//Wait for memory to respond to deassert of new_comm
		while(complete){
			wait();
		}
		cout<<"Check lines are Z:"<<data.read() <<endl;
		
	}

	SC_CTOR(stim) {
		SC_CTHREAD(stimgen, clock.pos());
	}


};
