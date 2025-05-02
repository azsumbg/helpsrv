#include "pch.h"
#include "helpsrv.h"

// RANDIt ************************************

dll::RANDIt::RANDIt()
{
	sq = new std::seed_seq{ rd(), rd(), rd(), rd(), rd(), rd(), rd(), rd() };
	twister = new std::mt19937{ *sq };
}
dll::RANDIt::~RANDIt()
{
	if (sq)delete sq;
	if (twister)delete twister;
}

int dll::RANDIt::operator()(int min, int max)
{
	std::uniform_int_distribution machine{ min, max };
	return machine(*twister);
}

//////////////////////////////////////////////

// GROUPPER *********************************

template<typename T> dll::GROUPPER<T>::GROUPPER() :max_size{ 1 }, mPtr{ reinterpret_cast<T*>(calloc(1,sizeof(T))) }
{
	if (mPtr)is_valid = true;
}
template<typename T> dll::GROUPPER<T>::GROUPPER(size_t lenght) : max_size{ lenght }, mPtr{ reinterpret_cast<T*>(calloc(lenght,sizeof(T))) }
{
	if (mPtr)is_valid = true;
}
template<typename T> dll::GROUPPER<T>::~GROUPPER()
{
	if (mPtr)free(mPtr);
}

template<typename T> size_t dll::GROUPPER<T>::capacity() const
{
	return max_size;
}
template<typename T> size_t dll::GROUPPER<T>::size() const
{
	return next_pos;
}
template<typename T> bool dll::GROUPPER<T>::valid() const
{
	return is_valid;
}

template<typename T> void dll::GROUPPER<T>::push_back(T element)
{
	if (!is_valid)return;

	if (next_pos < max_size)
	{
		mPtr[next_pos] = element;
		next_pos++;
		return;
	}

	T* tempPtr{ reinterpret_cast<T*>(calloc(max_size + 1,sizeof(T))) };
	for (size_t count = 0; count < max_size; ++count)tempPtr[count] = mPtr[count];
	
	tempPtr[max_size] = element;
	mPtr = tempPtr;
	++max_size;
	++next_pos;
}
template<typename T> void dll::GROUPPER<T>::push_front(T element)
{
	if (is_valid)*mPtr = element;
}

template<typename T> T& dll::GROUPPER<T>::operator[](size_t index)
{
	T* dummy{ reinterpret_cast<T*>(calloc(1,sizeof(T))) };
	if (index < next_pos)return mPtr[index];

	return dummy;
}
template<typename T> void dll::GROUPPER<T>::operator()(size_t index, T element)
{
	if (index < next_pos)mPtr[index] = element;
}

template<typename T> T& dll::GROUPPER<T>::begin()
{
	T* dummy{ reinterpret_cast(calloc(1,sizeof(T))) };
	if (is_valid && next_pos > 0)return *mPtr;
	return dummy;
}
template<typename T> T& dll::GROUPPER<T>::last()
{
	T* dummy{ reinterpret_cast(calloc(1,sizeof(T))) };
	if (is_valid && next_pos > 0)return mPtr[next_pos - 1];
	return dummy;
}

template<typename T> bool dll::GROUPPER<T>::insert(size_t index, T element)
{
	if (!is_valid || index >= next_pos || next_pos == 0)return false;

	if (index == next_pos - 1)
	{
		mPtr[next_pos - 1] = element;
		return true;
	}
	else
	{
		if (next_pos >= max_size)
		{
			T* tempPtr{ reinterpret_cast<T*>(calloc(max_size + 1,sizeof(T)) };
			for (size_t count = 0; count < max_size + 1, ++count)
			{
				if (count < index)tempPtr[count] = mPtr[count];
				else if (count == index)tempPtr[count] = element;
				else tempPtr[count + 1] = mPtr[count];
			}
			mPtr = tempPtr;
			++max_size;
			++next_pos;
			return true;
		}
		else
		{
			T* tempPtr{ reinterpret_cast<T*>(calloc(max_size,sizeof(T)) };
			for (size_t count = 0; count < next_pos, ++count)
			{
				if (count < index)tempPtr[count] = mPtr[count];
				else if (count == index)tempPtr[count] = element;
				else tempPtr[count + 1] = mPtr[count];
			}
			mPtr = tempPtr;
			++next_pos;
			return true;
		}
	}

	return false;
}
template<typename T> bool dll::GROUPPER<T>::erase(size_t index)
{
	if(!is_valid || next_pos <= 1)return false;

	T* tempPtr{ reinterpret_cast<T*>(calloc(max_size,sizeof(T))) };

	for (size_t count = 0; count < next_pos - 1; count++)
	{
		if (count < index)tempPtr[count] = mPtr[count];
		else if (count >= index)tempPtr[count] = mPtr[count + 1];
	}
	mPtr = tempPtr;
	next_pos--;
}

///////////////////////////////////////////////

// Distance algorithm *************************

float dll::Distance(FPOINT start_point, FPOINT target_point)
{
	float a = (float)(pow(abs(target_point.x - start_point.x), 2));
	float b = (float)(pow(abs(target_point.y - start_point.y), 2));

	return sqrt(a + b);
}

//////////////////////////////////////////////

// sorting algorithm for GROUPPER ************

bool dll::Sort(GROUPPER<FPOINT>& Bag, FPOINT target)
{
	if (Bag.size() < 2)return false;

	bool sorted = false;
	
	while (!sorted)
	{
		sorted = true;

		for (size_t pos = 0; pos < Bag.size() - 1; ++pos)
		{
			if (Distance(Bag[pos], target) > Distance(Bag[pos + 1], target))
			{
				FPOINT temp = Bag[pos];
				Bag(pos, Bag[pos + 1]);
				Bag((pos + 1), temp);
				sorted = false;
				break;
			}
		}
	}

	return true;
}

//////////////////////////////////////////////