#include "systemc.h"
#include "fifo_if.h"

class consumer: public sc_module {

	public:
	sc_port<fifo_in_if <T> > in;
	sc_in <bool> clock;

	void do_reads(){

	}

	SC_CTOR (consumer){
		SC_THREAD(do_reads);
		sensitive <<clock.pos();
	}

};
