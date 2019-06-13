//Not sure what is in this class yet

#include "systemc.h"
#include "fifo_if.h"
#include <iomanip>

template <class T> class producer: public sc_module
{
	public:
		sc_port <fifo_out_if<T> > out;
		//some other stuff

		sc_in_clk clock;

		void do_writes(){
			T data = 0;
			while(true){
				//some stuff

				if(out->write(data)){
					cout<<"W"<<data<< " at "<<sc_time_stamp() <<endl;
				}
				//do some other stuff
			}
		}

	CT_CTOR(producer){
		SC_THREAD(do_writes);
		sensitive <<clock.pos();
	}
	
};
