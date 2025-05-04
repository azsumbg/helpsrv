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

// sorting algorithm for GROUPPER<FPOINT> ************

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

// PROTON ***********************************

dll::PROTON::PROTON()
{
	start.x = 1.0f;
	start.y = 1.0f;

	width = 1.0f;
	height = 1.0f;

	end.x = start.x + width;
	end.y = start.y + height;

	x_radius = start.x + (end.x - start.x) / 2;
	y_radius = start.y + (end.y - start.y) / 2;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}
dll::PROTON::PROTON(float _sx, float _sy, float _width, float _height)
{
	start.x = _sx;
	start.y = _sy;

	width = _width;
	height = _height;

	end.x = start.x + width;
	end.y = start.y + height;

	x_radius = start.x + (end.x - start.x) / 2;
	y_radius = start.y + (end.y - start.y) / 2;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}

void dll::PROTON::SetEdges()
{	
	end.x = start.x + width;
	end.y = start.y + height;

	x_radius = start.x + (end.x - start.x) / 2;
	y_radius = start.y + (end.y - start.y) / 2;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}

void dll::PROTON::NewDims(float _new_width, float _new_height)
{
	width = _new_width;
	height = _new_height;

	end.x = start.x + width;
	end.y = start.y + height;

	x_radius = start.x + (end.x - start.x) / 2;
	y_radius = start.y + (end.y - start.y) / 2;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}
void dll::PROTON::NewWidth(float _new_width)
{
	width = _new_width;
	
	end.x = start.x + width;
	
	x_radius = start.x + (end.x - start.x) / 2;
	
	center.x = start.x + x_radius;
}
void dll::PROTON::NewHeight(float _new_height)
{
	height = _new_height;

	end.y = start.y + height;

	y_radius = start.y + (end.y - start.y) / 2;

	center.y = start.y + y_radius;
}

float dll::PROTON::GetWidth() const
{
	return width;
}
float dll::PROTON::GetHeight() const
{
	return height;
}

/////////////////////////////////////////////

// BASE *************************************

dll::BASE::BASE(uint8_t _what_to_create, float _sx, float _sy) :PROTON()
{
	_type = _what_to_create;

	start.x = _sx;
	start.y = _sy;

	switch (_type)
	{
	case field1_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 16;
		_frame_delay = 5;
		break;

	case field2_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 18;
		_frame_delay = 4;
		break;

	case field3_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 12;
		_frame_delay = 6;
		break;

	case field4_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 20;
		_frame_delay = 4;
		break;

	case field5_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 8;
		_frame_delay = 9;
		break;

	case intro_type:
		NewDims(1000.0f, 800.0f);
		_max_frames = 8;
		_frame_delay = 9;
		break;

	case fireball_type:
		NewDims(20.0f, 12.0f);
		_max_frames = 4;
		_frame_delay = 18;
		break;

	case ev_archer_type:
		NewDims(80.0f, 80.0f);
		_max_frames = 6;
		_frame_delay = 12;
		break;

	case ev_coyote_type:
		NewDims(100.0f, 51.0f);
		_max_frames = 26;
		_frame_delay = 3;
		break;

	case ev_dragon_type:
		NewDims(200.0f, 120.0f);
		_max_frames = 21;
		_frame_delay = 4;
		break;

	case ev_hydra_type:
		NewDims(100.0f, 75.0f);
		_max_frames = 12;
		_frame_delay = 6;
		break;

	case ev_mage_type:
		NewDims(70.0f, 70.0f);
		_max_frames = 6;
		_frame_delay = 12;
		break;

	case ev_minotaur_type:
		NewDims(78.0f, 100.0f);
		_max_frames = 7;
		_frame_delay = 10;
		break;

	case ev_warrior_type:
		NewDims(80.0f, 59.0f);
		_max_frames = 8;
		_frame_delay = 12;
		break;

	case gd_archer_type:
		NewDims(80.0f, 57.0f);
		_max_frames = 42;
		_frame_delay = 2;
		break;

	case gd_horse_type:
		NewDims(100.0f, 94.0f);
		_max_frames = 44;
		_frame_delay = 2;
		break;

	case gd_dragon_type:
		NewDims(180.0f, 126.0f);
		_max_frames = 7;
		_frame_delay = 10;
		break;

	case gd_hydra_type:
		NewDims(100.0f, 57.0f);
		_max_frames = 60;
		_frame_delay = 1;
		break;

	case gd_unicorn_type:
		NewDims(110.0f, 96.0f);
		_max_frames = 20;
		_frame_delay = 4;
		break;

	case gd_minotaur_type:
		NewDims(78.0f, 100.0f);
		_max_frames = 7;
		_frame_delay = 10;
		break;

	case gd_warrior_type:
		NewDims(78.0f, 90.0f);
		_max_frames = 8;
		_frame_delay = 12;
		break;
	}
}

