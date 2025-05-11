#pragma once

#ifdef HELPSRV_EXPORTS
#define HELPSRV_API __declspec(dllexport)
#else
#define HELPSRV_API __declspec(dllimport)
#endif

#include <random>

constexpr float scr_width{ 1000.0f };
constexpr float scr_height{ 800.0f };

constexpr float sky{ 50.0f };
constexpr float ground{ 750.0f };

constexpr uint8_t no_type{ 0 };

constexpr uint8_t field1_type{ 1 };
constexpr uint8_t field2_type{ 2 };
constexpr uint8_t field3_type{ 3 };
constexpr uint8_t field4_type{ 4 };
constexpr uint8_t field5_type{ 5 };
constexpr uint8_t intro_type{ 6 };

constexpr uint8_t fireball_type{ 7 };

constexpr uint8_t ev_archer_type{ 8 };
constexpr uint8_t ev_arrow_type{ 9 };
constexpr uint8_t ev_coyote_type{ 10 };
constexpr uint8_t ev_dragon_type{ 11 };
constexpr uint8_t ev_hydra_type{ 12 };
constexpr uint8_t ev_mage_type{ 13 };
constexpr uint8_t ev_minotaur_type{ 14 };
constexpr uint8_t ev_warrior_type{ 15 };

constexpr uint8_t gd_archer_type{ 16 };
constexpr uint8_t gd_arrow_type{ 17 };
constexpr uint8_t gd_horse_type{ 18 };
constexpr uint8_t gd_dragon_type{ 19 };
constexpr uint8_t gd_hydra_type{ 20 };
constexpr uint8_t gd_unicorn_type{ 21 };
constexpr uint8_t gd_minotaur_type{ 22 };
constexpr uint8_t gd_warrior_type{ 23 };


enum class dirs { stop = 0, up = 1, down = 2, left = 3, right = 4 };
enum class states { move = 0, attack = 1, heal = 2, flee = 3, stop = 4, shoot = 5,next_turn = 6 };

struct FPOINT
{
	float x{ 0 };
	float y{ 0 };
};

namespace dll
{
	class HELPSRV_API RANDIt
	{
	private:

		std::random_device rd{};
		std::seed_seq* sq{ nullptr };
		std::mt19937* twister{ nullptr };

	public:
		RANDIt();
		~RANDIt();

		int operator()(int min, int max);
	};

	template<typename T> class GROUPPER
	{
	private:
		T* mPtr{ nullptr };
		size_t max_size{ 0 };
		size_t next_pos{ 0 };

		bool is_valid{ false };

	public:
		GROUPPER();
		GROUPPER(size_t lenght);
		~GROUPPER();

		size_t capacity() const;
		size_t size() const;
		bool valid() const;

		void push_back(T element);
		void push_front(T element);

		bool insert(size_t index, T element);
		bool erase(size_t index);

		T& begin();
		T& last();

		T& operator[](size_t index);
		void operator()(size_t index, T element);
	};

	float HELPSRV_API Distance(FPOINT start_point, FPOINT target_point);

	bool HELPSRV_API Sort(GROUPPER<FPOINT>& bag, FPOINT target);

	//////////////////////////////////////////////////////

	class HELPSRV_API PROTON
	{
	protected:
		float width{ 0 };
		float height{ 0 };

	public:
		FPOINT start{};
		FPOINT end{};
		FPOINT center{};
		float x_radius{0};
		float y_radius{ 0 };

		PROTON();
		PROTON(float _sx, float _sy, float _width, float _height);
		virtual ~PROTON() {};

		void SetEdges();

		void NewDims(float _new_width, float _new_height);
		void NewWidth(float _new_width);
		void NewHeight(float _new_height);

		float GetWidth() const;
		float GetHeight() const;
	};

	class HELPSRV_API BASE :public PROTON
	{
	protected:
		uint8_t _type = no_type;

		int _frame{ 0 };
		int _max_frames{ 0 };
		int _frame_delay{ 0 };

