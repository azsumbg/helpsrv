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

///////////////////////////////////////////////