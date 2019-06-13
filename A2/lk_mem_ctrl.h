#include <iostream>

//#define NOP 0
//Replacing NOP with RDBLK
#define RDBLK 0
#define RDBYT 1
#define WTBYT 2
#define WTBLK 3

static sc_logic Z[8] = {SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, 
			SC_LOGIC_Z, SC_LOGIC_Z, SC_LOGIC_Z, 
			SC_LOGIC_Z, SC_LOGIC_Z};

	SC_MODULE(lk_mem_ctrl){

	sc_inout<sc_lv<8> > data;
	sc_in <sc_uint<8> > addr;
	sc_in <sc_uint<2> > comm;
	sc_in <bool> new_comm;
	sc_out <bool> complete;

	sc_in_clk clk;
	sc_in <bool> reset;

	sc_lv <8> memory[256];

	void do_switch(sc_uint<2> comm_s){
		switch(comm_s){
			case RDBYT:
				addr_s = addr.read();//Read desired address from bus
				data_s = memory[addr_s];
				std::cout << "@" << sc_time_stamp()<<": RDBYT, address = ";
				std::cout <<addr_s <<", Data = "<<data_s <<std::endl;
				wait();
				data.write(data_s);//put data on output port
				break;
			case RDBLK:
				
				addr_s = addr.read();
				for (int i =0; i <4; i++){
					data_s = memory[addr_s];
					std::cout << "@" << sc_time_stamp()<<": RDBLK-";
					std::cout<< i <<" , address = ";
					std::cout <<addr_s <<", Data = "<<data_s <<std::endl;
					wait();
					data.write(data_s);//Alternatively, write to buffer
					addr_s++;				
				}
				
				break;			
			case WTBYT:
				addr_s = addr.read();//Read desired address from bus
				data_s = data.read();//Read data to write from bus
				std::cout << "@" << sc_time_stamp()<<": WTBYT, address = ";
				std::cout <<addr_s <<", Data = "<<data_s <<std::endl;
				wait();
				memory[addr_s] = data_s;//Put data into memory @ address
				break;
			case WTBLK: //Not originally structured with for loop - Still valid?
				
				addr_s = addr.read();//Read first desired address from bus
				for( int i = 0; i < 4; i++){
					data_s = data.read();//Read data to write from bus
					std::cout << "@" << sc_time_stamp()<<": WTBLK-";
					std::cout<< i <<" , address = ";
					std::cout <<addr_s <<", Data = "<<data_s <<std::endl;
					wait();//After 1 clock cycle, write into memory space
					memory[addr_s] = data_s;//Put data into memory @ address
					addr_s++;//Move to the next byte
				}				
				
				break;
			default:
				std::cout << "Illegal Command: "<< comm_s <<std::endl;
				break;
		}
	}

	void mem_process(){
		//Internal variables to hold samples
		sc_uint <2> comm_s;
		sc_uint <8> addr_s;
		sc_lv <8> data_s;

		while(true){
			if(reset.read()==true){
				complete.write(false);
			}
			else{
				if(new_comm.read()==true){
					comm_s = comm.read();
					do_switch(comm_s);
					complete.write(true);
					while(new_comm.read()==true){
						if(reset.read()==true){
							break;
						}
						wait();	
					}
					if(comm_s == RDBYT){
						data.write(Z);
					}
					complete.write(false);					
				}
					
			}
			wait();
		}
	}
	
	SC_CTOR(lk_mem_ctrl){

		SC_THREAD(mem_process, clk.pos());
	}

};
