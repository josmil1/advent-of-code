#include "Puzzles.hpp"

namespace
{
enum SpaceType : uint16_t
{
	Empty         = 0,
	MirrorBack    = 1,
	MirrorForward = 2,
	SplitterH     = 3,
	SplitterV     = 4,
};

struct Space
{
	SpaceType type{SpaceType::Empty};
	bool      visited_right{false};
	bool      visited_left{false};
	bool      visited_up{false};
	bool      visited_down{false};
};

using Grid = std::unordered_map<uint64_t, Space>;

inline uint64_t key(uint32_t x, uint32_t y)
{
	return static_cast<uint64_t>(x) << 32 | y;
}

enum Direction : uint16_t
{
	Right = 0,
	Down  = 1,
	Left  = 2,
	Up    = 3,
};

struct Pos
{
	int32_t x{0};
	int32_t y{0};
};

struct LightBeam
{
	Direction dir{Direction::Right};
	Pos       start{0, 0};
};

uint64_t puzzle_16_1(std::ifstream &in_file)
{
	std::string line;

	uint32_t width{0}, height{0};

	Grid grid;

	// Parse map
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == width)
			{
				width = line.size();
			}

			for (uint32_t i = 0; i < line.size(); i++)
			{
				switch (line[i])
				{
					case '.':
					{
						continue;
						break;
					}
					case '\\':
					{
						grid[key(i, height)] = {SpaceType::MirrorBack};
						break;
					}
					case '/':
					{
						grid[key(i, height)] = {SpaceType::MirrorForward};
						break;
					}
					case '-':
					{
						grid[key(i, height)] = {SpaceType::SplitterH};
						break;
					}
					case '|':
					{
						grid[key(i, height)] = {SpaceType::SplitterV};
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}
			}

			height++;
		}
	}

	// Print map
	/*
	for (int j = 0; j < height; j++)
	{
	    for (int i = 0; i < width; i++)
	    {
	        if (grid.count(key(i, j)))
	        {
	            switch (grid[key(i, j)].type)
	            {
	                case SpaceType::MirrorBack:
	                {
	                    std::cout << '\\';
	                    break;
	                }
	                case SpaceType::MirrorForward:
	                {
	                    std::cout << '/';
	                    break;
	                }
	                case SpaceType::SplitterH:
	                {
	                    std::cout << '-';
	                    break;
	                }
	                case SpaceType::SplitterV:
	                {
	                    std::cout << '|';
	                    break;
	                }
	                default:
	                {
	                    std::cout << "Error" << std::endl;
	                }
	            }
	        }
	        else
	        {
	            std::cout << '.';
	        }
	    }
	    std::cout << std::endl;
	}
	*/

	// Path trace
	uint32_t max_energy{0};

	std::vector<LightBeam> start_beams;

	for (int i = 0; i < width; i++)
	{
		start_beams.push_back({Direction::Down, {i, 0}});
	}
	for (int i = 0; i < width; i++)
	{
		start_beams.push_back({Direction::Up, {i, static_cast<int>(height) - 1}});
	}
	for (int j = 0; j < height; j++)
	{
		start_beams.push_back({Direction::Right, {0, j}});
	}
	for (int j = 0; j < height; j++)
	{
		start_beams.push_back({Direction::Left, {static_cast<int>(width) - 1, j}});
	}

	while (!start_beams.empty())
	{
		auto sbeam = start_beams.back();
		start_beams.pop_back();

		auto this_grid = grid;

		std::vector<LightBeam> beams;
		beams.push_back({sbeam.dir, sbeam.start});

		while (!beams.empty())
		{
			auto beam = beams.back();
			beams.pop_back();

			Pos cur = beam.start;

			while (cur.x >= 0 && cur.x < width && cur.y >= 0 && cur.y < height)
			{
				if (!this_grid.count(key(cur.x, cur.y)))
				{
					this_grid[key(cur.x, cur.y)] = {SpaceType::Empty};
				}

				auto &space = this_grid[key(cur.x, cur.y)];

				// Break early if already done this path before
				bool break_early{false};
				switch (beam.dir)
				{
					case Direction::Right:
					{
						break_early = space.visited_right;
						break;
					}
					case Direction::Down:
					{
						break_early = space.visited_down;
						break;
					}
					case Direction::Left:
					{
						break_early = space.visited_left;
						break;
					}
					case Direction::Up:
					{
						break_early = space.visited_up;
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}

				if (break_early)
				{
					break;
				}

				switch (beam.dir)
				{
					case Direction::Right:
					{
						space.visited_right = true;
						break;
					}
					case Direction::Down:
					{
						space.visited_down = true;
						break;
					}
					case Direction::Left:
					{
						space.visited_left = true;
						break;
					}
					case Direction::Up:
					{
						space.visited_up = true;
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}

				// Process space
				switch (space.type)
				{
					case SpaceType::Empty:
					{
						break;
					}
					case SpaceType::MirrorBack:
					{
						switch (beam.dir)
						{
							case Direction::Right:
							{
								beam.dir = Direction::Down;
								break;
							}
							case Direction::Down:
							{
								beam.dir = Direction::Right;
								break;
							}
							case Direction::Left:
							{
								beam.dir = Direction::Up;
								break;
							}
							case Direction::Up:
							{
								beam.dir = Direction::Left;
								break;
							}
							default:
							{
								std::cout << "Error" << std::endl;
							}
						}
						break;
					}
					case SpaceType::MirrorForward:
					{
						switch (beam.dir)
						{
							case Direction::Right:
							{
								beam.dir = Direction::Up;
								break;
							}
							case Direction::Down:
							{
								beam.dir = Direction::Left;
								break;
							}
							case Direction::Left:
							{
								beam.dir = Direction::Down;
								break;
							}
							case Direction::Up:
							{
								beam.dir = Direction::Right;
								break;
							}
							default:
							{
								std::cout << "Error" << std::endl;
							}
						}
						break;
					}
					case SpaceType::SplitterH:
					{
						switch (beam.dir)
						{
							case Direction::Right:
							case Direction::Left:
							{
								break;
							}
							case Direction::Up:
							{
								beam.dir = Direction::Right;

								beams.push_back({Direction::Left, {cur.x - 1, cur.y}});
								break;
							}
							case Direction::Down:
							{
								beam.dir = Direction::Right;

								beams.push_back({Direction::Left, {cur.x - 1, cur.y}});
								break;
							}
							default:
							{
								std::cout << "Error" << std::endl;
							}
						}
						break;
					}
					case SpaceType::SplitterV:
					{
						switch (beam.dir)
						{
							case Direction::Up:
							case Direction::Down:
							{
								break;
							}
							case Direction::Right:
							{
								beam.dir = Direction::Up;

								beams.push_back({Direction::Down, {cur.x, cur.y + 1}});
								break;
							}
							case Direction::Left:
							{
								beam.dir = Direction::Up;

								beams.push_back({Direction::Down, {cur.x, cur.y + 1}});
								break;
							}
							default:
							{
								std::cout << "Error" << std::endl;
							}
						}
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}

				// Move beam
				switch (beam.dir)
				{
					case Direction::Right:
					{
						cur.x++;
						break;
					}
					case Direction::Down:
					{
						cur.y++;
						break;
					}
					case Direction::Left:
					{
						cur.x--;
						break;
					}
					case Direction::Up:
					{
						cur.y--;
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}
			}

			uint32_t visited_count{0};

			for (auto &s : this_grid)
			{
				if (s.second.visited_left || s.second.visited_right || s.second.visited_down || s.second.visited_up)
				{
					visited_count++;
				}
			}

			// std::cout << visited_count << std::endl;
			max_energy = std::max(max_energy, visited_count);

			/*
			for (int j = 0; j < height; j++)
			{
			    for (int i = 0; i < width; i++)
			    {
			        if (this_grid.count(key(i, j)))
			        {
			            auto &s = this_grid[key(i, j)];
			            if (s.visited_left || s.visited_right || s.visited_down || s.visited_up)
			            {
			                std::cout << "#";
			            }
			            else
			            {
			                std::cout << ".";
			            }
			        }
			        else
			        {
			            std::cout << ".";
			        }
			    }
			    std::cout << std::endl;
			}
			*/
		}
	}

	return max_energy;
}

uint64_t puzzle_16_2(std::ifstream &in_file)
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

uint64_t puzzle_16(std::ifstream &in_file)
{
	return puzzle_16_1(in_file);
}
