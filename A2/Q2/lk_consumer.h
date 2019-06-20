#pragma once
#include "systemc.h"
#include "fifo_if.h"
#include <iomanip>

template <class T> class consumer: public sc_module {

	public:
	sc_port<fifo_in_if <T> > in;
	sc_in <bool> clock;

	void do_reads(){
		T data;
		while(true){
			if(in->read(data)){		
				cout <<"R: "<<data <<" at "<<sc_time_stamp()<<endl;
				data += data % 10;
			}
			//only try to read once per clock cycle
			wait();
		}
	}

	SC_CTOR (consumer){
		SC_THREAD(do_reads);
		sensitive <<clock.pos();
	}

};
