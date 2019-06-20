#include "systemc.h"
#include "fifo_if.h"
#include "lk_fifo.h"
#include "lk_consumer.h"
#include "lk_producer.h"

//#include <iomanip>


int sc_main(int argc, char* argv[])
{
	sc_clock fast_clock("fast_clock", 50, SC_NS);
	sc_clock slow_clock("slow_clock", 100, SC_NS);
	
	//Special constructor since it is a template
	lk_fifo <int> fifo1("fifo1", 8);
	//Don't need to connect any signals since it inherits from fifo_if

	//set up signals
	producer <int> prod1("prod1");
	prod1.out(fifo1);
	prod1.clock(fast_clock);

	consumer <int> cons1("cons1");
	cons1.in(fifo1);
	cons1.clock(slow_clock);

	sc_start(2200, SC_NS);

    return 0;
}


