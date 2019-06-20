#pragma once

#include "systemc.h"
#include "fifo_if.h"
#include <iomanip>

template <class T> class producer: public sc_module
{
	public:
		sc_port <fifo_out_if<T> > out;
		sc_in <bool> clock;

		void do_writes(){
			//Attempt to understand why write is being triggered twice @ 0s
			//Does same thing when connected to both clocks
			//Mirrored version of consumer code
			//cout<<"triggered at "<<sc_time_stamp()<<endl;
			//wait(1, SC_NS);
			T data = 0;
			while(true){
				//cout<<"triggered at "<<sc_time_stamp()<<endl;
				if(out->write(data)){
					cout<<"W: "<<data<< " at "<<sc_time_stamp() <<endl;
					data = (data + 1)%10;				
				}
				//only try to write once/cycle				
				wait();
			
			}
		}

	SC_CTOR(producer){
		SC_THREAD(do_writes);
		sensitive <<clock.pos();
	}
	
};
