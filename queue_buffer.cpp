#pragma once
#include <mutex>
#include <atomic>

template <typename T> class queue_buffer{
private:
	unsigned long long cap;
	T * buffer;
	std::atomic< unsigned long long > start, end;
	std::atomic<bool> emptyflag;
	std::mutex m;
public:
	/*Buffer size is immutable*/
	queue_buffer(unsigned long long cap);

	~queue_buffer();

	/*Adds argument to buffer. Returns remaining capacity, -1 if was full*/
	unsigned long long add(const T  &_buffer);

	unsigned long long addamount(T * _buffer,const unsigned long long & amount);

	unsigned long long addsome(T * _buffer,const unsigned long long & max_amount, unsigned long long &added_amount);

	/*Copies next T into argument. Returns true on success, false on failure (buffer was empty)*/
	bool get(T &into);

	T* getamount(const unsigned long long & amount);

	T* getsome(unsigned long long &got_amount,const unsigned long long &max_amount);

	T* getsome(unsigned long long &got_amount);

	/*Returns whether buffer is empty*/
	bool isEmpty() {
		//std::unique_lock<std::mutex> lk(m);
		//m.lock();
		//m.unlock();
		return emptyflag;
	}

	/*Clears buffer of data. O(n) - cost is infinitesimal*/
	void clear() {
		end = start = 0;
		emptyflag = true;
	}

	/*Returns capacity of element T*/
	unsigned long long capacity() {
		return cap;
	}

	/*should be protected*/
	T * exposeBuffer() {
		return &(buffer[start]);
	}

	unsigned long long size() {
		m.lock();
		if (start < end) {
			return end - start;
		}
		if (!emptyflag && start == end) { m.unlock(); return cap; }
		m.unlock();
		return cap-start+end;
	}

};


template <typename T>
queue_buffer<T>::queue_buffer(unsigned long long _capacity) {
	cap = _capacity;
	buffer = new T[cap];

	start = 0;
	end = 0;

	emptyflag = true;
}

template <typename T>
unsigned long long queue_buffer<T>::add(const T  &_buffer) {
	m.lock();
	//std::unique_lock<std::mutex> lk(m);
	if (start == end) {
		if (isEmpty()) {
			//empty - add and set to not empty
			emptyflag = false;
		}
		else {
			//full
			m.unlock();
			return -1;
		}
	}
	//std::cout << "ADDING" << std::endl;
	buffer[end] = _buffer;
	end = (end + 1) % cap;
	//returns capacity left
	if (end == start && !isEmpty()) { m.unlock(); return 0; }
	if (start <= end) {
		m.unlock(); return cap + start - end;
	}
	else { m.unlock(); return start - end; }
}

template <typename T>
unsigned long long queue_buffer<T>::addamount(T * _buffer,const unsigned long long &amount) {

}

template <typename T>
unsigned long long queue_buffer<T>::addsome(T * _buffer,const unsigned long long &max_amount, unsigned long long &added_amount) {

}

template <typename T>
bool queue_buffer<T>::get(T &into) {
	m.lock();
	//std::unique_lock<std::mutex> lk(m);
	if (isEmpty()) { m.unlock(); return false; }
	//std::cout << "getting" << std::endl;

	into = buffer[start];
	start = (start + 1) % cap;
	if (start == end) { emptyflag = true; }
	m.unlock();
	return true;
}

template <typename T>
T* queue_buffer<T>::getamount(const unsigned long long &amount) {

}

template <typename T>
T* queue_buffer<T>::getsome(unsigned long long &got_amount,const unsigned long long &max_amount) {

}

template <typename T>
T* queue_buffer<T>::getsome(unsigned long long &got_amount) {

}

template <typename T>
queue_buffer<T>::~queue_buffer() {
	delete buffer;
}
