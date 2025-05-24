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

	x_radius = (end.x - start.x) / 2;
	y_radius = (end.y - start.y) / 2;

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

	x_radius = (end.x - start.x) / 2;
	y_radius = (end.y - start.y) / 2;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}

void dll::PROTON::SetEdges()
{	
	end.x = start.x + width;
	end.y = start.y + height;

	x_radius = (end.x - start.x) / 2;
	y_radius = (end.y - start.y) / 2;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}

void dll::PROTON::NewDims(float _new_width, float _new_height)
{
	width = _new_width;
	height = _new_height;

	end.x = start.x + width;
	end.y = start.y + height;

	x_radius = (end.x - start.x) / 2;
	y_radius = (end.y - start.y) / 2;

	center.x = start.x + x_radius;
	center.y = start.y + y_radius;
}
void dll::PROTON::NewWidth(float _new_width)
{
	width = _new_width;
	
	end.x = start.x + width;
	
	x_radius = (end.x - start.x) / 2;
	
	center.x = start.x + x_radius;
}
void dll::PROTON::NewHeight(float _new_height)
{
	height = _new_height;

	end.y = start.y + height;

	y_radius = (end.y - start.y) / 2;

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

	case ev_arrow_type:
		NewDims(26.0f, 26.0f);
		_max_frames = 0;
		break;

	case gd_arrow_type:
		NewDims(26.0f, 26.0f);
		_max_frames = 0;
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
void dll::BASE::ChangeType(uint8_t _to_what)
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

// CREATURE *********************************

void dll::CREATURE::SetPathInfo(float _to_where_x, float _to_where_y)
{
	move_sx = start.x;
	move_sy = start.y;

	move_ex = _to_where_x;
	move_ey = _to_where_y;

	if (move_sx == move_ex)
	{
		vert_dir = true;
		return;
	}
	if (move_sy == move_ey)
	{
		hor_dir = true;
		return;
	}

	slope = (move_ey - move_sy) / (move_ex - move_sx);
	intercept = move_sy - slope * move_sx;
}

dll::CREATURE::CREATURE(uint8_t _whattype, float _wherex, float _wherey) :BASE(_whattype, _wherex, _wherey)
{
	switch (_whattype)
	{
	case ev_archer_type:
		move_points = 150;
		heal_delay = 2;
		max_lifes = 80;
		strenght = 40;
		lifes = max_lifes;
		break;

	case ev_coyote_type:
		move_points = 200;
		heal_delay = 3;
		max_lifes = 100;
		strenght = 50;
		lifes = max_lifes;
		break;

	case ev_dragon_type:
		move_points = 250;
		heal_delay = 4;
		max_lifes = 150;
		strenght = 80;
		lifes = max_lifes;
		break;

	case ev_hydra_type:
		move_points = 150;
		heal_delay = 3;
		max_lifes = 120;
		strenght = 50;
		lifes = max_lifes;
		break;

	case ev_mage_type:
		move_points = 100;
		heal_delay = 3;
		max_lifes = 50;
		strenght = 40;
		lifes = max_lifes;
		break;

	case ev_minotaur_type:
		move_points = 250;
		heal_delay = 5;
		max_lifes = 120;
		strenght = 65;
		lifes = max_lifes;
		break;

	case ev_warrior_type:
		move_points = 250;
		heal_delay = 2;
		max_lifes = 90;
		strenght = 30;
		lifes = max_lifes;
		break;

	case gd_archer_type:
		move_points = 100;
		heal_delay = 2;
		max_lifes = 80;
		strenght = 45;
		lifes = max_lifes;
		break;

	case gd_horse_type:
		move_points = 250;
		heal_delay = 4;
		max_lifes = 90;
		strenght = 40;
		lifes = max_lifes;
		break;

	case gd_dragon_type:
		move_points = 200;
		heal_delay = 4;
		max_lifes = 150;
		strenght = 85;
		lifes = max_lifes;
		break;

	case gd_hydra_type:
		move_points = 150;
		heal_delay = 3;
		max_lifes = 120;
		strenght = 50;
		lifes = max_lifes;
		break;

	case gd_unicorn_type:
		move_points = 250;
		heal_delay = 3;
		max_lifes = 100;
		strenght = 45;
		lifes = max_lifes;
		break;

	case gd_minotaur_type:
		move_points = 250;
		heal_delay = 3;
		max_lifes = 100;
		strenght = 70;
		lifes = max_lifes;
		break;

	case gd_warrior_type:
		move_points = 200;
		heal_delay = 2;
		max_lifes = 100;
		strenght = 35;
		lifes = max_lifes;
		break;
	}

	max_move_points = move_points;
}

bool dll::CREATURE::Move(float _to_x, float _to_y)
{
	if (move_points <= 0)
	{
		state = states::next_turn;
		return false;
	}

	SetPathInfo(_to_x, _to_y);

	if (vert_dir)
	{
		if (move_ey < move_sy)
		{
			if (start.y <= move_ey)
			{
				move_points = 0;
				return false;
			}

			if (start.y - 1 >= sky)
			{
				--start.y;
				SetEdges();
				move_points--;
				return true;
			}
			else
			{
				start.y += 10.0f;
				SetEdges();
				move_points = 0;
				return false;
			}
		}
		else if (move_ey > move_sy)
		{
			if (end.y >= move_ey)
			{
				move_points = 0;
				return false;
			}
			
			if (end.y + 1.0f <= ground)
			{
				++start.y;
				SetEdges();
				move_points -= 1;
				return true;
			}
			else
			{
				start.y -= 10.0f;
				SetEdges();
				move_points = 0;
				return false;
			}
		}	
	}
	else if (hor_dir)
	{
		if (move_ex < move_sx)
		{
			if (start.x <= move_ex) 
			{
				move_points = 0;
				return false;
			}
			if (start.x - 1.0f >= 0)
			{
				--start.x;
				SetEdges();
				--move_points;
				return true;
			}
			else
			{
				move_points = 0;
				return false;
			}
		}
		else if (move_ex > move_sx)
		{
			if (end.x >= move_ex)return false;
			
			if (end.x + 1.0f <= scr_width)
			{
				++start.x;
				SetEdges();
				--move_points;
				return true;
			}
		}
	}
	else if (move_ex < move_sx)
	{
		if (start.x <= move_ex)
		{
			if ((move_ey > move_sy && end.y >= move_ey) || (move_ey < move_sy && start.y <= move_ey))
			{
				move_points = 0;
				return false;
			}
		}
		if (start.x - 1.0f > 0 && start.y >= sky && end.y <= ground)
		{
			--start.x;
			start.y = start.x * slope + intercept;
			SetEdges();
			--move_points;
			return true;
		}
		else
		{
			if (start.y < sky)
			{
				start.y += 10.0f;
				SetEdges();
			}
			if (end.y > ground)
			{
				start.y -= 10.0f;
				SetEdges();
			}
			move_points = 0;
			return false;
		}
	}
	else if (move_ex > move_sx)
	{
		if (end.x >= move_ex)
		{
			if ((move_ey > move_sy && end.y >= move_ey) || (move_ey < move_sy && start.y <= move_ey))
			{
				move_points = 0;
				return false;
			}
		}
		if (end.x + 1.0f < scr_width && start.y >= sky && end.y <= ground)
		{
			++start.x;
			start.y = start.x * slope + intercept;
			SetEdges();
			--move_points;
			
			return true;
		}
		else
		{
			if (start.y < sky)
			{
				start.y += 10.0f;
				SetEdges();
			}
			if (end.y > ground)
			{
				start.y -= 10.0f;
				SetEdges();
			}

			move_points = 0;
			return false;
		}
	}
	
	if (move_points <= 0)state = states::next_turn;
	return false;
}
int dll::CREATURE::Attack()
{
	state = states::next_turn;
	return strenght;
}
void dll::CREATURE::Heal()
{
	--heal_delay;
	if (heal_delay <= 0)
	{
		switch (_type)
		{
		case ev_archer_type:
			heal_delay = 2;
			break;

		case ev_coyote_type:
			heal_delay = 3;
			break;

		case ev_dragon_type:
			heal_delay = 4;
			break;

		case ev_hydra_type:
			heal_delay = 3;
			break;

		case ev_mage_type:
			heal_delay = 3;
			break;

		case ev_minotaur_type:
			heal_delay = 5;
			break;

		case ev_warrior_type:
			heal_delay = 2;
			break;

		case gd_archer_type:
			heal_delay = 2;
			break;

		case gd_horse_type:
			heal_delay = 4;
			break;

		case gd_dragon_type:
			heal_delay = 4;
			break;

		case gd_hydra_type:
			heal_delay = 3;
			break;

		case gd_unicorn_type:
			heal_delay = 3;
			break;

		case gd_minotaur_type:
			heal_delay = 3;
			break;

		case gd_warrior_type:
			heal_delay = 2;
			break;
		}

		RANDIt _Randerer;

		if (lifes + 40 + _Randerer(0, 20) <= max_lifes)lifes = 40 + _Randerer(0, 20);
		else lifes = max_lifes;
		state = states::next_turn;
	}
	state = states::next_turn;
}
int dll::CREATURE::GetMaxLifes()const
{
	return max_lifes;
}
int dll::CREATURE::GetMovePoints()const
{
	return move_points;
}
int dll::CREATURE::GetMaxMovePoints() const
{
	return max_move_points;
}
void dll::CREATURE::SetMovePoints(int points)
{
	move_points = points;
}

/////////////////////////////////////////////

// EVILS ***********************************

dll::EVILS::EVILS(uint8_t _which, float _sx, float _sy) :CREATURE(_which, _sx, _sy) 
{
	dir = dirs::left;
};

states dll::EVILS::AINextMove(GROUPPER<FPOINT>& Enemies)
{
	if (move_points <= 0 || state == states::next_turn)
	{
		move_points = max_move_points;
		state = states::next_turn;
		return state;
	}
	else
	{
		RANDIt _Randerer;

		if ((lifes < max_lifes / 2 && _Randerer(0, 5) == 1) || (state == states::heal && lifes < max_lifes))
		{
			Heal();
			state = states::heal;
			move_points -= 10;
			if (lifes >= max_lifes)
			{
				state = states::stop;
				return state;
			}
			return state;
		}

		state = states::next_turn;

		Sort(Enemies, center);

		bool melee = false;

		if (Enemies.begin().x >= start.x && Enemies.begin().x <= end.x && Enemies.begin().y >= start.y && Enemies.begin().y <= end.y)
			melee = true;

		if (_type == ev_archer_type || _type == gd_archer_type || _type == ev_mage_type)
		{
			if (melee)
			{
				if (move_points <= 0)state = states::next_turn;
				else
				{
					state = states::attack;
					move_points = 0;
				}
				return state;
			}
			else
			{
				if (move_points <= 0)state = states::next_turn;
				else
				{
					state = states::shoot;
					move_points -= max_move_points;
				}
				return state;
			}
		}
		else
		{
			if (melee)
			{
				if (move_points <= 0)state = states::next_turn;
				else
				{
					state = states::attack;
					move_points = 0;
				}
				return state;
			}
			else state = states::move;
		}
	}
	return state;
}
void dll::EVILS::Release()
{
	delete this;
}

///////////////////////////////////////////

// HERO ***********************************

dll::HERO::HERO(uint8_t _which, float _sx, float _sy) :CREATURE(_which, _sx, _sy) 
{
	dir = dirs::right;
};

states dll::HERO::AINextMove(GROUPPER<FPOINT>& Enemies)
{
	if (move_points <= 0)
	{
		move_points = max_move_points;
		state = states::next_turn;
		return state;
	}

	if (lifes < max_lifes / 2)
	{
		state = states::heal;
		if (lifes >= max_lifes)
		{
			state = states::stop;
			return state;
		}
		return state;
	}

	state = states::stop;

	
	return state;
}
void dll::HERO::Release()
{
	delete this;
}

///////////////////////////////////////////

// SHOTS **********************************

dll::SHOTS::SHOTS(uint8_t what, float _tox, float _toy, float _targ_x, float _targ_y) :BASE(what, _tox, _toy)
{
	init_x = _tox;
	init_y = _toy;

	target_x = _targ_x;
	target_y = _targ_y;

	if (_type == ev_arrow_type || _type == gd_arrow_type)nadir = start.y - 150.0f;
	
	if (init_x == target_x)
	{
		vert_dir = true;
		return;
	}
	if (init_y == target_y)
	{
		hor_dir = true;
		return;
	}

	if (_type == ev_arrow_type || _type == gd_arrow_type)slope = (nadir - init_y) / (target_x - init_x);
	else slope = (target_y - init_y) / (target_x - init_x);
	intercept = init_y - slope * init_x;

}

bool dll::SHOTS::Move()
{
	if ((_type == gd_arrow_type || _type == ev_arrow_type) && start.y <= nadir && dir != dirs::down_left
		&& dir != dirs::down_right)
	{
		init_x = start.x;
		init_y = start.y;

		if (init_x > target_x)dir = dirs::down_left;
		else if (init_x < target_x) dir = dirs::down_right;
		else dir = dirs::down_left;

		if (init_x == target_x) vert_dir = true;
		else vert_dir = false;

		if (init_y == target_y) hor_dir = true;
		else hor_dir = false;

		if (!vert_dir)
		{
			slope = (target_y - init_y) / (target_x - init_x);
			intercept = init_y - slope * init_x;
		}
	}

	if (vert_dir)
	{
		if (target_y < init_y)
		{
			if (start.y - 1.0f > sky)
			{
				--start.y;
				SetEdges();
				return true;
			}

			return false;
		}
		else if (target_y > init_y)
		{
			if (end.y + 1.0f < ground)
			{
				++start.y;
				SetEdges();
				return true;
			}
			return false;
		}
	}
	if (hor_dir)
	{
		if (target_x < init_x)
		{
			if (start.x - 1.0f > 0)
			{
				--start.x;
				SetEdges();
				return true;
			}

			return false;

		}
		else if (target_x > init_x)
		{
			if (end.x + 1.0f < scr_width)
			{
				++start.x;
				SetEdges();
				return true;
			}
			return false;
		}
	}

	if (target_x < init_x)
	{
		if (start.x - 1.0f > 0)
		{
			--start.x;
			start.y = start.x * slope + intercept;
			SetEdges();
			return true;
		}
		return false;
	}
	else if (target_x > init_x)
	{
		if (end.x + 1.0f < scr_width)
		{
			++start.x;
			start.y = start.x * slope + intercept;
			SetEdges();
			return true;
		}
		return false;
	}

	return false;
}
void dll::SHOTS::Release()
{
	delete this;
}

///////////////////////////////////////////

// FACTORIES ******************************

HELPSRV_API dll::Creatures dll::CreatureFactory(uint8_t which, float sx, float sy)
{
	Creatures ret{ nullptr };

	if (which == ev_archer_type || which == ev_coyote_type || which == ev_dragon_type || which == ev_hydra_type
		|| which == ev_warrior_type || which == ev_mage_type || which == ev_minotaur_type) ret = new dll::EVILS(which, sx, sy);
	else if (which == gd_archer_type || which == gd_unicorn_type || which == gd_dragon_type || which == gd_hydra_type
		|| which == gd_warrior_type || which == gd_horse_type || which == gd_minotaur_type) ret = new dll::HERO(which, sx, sy);

	return ret;
}
HELPSRV_API dll::Shot dll::ShotFactory(uint8_t which, float sx, float sy, float to_x, float to_y)
{
	Shot ret{ nullptr };

	ret = new SHOTS(which, sx, sy, to_x, to_y);

	return ret;
}