#include "Puzzles.hpp"

#include <deque>

namespace
{
struct Pos
{
	int32_t x{0};
	int32_t y{0};

	Pos operator+(const Pos &other) const
	{
		return {x + other.x,
		        y + other.y};
	}

	bool operator==(const Pos &other) const
	{
		return x == other.x &&
		       y == other.y;
	}
};

using Grid = std::unordered_map<uint64_t, uint32_t>;

struct Garden
{
	Grid    grid;
	int32_t width;
	int32_t height;
};

inline uint64_t key(int32_t x, int32_t y)
{
	return static_cast<int64_t>(x) << 32 | y;
}

inline uint64_t key(Pos &pos)
{
	return key(pos.x, pos.y);
}

struct PlotState
{
	Pos pos{};
	int distance{0};

	bool operator==(const PlotState &other) const
	{
		return pos == other.pos &&
		       distance == other.distance;
	}
};

struct PlotStateHasher
{
	std::size_t operator()(const PlotState &c) const
	{
		using std::hash;
		using std::size_t;
		using std::string;

		return ((hash<string>()(std::to_string(key(c.pos.x, c.pos.y))) ^ (hash<int>()(c.distance) << 1)) >> 1);
	}
};

uint64_t puzzle_21_1(std::ifstream &in_file)
{
	Garden garden{};

	Pos start{};

	std::string line;

	// Parse garden
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == garden.width)
			{
				garden.width = line.size();
			}

			for (int i = 0; i < garden.width; i++)
			{
				if ('#' != line[i])
				{
					garden.grid[key(i, garden.height)] = 0;

					if ('S' == line[i])
					{
						start = {i, garden.height};
					}
				}
			}

			garden.height++;
		}
	}

	// Print
	for (int j = 0; j < garden.height; j++)
	{
		for (int i = 0; i < garden.width; i++)
		{
			if (garden.grid.count(key(i, j)))
			{
				std::cout << '.';
			}
			else
			{
				std::cout << '#';
			}
		}

		std::cout << std::endl;
	}

	constexpr int STEPS = 64;

	std::vector<Pos> final_positions;

	std::unordered_map<PlotState, bool, PlotStateHasher> visited;

	std::deque<std::pair<Pos, int>> positions;
	positions.push_back({start, 0});

	while (!positions.empty())
	{
		auto cur      = positions.front().first;
		auto distance = positions.front().second;
		positions.pop_front();

		if (visited.count({cur, distance}) && visited[{cur, distance}])
		{
			continue;
		}

		visited[{cur, distance}] = true;

		if (distance == STEPS)
		{
			final_positions.push_back(cur);
		}
		else
		{
			for (int i = -1; i <= 1; i++)
			{
				for (int j = -1; j <= 1; j++)
				{
					if ((i == 0 && j == 0) || (std::abs(i) == std::abs(j)))
					{
						continue;
					}

					auto new_pos = cur + Pos{i, j};

					if (new_pos.x >= 0 && new_pos.x < garden.width &&
					    new_pos.y >= 0 && new_pos.y < garden.height &&
					    garden.grid.count(key(new_pos)))
					{
						// Visit neighbors
						positions.push_back({new_pos, distance + 1});
					}
				}
			}
		}
	}

	/*
	   for (int i = -STEPS; i <= STEPS; i++)
	   {
	       for (int j = -STEPS; j <= STEPS; j++)
	       {
	           auto check = start + Pos{i, j};

	           if ((check.x + check.y) % 2 != 0)
	           {
	               continue;
	           }

	           if (check.x >= 0 && check.x < garden.width && check.y >= 0 && check.y <= garden.height)
	           {
	               if (garden.grid.count(key(check)))
	               {
	                   positions.push_back(check);
	               }
	           }
	       }
	   }
	*/

	/*
	positions.push_back({start});
	for (int s = 0; s < STEPS; s++)
	{
	    std::vector<Pos> final_pos;
	    while (!positions.empty())
	    {
	        auto cur = positions.back();

	        Pos up = cur + Pos{0, -1};
	        if (up.x >= 0 && up.x < garden.width && up.y >= 0 && up.y <= garden.height)
	        {
	            if (garden.grid.count(key(up)))
	            {
	                final_pos.push_back(up);
	            }
	        }

	        Pos down = cur + Pos{0, 1};
	        if (down.x >= 0 && down.x < garden.width && down.y >= 0 && down.y <= garden.height)
	        {
	            if (garden.grid.count(key(down)))
	            {
	                final_pos.push_back(down);
	            }
	        }

	        Pos right = cur + Pos{1, 0};
	        if (right.x >= 0 && right.x < garden.width && right.y >= 0 && right.y <= garden.height)
	        {
	            if (garden.grid.count(key(right)))
	            {
	                final_pos.push_back(right);
	            }
	        }

	        Pos left = cur + Pos{-1, 0};
	        if (left.x >= 0 && left.x < garden.width && left.y >= 0 && left.y <= garden.height)
	        {
	            if (garden.grid.count(key(left)))
	            {
	                final_pos.push_back(left);
	            }
	        }

	        positions.pop_back();
	    }

	    positions = final_pos;
	}
	*/

	/*
	   positions.push_back({start});
	   for (int s = 0; s < STEPS; s++)
	   {
	       std::vector<Pos> final_pos;

	       while (!positions.empty())
	       {
	           auto cur = positions.back();
	           positions.pop_back();

	           for (int i = -1; i <= 1; i++)
	           {
	               for (int j = -1; j <= 1; j++)
	               {
	                   if (i == 0 && j == 0 || std::abs(i) + std::abs(j) != 1)
	                   {
	                       continue;
	                   }

	                   auto check = cur + Pos{i, j};

	                   if (check.x >= 0 && check.x < garden.width && check.y >= 0 && check.y <= garden.height)
	                   {
	                       if (garden.grid.count(key(check)))
	                       {
	                           final_pos.push_back(check);
	                       }
	                   }
	               }
	           }
	       }

	       positions = final_pos;
	   }
	*/

	uint64_t ans = 0;
	for (int j = 0; j < garden.height; j++)
	{
		for (int i = 0; i < garden.width; i++)
		{
			if (garden.grid.count(key(i, j)))
			{
				if (std::find(final_positions.begin(), final_positions.end(), Pos{i, j}) != final_positions.end())
				{
					std::cout << 'O';
					ans++;
				}
				else
				{
					std::cout << '.';
				}
			}
			else
			{
				std::cout << '#';
			}
		}

		std::cout << std::endl;
	}

	return ans;
}

uint64_t puzzle_21_2(std::ifstream &in_file)
{
	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::cout << line << std::endl;
		}
	}

	return 0;
}
}        // namespace

uint64_t puzzle_21(std::ifstream &in_file)
{
	return puzzle_21_1(in_file);
}
