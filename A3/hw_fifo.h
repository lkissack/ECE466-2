#pragma once
#include "systemc.h"
#include <iomanip>

template <class T> class hw_fifo : public sc_module{

	public:
		sc_in<bool> clk;
    	sc_in<T> data_in;
		sc_in<bool> valid_in;
		sc_out<bool> ready_out;
		sc_out<T> data_out;
		sc_out<bool> valid_out;
		sc_in<bool> ready_in;
		
		SC_HAS_PROCESS(hw_fifo);
		
		hw_fifo(sc_module_name nm, unsigned size) : sc_module(nm), _size(size){
		
			assert(size >0);
			_first = _items = 0;
			_data = new T[_size];
			SC_CTHREAD(main, clk.pos());
			ready_out.initialize(true);
			valid_out.initialize(false);
		}
		
		~hw_fifo(){delete[] _data;}
		
	protected:
		void main()
		{
			while(1) {
				//cout<<"hw fifo process"<<endl;
				  if (valid_in.read() && ready_out.read())
				  {
				    // store new data item into fifo
				    _data[(_first + _items) % _size] = data_in.read();
				    _items++;
				  }
				  if (ready_in.read() && valid_out.read())
				  {
				    // discard data item that was just read from fifo
				    --_items;
				    _first = (_first + 1) % _size;
				  }
				  // update all output signals
				  ready_out.write(_items < _size);
				  valid_out.write(_items > 0);
				  data_out.write(_data[_first]);
			  	wait();
			}
		}

    unsigned _size, _first, _items;
    T* _data;
};
