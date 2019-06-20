//Not sure what else needs to be included in this file
#pragma once


template <class T> class fifo_in_if: virtual public sc_interface
{
	public:
	virtual bool read (T&) = 0;
};


template <class T> class fifo_out_if: virtual public sc_interface
{
	public:
	virtual bool write (T) = 0;
};