	public:
		dirs dir{ dirs::stop };

		BASE(uint8_t _what_to_create, float _sx, float _sy);
		virtual ~BASE() {};

		int GetFrame();
		uint8_t GetType() const;
		void ChangeType(uint8_t _to_what);
	};

	class HELPSRV_API SHOTS :public BASE
	{
	protected:
		
		float init_x{ 0 };
		float init_y{ 0 };
		float target_x{ 0 };
		float target_y{ 0 };

		float slope{ 0 };
		float intercept{ 0 };

		bool hor_dir{ false };
		bool vert_dir{ false };

	public:
		int strenght{ 0 };

		SHOTS(uint8_t what, float _tox, float _toy, float _targ_x, float _targ_y);

		bool Move();
		void Release();

		friend HELPSRV_API SHOTS* ShotFactory(uint8_t which, float sx, float sy, float to_x, float to_y);
	};

	class HELPSRV_API CREATURE :public BASE
	{
	protected:

		bool hor_dir{ false };
		bool vert_dir{ false };

		float move_sx{ 0 };
		float move_sy{ 0 };

		float move_ex{ 0 };
		float move_ey{ 0 };

		float slope{ 0 };
		float intercept{ 0 };

		void SetPathInfo(float _to_where_x, float _to_where_y);

		int move_points{ 0 };
		int max_move_points{ 0 };
		int attack_delay{ 0 };
		int heal_delay{ 0 };

		int max_lifes{ 0 };
	public:

		int strenght{ 0 };
		int lifes{ 0 };
		states state{ states::stop };

		CREATURE(uint8_t _whattype, float _wherex, float _wherey);
		virtual ~CREATURE() {};

		bool Move(float _to_x, float _to_y);
		int Attack();
		void Heal();
		int GetMaxLifes()const;
		virtual states AINextMove(GROUPPER<FPOINT>& Enemies) = 0;
		virtual void Release() = 0;
	};

	class EVILS :public CREATURE
	{
	protected:
		EVILS(uint8_t _which, float _sx, float _sy);

	public:

		states AINextMove(GROUPPER<FPOINT>& Enemies) override;
		void Release() override;

		friend HELPSRV_API CREATURE* CreatureFactory(uint8_t which, float sx, float sy);
	};

	class HERO :public CREATURE
	{
	protected:
		HERO(uint8_t _which, float _sx, float _sy);

	public:

		states AINextMove(GROUPPER<FPOINT>& Enemies) override;
		void Release() override;

		friend HELPSRV_API CREATURE* CreatureFactory(uint8_t which, float sx, float sy);
	};

	//////////////////////////////////////////////////////

	typedef BASE* Fields;
	typedef CREATURE* Creatures;
	typedef SHOTS* Shot;

	HELPSRV_API Creatures CreatureFactory(uint8_t which, float sx, float sy);

	HELPSRV_API Shot ShotFactory(uint8_t which, float sx, float sy, float to_x, float to_y);

	//////////////////////////////////////////////////////

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

		return *dummy;
	}
	template<typename T> void dll::GROUPPER<T>::operator()(size_t index, T element)
	{
		if (index < next_pos)mPtr[index] = element;
	}

	template<typename T> T& dll::GROUPPER<T>::begin()
	{
		T* dummy{ reinterpret_cast<T*>(calloc(1,sizeof(T))) };
		if (is_valid && next_pos > 0)return *mPtr;
		return *dummy;
	}
	template<typename T> T& dll::GROUPPER<T>::last()
	{
		T* dummy{ reinterpret_cast<T*>(calloc(1,sizeof(T))) };
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
				T* tempPtr{ reinterpret_cast<T*>(calloc(max_size + 1,sizeof(T))) };
				for (size_t count = 0; count < max_size + 1; ++count)
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
				T* tempPtr{ reinterpret_cast<T*>(calloc(max_size,sizeof(T))) };
				for (size_t count = 0; count < next_pos; ++count)
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
		if (!is_valid || next_pos <= 1)return false;

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
}