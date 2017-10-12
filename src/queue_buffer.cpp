#include <QueueBuffer.h>

//! \todo TODO: clean this up, jesus
//BUGHUNT: audio is messed after playing about one fill worth of audio
// -> i think adding and/or reading from the fringes has errors.

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
bool QueueBuffer<T>::add(T &_buffer)
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
			return false;
		}
	}
	buffer[end] = _buffer;
	end = (end + 1) % cap;
	return true;
}

template <typename T>
int QueueBuffer<T>::add(std::vector<T> &_buffer)
{
	std::lock_guard<std::mutex> lock(m);
	unsigned int current_cap = (cap - currentSize());
	if (current_cap == 0)
		return 0;
	emptyflag = false;
	if(end > start){
		unsigned int spaceAtEnd = cap-end;
		unsigned int spaceAtStart = start;
		unsigned int remaining = _buffer.size();
		//fits to end
		if(remaining<=spaceAtEnd){
			std::copy(_buffer.begin(),_buffer.end(),&buffer[end]);	
			end+=_buffer.size();
			return _buffer.size();		
		}
		//copy to end to cap and to 0 to start
		std::copy(_buffer.begin(),_buffer.begin()+spaceAtEnd,&buffer[end]);
		remaining-=spaceAtEnd;
		if(remaining<=spaceAtStart){
			std::copy(_buffer.begin()+spaceAtEnd,_buffer.end(),&buffer[0]);	
			end+=_buffer.size();			
			return _buffer.size();		
		}
		std::copy(_buffer.begin()+spaceAtEnd,_buffer.begin()+spaceAtEnd+spaceAtStart,&buffer[0]);
		end = static_cast<unsigned int>(start);
		return spaceAtEnd + spaceAtStart;
	}
	else{
		//free space is in the middle
		if(_buffer.size()<=current_cap){
			std::copy(_buffer.begin(),_buffer.end(),&buffer[end]);
			end+=_buffer.size();
			return _buffer.size();				
		}
		std::copy(_buffer.begin(),_buffer.begin()+current_cap,&buffer[end]);
		end = static_cast<unsigned int>(start);
		return current_cap;
	}
}

template <typename T>
std::vector<T> QueueBuffer<T>::get(const unsigned int &amount)
{
	std::lock_guard<std::mutex> lock(m);
	std::vector<T> res;
	if (emptyflag) return res;
	res.reserve(amount);
	if (end > start)
	{
		if (start + amount < end)
		{
			res.insert(res.begin(), &buffer[start], &buffer[start + amount]);
			start = start + amount;
			return res;
		}
		else
		{
			res.insert(res.begin(), &buffer[start], &buffer[end]);
			start = (unsigned int)end;
			emptyflag = true;
		}
	}
	else
	{
		unsigned int remaining = amount;
		if (start + amount < cap)
		{
			res.insert(res.begin(), &buffer[start], &buffer[start + amount]);
			start = start + amount;
		}
		else
		{
			remaining -= cap - start;
			res.insert(res.begin(), &buffer[start], &buffer[cap]);
			if (remaining < end)
			{
				res.insert(res.begin() + (cap - start), buffer, &buffer[remaining]);
				start = remaining;
			}
			else
			{
				res.insert(res.begin() + (cap - start), buffer, &buffer[end]);
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