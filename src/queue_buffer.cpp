#include <QueueBuffer.h>

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
