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
			T data = 0;
			//Gets triggered an extra time at initialization
			wait();
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
