//parameterized fifo hierarchical channel
//size and numerical data type specified when channel object is created
//Non-blocking read and write

#include "systemc.h"
#include "fifo_if.h"

template <class T> class lk_fifo : public sc_module, public fifo_out_if <T>, public fifo_in_if <T>
{

private:
	//fifo type and size is specified when created
	//shared data - protected by arbitrator
	T fifo_data[size];
	int first,last;

	sc_event read_req, write_req, done;
	bool read_flag, write_flag, read_status, write_status;
	//Not really sure if this is valid
	T* read_content;
	T write_content;

public:

//constructor?
	SC_HAS_PROCESS(lk_fifo);
	//not sure how to set up contructor with T and size?
	lk_fifo(sc_module_name nm): sc_module(nm), size(20){
		//set size? size = 
		read_flag = false;
		write_flag = false;
		read_status = false;
		write_status = false;
		SC_THREAD(arbitrator);
		sensitive << read_req <<write_req;
	} 

//read must go first
	void arbitrator(){
		while(true){
			if(read_flag){
				read_flag = false;
				//if fifo is not empty, read and set read_status = true;
				//else read_status = false;			

			}
			if(write_flag){
				write_flag = false;
				//if fifo is not full, write & set write_status = true;
				//else write_status = false;
			}
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
		read_content = data;
		read_req.notify();
		wait(done);
		return read_status;		
	}




};
