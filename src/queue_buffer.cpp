#pragma once
#include <mutex>
#include <atomic>

#include <iBuffer.h>

//TODO: megpróbálni locklessé tenni

template <typename T>
 class queue_buffer : public iBuffer<T>{
private:
	unsigned int cap;
	T * buffer;
	std::atomic<unsigned int> start, end;
	std::atomic<bool> emptyflag;
	std::mutex m;
public:
	/*Buffer size is immutable*/
	queue_buffer(unsigned int cap);

	~queue_buffer();

	/*Adds argument to buffer. Returns remaining capacity, -1 if was full*/
	int add(T  &_buffer);
	/*Copies next T into argument. Returns true on success, false on failure (buffer was empty)*/
	int get(T &into);
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
		if (start < end) {
			return end - start;
		}
		if (!emptyflag && start == end) { return cap; }
		return cap-start+end;
	}

};


template <typename T>
queue_buffer<T>::queue_buffer(unsigned int _capacity) {
	cap = _capacity;
	buffer = new T[cap];

	start = 0;
	end = 0;

	emptyflag = true;
}

template <typename T>
int queue_buffer<T>::add(T  &_buffer) {
	std::lock_guard<std::mutex> lock(m);
	if (start == end) {
		if (isEmpty()) {
			//empty - add and set to not empty
			emptyflag = false;
		}
		else {
			//full
			return -1;
		}
	}
	buffer[end] = _buffer;
	end = (end + 1) % cap;
	//returns capacity left
	if (end == start && !isEmpty()) { return 0; }
	if (start <= end) {
		return cap + start - end;
	}
	else { return start - end; }
}

template <typename T>
int queue_buffer<T>::get(T &into) {
	std::lock_guard<std::mutex> lock(m);
	if (isEmpty()) { return 0; }

	into = buffer[start];
	start = (start + 1) % cap;
	if (start == end) { emptyflag = true; }
	if (start < end) {
			return end - start;
		}
	if (!emptyflag && start == end) { return cap; }
	return cap-start+end;
}

template <typename T>
queue_buffer<T>::~queue_buffer() {
	delete buffer;
}
