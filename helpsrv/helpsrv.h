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
enum class states { move = 0, attack = 1, heal = 2, flee = 3 };

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

	template<typename T> class HELPSRV_API GROUPPER
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

	float Distance(FPOINT start_point, FPOINT target_point);

	bool Sort(GROUPPER<FPOINT>& bag, FPOINT target);

}