#pragma once

template <typename T>
class iBuffer{
    /*Adds argument to buffer. Returns remaining capacity, -1 if was full*/
    virtual unsigned int add(const T  &_buffer) = 0;
    
    /*Copies next T into argument. Returns true on success, false on failure (buffer was empty)*/
	virtual bool get(T &into) = 0;

    /*Returns whether buffer is empty*/
	virtual bool isEmpty() = 0;

    /*Returns whether buffer is full*/
    virtual bool isFull() = 0;

	/*Clears buffer of data*/
	virtual void clear() = 0;
	
	/*Returns capacity in number of element T*/
	virtual unsigned int capacity() = 0;
	
	virtual unsigned int size() = 0;
};