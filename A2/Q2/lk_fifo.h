//parameterized fifo hierarchical channel
//size and numerical data type specified when channel object is created
//Non-blocking read and write
#pragma once
#include "systemc.h"
#include "fifo_if.h"
#include <iomanip>

template <class T> class lk_fifo : public sc_module, public fifo_out_if <T>, public fifo_in_if <T>
{

private:
	//fifo type and size is specified when created
	//shared data - protected by arbitrator
	T* fifo_data;

	//sc_ports not necessary since it inherits from fifo_if interfaces

	//current size not actually used
	int free, read_index, write_index, size;

	sc_event read_req, write_req, done;
	bool read_flag, write_flag, read_status, write_status;
	
	T* read_content;
	T write_content;

public:

	SC_HAS_PROCESS(lk_fifo);
	lk_fifo(sc_module_name nm, int size_): sc_module(nm){
				
		size = size_;
		fifo_data = new T[size_];
		read_flag = false;
		write_flag = false;
		read_status = false;
		write_status = false;
		SC_THREAD(arbitrator);
		sensitive << read_req <<write_req;
		reset();
	} 
	//Same behaviour as described in SystemC slide 15
	void reset(){
	free = size;
	read_index = 0;
	write_index = 0;
	}

	//read must go first
	void arbitrator(){		
		while(true){
			if(read_flag){
				read_flag = false;
				if(free < size){
					*read_content = fifo_data[read_index];
					//the read index will increase then wrap around
					read_index = (read_index + 1) % size;
					free++;
					//cout<<"Free elements remaining: "<<free<<endl;
					read_status = true;
				}
				else{
					read_status = false;
				}
				//cout<<" Status: "<<read_status<<endl;			

			}
			if(write_flag){
				write_flag = false;
				if(free > 0){
					fifo_data[write_index] = write_content;
					write_index = (write_index + 1)%size;
					free--;
					//cout<<"Free elements remaining: "<<free<<endl;
					write_status = true;
				}
				else{
					write_status = false;
				}	
			}
		//cout<<"Arbitrator - Free elements remaining: "<<free<<endl;
		done.notify();		
		wait();
	
		}
	}

	bool write(T data){
		write_flag = true;
		write_content = data;
		write_req.notify();
		wait(done);
		return write_status;
	}

	bool read (T& data){
		read_flag = true;
		read_content = &data;
		read_req.notify();
		wait(done);
		return read_status;		
	}
};
