#pragma once

#include "iBuffer.h"

template <typename T>
class iAdvancedBuffer : iBuffer<T> {
public:
	/*Adds argument to buffer. Returns remaining capacity, -1 if was already full*/
	virtual int addamount(T * _buffer,const unsigned int & amount) = 0;

	virtual int addsome(T * _buffer,const unsigned int & max_amount, unsigned int &added_amount) = 0;
    
	/*Copies next T into argument. Returns true on success, false on failure (buffer was empty)*/
	/*TODO: NEM LESZ JÓ! ha csak pointert adok vissza és sokáig tart a feldolgozás lehet hogy az adatot már felülírta.*/
	/*DE ha blokkolóban írom meg? -> át kell adni a buffer pointerét argumentumként -> blokkolva bemásolja*/
	virtual T* getamount(const unsigned int & amount) = 0;

	virtual T* getsome(unsigned int &got_amount,const unsigned int &max_amount) = 0;

	virtual T* getsome(unsigned int &got_amount) = 0;
};