int dll::BASE::GetFrame()
{
	--_frame_delay;
	if (_frame_delay <= 0)
	{
		switch (_type)
		{
		case field1_type:
			_frame_delay = 5;
			break;

		case field2_type:
			_frame_delay = 4;
			break;

		case field3_type:
			_frame_delay = 6;
			break;

		case field4_type:
			_frame_delay = 4;
			break;

		case field5_type:
			_frame_delay = 9;
			break;

		case intro_type:
			_frame_delay = 9;
			break;

		case fireball_type:
			_frame_delay = 18;
			break;

		case ev_archer_type:
			_frame_delay = 12;
			break;

		case ev_coyote_type:
			_frame_delay = 3;
			break;

		case ev_dragon_type:
			_frame_delay = 4;
			break;

		case ev_hydra_type:
			_frame_delay = 6;
			break;

		case ev_mage_type:
			_frame_delay = 12;
			break;

		case ev_minotaur_type:
			_frame_delay = 10;
			break;

		case ev_warrior_type:
			_frame_delay = 12;
			break;

		case gd_archer_type:
			_frame_delay = 2;
			break;

		case gd_horse_type:
			_frame_delay = 2;
			break;

		case gd_dragon_type:
			_frame_delay = 10;
			break;

		case gd_hydra_type:
			_frame_delay = 1;
			break;

		case gd_unicorn_type:
			_frame_delay = 4;
			break;

		case gd_minotaur_type:
			_frame_delay = 10;
			break;

		case gd_warrior_type:
			_frame_delay = 12;
			break;
		}

		++_frame;
		if (_frame >= _max_frames)_frame = 0;
	}

	return _frame;
}
uint8_t dll::BASE::GetType() const
{
	return _type;
}
bool dll::BASE::ChangeType(uint8_t _to_what)
{
	_type = _to_what;
	_frame = 0;

	switch (_type)
	{
	case field1_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 16;
		_frame_delay = 5;
		break;

	case field2_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 18;
		_frame_delay = 4;
		break;

	case field3_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 12;
		_frame_delay = 6;
		break;

	case field4_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 20;
		_frame_delay = 4;
		break;

	case field5_type:
		NewDims(1000.0f, 750.0f);
		_max_frames = 8;
		_frame_delay = 9;
		break;

	case intro_type:
		NewDims(1000.0f, 800.0f);
		_max_frames = 8;
		_frame_delay = 9;
		break;

	case fireball_type:
		NewDims(20.0f, 12.0f);
		_max_frames = 4;
		_frame_delay = 18;
		break;

	case ev_archer_type:
		NewDims(80.0f, 80.0f);
		_max_frames = 6;
		_frame_delay = 12;
		break;

	case ev_coyote_type:
		NewDims(100.0f, 51.0f);
		_max_frames = 26;
		_frame_delay = 3;
		break;

	case ev_dragon_type:
		NewDims(200.0f, 120.0f);
		_max_frames = 21;
		_frame_delay = 4;
		break;

	case ev_hydra_type:
		NewDims(100.0f, 75.0f);
		_max_frames = 12;
		_frame_delay = 6;
		break;

	case ev_mage_type:
		NewDims(70.0f, 70.0f);
		_max_frames = 6;
		_frame_delay = 12;
		break;

	case ev_minotaur_type:
		NewDims(78.0f, 100.0f);
		_max_frames = 7;
		_frame_delay = 10;
		break;

	case ev_warrior_type:
		NewDims(80.0f, 59.0f);
		_max_frames = 8;
		_frame_delay = 12;
		break;

	case gd_archer_type:
		NewDims(80.0f, 57.0f);
		_max_frames = 42;
		_frame_delay = 2;
		break;

	case gd_horse_type:
		NewDims(100.0f, 94.0f);
		_max_frames = 44;
		_frame_delay = 2;
		break;

	case gd_dragon_type:
		NewDims(180.0f, 126.0f);
		_max_frames = 7;
		_frame_delay = 10;
		break;

	case gd_hydra_type:
		NewDims(100.0f, 57.0f);
		_max_frames = 60;
		_frame_delay = 1;
		break;

	case gd_unicorn_type:
		NewDims(110.0f, 96.0f);
		_max_frames = 20;
		_frame_delay = 4;
		break;

	case gd_minotaur_type:
		NewDims(78.0f, 100.0f);
		_max_frames = 7;
		_frame_delay = 10;
		break;

	case gd_warrior_type:
		NewDims(78.0f, 90.0f);
		_max_frames = 8;
		_frame_delay = 12;
		break;
	}
}

/////////////////////////////////////////////