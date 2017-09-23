#include <QueueBuffer.h>

//! \todo TODO: clean this up, jesus
//! \todo TODO: finish the implementation of queue buffer

//TMP
#include <iostream>

template <typename T>
QueueBuffer<T>::QueueBuffer(unsigned int _capacity)
{
	cap = _capacity;
	buffer = new T[cap];

	start = 0;
	end = 0;

	emptyflag = true;
}

template <typename T>
int QueueBuffer<T>::add(T &_buffer)
{
	std::lock_guard<std::mutex> lock(m);
	if (start == end)
	{
		if (isEmpty())
		{
			//empty - add and set to not empty
			emptyflag = false;
		}
		else
		{
			//full
			return -1;
		}
	}
	buffer[end] = _buffer;
	end = (end + 1) % cap;
	//returns capacity left
	return cap - currentSize();
}

template <typename T>
int QueueBuffer<T>::add(std::vector<T> &_buffer)
{
	std::lock_guard<std::mutex> lock(m);
	unsigned int current_cap = (cap - currentSize());
	if (current_cap < _buffer.size())
	{
		return current_cap;
	}
	std::copy(_buffer.begin(), _buffer.end(), buffer + end);
	emptyflag = false;
	end = (end + _buffer.size()) % cap;
	_buffer.clear();
	//returns capacity left
	return cap - currentSize();
}

template <typename T>
std::vector<T> QueueBuffer<T>::get(const unsigned int &amount)
{
	std::lock_guard<std::mutex> lock(m);
	//std::vector<T> res(amount,0);
	std::vector<T> res;
	//into = buffer[start];
	/*start = (start + 1) % cap;
	if (start == end)
	{
		emptyflag = true;
	}
	if (start < end)
	{
		return end - start;
	}
	if (!emptyflag && start == end)
	{
		return cap;
	}
	return cap - start + end;*/
	//tmp
	//std::cout<<"Asked for: "<<amount<<", can give: "<<currentSize()<<std::endl;
	if(amount>currentSize()){std::cout<<"NOT ENOUGH IN BUFFER!"<<std::endl;}
	if (end > start)
	{
		if(start+amount<end){
			//start->start+amount
			//std::copy(&buffer[start],&buffer[start+amount],res.begin());
			//std::cout<<"start->start+amount"<<std::endl;
			res.insert(res.begin(),&buffer[start],&buffer[start+amount]);
			start = start + amount;
			return res;
		}
		else{
			//start->end
			//std::copy(&buffer[start],&buffer[end],res.begin());
			//std::cout<<"start->end"<<std::endl;
			res.insert(res.begin(),&buffer[start],&buffer[end]);			
			start = (unsigned int)end;
			emptyflag = true;
		}
	}
	else
	{
		unsigned int remaining = amount;
		if(start+amount<cap){
			//start->start+amount
			//std::copy(&buffer[start],&buffer[start+amount],res.begin());
			//std::cout<<"start->start+amount(end)"<<std::endl;
			res.insert(res.begin(),&buffer[start],&buffer[start+amount]);			
			start = start + amount;
		}
		else{
			remaining-=cap-start;
			//start->cap
			//std::copy(&buffer[start],&buffer[cap],res.begin());
			res.insert(res.begin(),&buffer[start],&buffer[cap]);			
			//std::cout<<"start->cap"<<std::endl;			
			if(remaining<end){
				//0->remaining
				//std::copy(buffer,&buffer[remaining],res.begin()+(cap-start));
				res.insert(res.begin()+(cap-start),buffer,&buffer[remaining]);				
				//std::cout<<"0->remaining"<<std::endl;
				start = remaining;
			}
			else{
				//0->end
				//std::copy(buffer,&buffer[end],res.begin()+(cap-start));
				res.insert(res.begin()+(cap-start),buffer,&buffer[end]);				
				//std::cout<<"0->end"<<std::endl;
				emptyflag = true;
				start = (unsigned int)end;
			}
		}
	}
	return res;
}

template <typename T>
QueueBuffer<T>::~QueueBuffer()
{
	delete buffer;
}

template class QueueBuffer<float>;
template class QueueBuffer<int>;