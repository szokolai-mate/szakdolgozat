#pragma once

#include <mutex>
#include <atomic>
#include <vector>

#include <iBuffer.h>

//TODO: really separate
//TODO: megpróbálni locklessé tenni

template <typename T>
class QueueBuffer : public DataFlow::iBuffer<T>{
private:
	unsigned int cap;
	T * buffer;
	std::atomic<unsigned int> start, end;
	std::atomic<bool> emptyflag;
	std::mutex m;

	unsigned int currentSize() {
		if(emptyflag){return 0;}
		if (start < end) {
			return end - start;
		}
		if (!emptyflag && start == end) { return cap; }
		return cap-start+end;
	}
public:
	/*Buffer size is immutable*/
	QueueBuffer(unsigned int cap);

	~QueueBuffer();

	/*Adds argument to buffer. Returns remaining capacity, -1 if was full*/
	int add(T  &_buffer);
	int add(std::vector<T>  &_buffer);
	/**/
	std::vector<T> get(const unsigned int & amount);
	/*Returns whether buffer is empty*/
	bool isEmpty() {
		return emptyflag;
	}

	bool isFull(){
		std::lock_guard<std::mutex> lock(m);
		if (!emptyflag && start == end) { return true; }
		else {return false;}
		
	}

	/*Clears buffer of data. O(n) - cost is infinitesimal*/
	void clear() {
		std::lock_guard<std::mutex> lock(m);
		end = start = 0;
		emptyflag = true;
	}

	/*Returns capacity of element T*/
	unsigned int capacity() {
		return cap;
	}

	/*should be protected*/
	T * exposeBuffer() {
		return &(buffer[start]);
	}

	unsigned int size() {
		std::lock_guard<std::mutex> lock(m);
		return currentSize();
	}

};