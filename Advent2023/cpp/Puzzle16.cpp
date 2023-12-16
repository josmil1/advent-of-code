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

struct LightMap
{
	Grid     grid;
	uint32_t width;
	uint32_t height;
};

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

void print(LightMap &lightmap)
{
	for (int j = 0; j < lightmap.height; j++)
	{
		for (int i = 0; i < lightmap.width; i++)
		{
			if (lightmap.grid.count(key(i, j)))
			{
				switch (lightmap.grid[key(i, j)].type)
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
}

void print_visited(LightMap &lightmap)
{
	for (int j = 0; j < lightmap.height; j++)
	{
		for (int i = 0; i < lightmap.width; i++)
		{
			if (lightmap.grid.count(key(i, j)))
			{
				auto &s = lightmap.grid[key(i, j)];
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
}

// Note lightmap here is passed by copy
uint32_t get_energised_count(LightMap lightmap, LightBeam &start_beam)
{
	std::vector<LightBeam> beams;
	beams.push_back({start_beam.dir, start_beam.start});

	while (!beams.empty())
	{
		auto beam = beams.back();
		beams.pop_back();

		Pos cur = beam.start;

		while (cur.x >= 0 && cur.x < lightmap.width && cur.y >= 0 && cur.y < lightmap.height)
		{
			if (!lightmap.grid.count(key(cur.x, cur.y)))
			{
				lightmap.grid[key(cur.x, cur.y)] = {SpaceType::Empty};
			}

			auto &space = lightmap.grid[key(cur.x, cur.y)];

			// Break early if already done this path before
			bool break_early{false};
			switch (beam.dir)
			{
				case Direction::Right:
				{
					break_early = space.visited_right;

					space.visited_right = true;
					break;
				}
				case Direction::Down:
				{
					break_early = space.visited_down;

					space.visited_down = true;
					break;
				}
				case Direction::Left:
				{
					break_early = space.visited_left;

					space.visited_left = true;
					break;
				}
				case Direction::Up:
				{
					break_early = space.visited_up;

					space.visited_up = true;
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
	}

	auto visited_count = std::accumulate(lightmap.grid.begin(), lightmap.grid.end(), 0,
	                                     [](uint64_t sum, const Grid::value_type &s) { return (s.second.visited_left || s.second.visited_right || s.second.visited_down || s.second.visited_up) ? sum + 1 : sum; });

	return visited_count;
}

LightMap get_grid(std::ifstream &in_file)
{
	std::string line;
	LightMap    lightmap{};

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == lightmap.width)
			{
				lightmap.width = line.size();
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
						lightmap.grid[key(i, lightmap.height)] = {SpaceType::MirrorBack};
						break;
					}
					case '/':
					{
						lightmap.grid[key(i, lightmap.height)] = {SpaceType::MirrorForward};
						break;
					}
					case '-':
					{
						lightmap.grid[key(i, lightmap.height)] = {SpaceType::SplitterH};
						break;
					}
					case '|':
					{
						lightmap.grid[key(i, lightmap.height)] = {SpaceType::SplitterV};
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}
			}

			lightmap.height++;
		}
	}

	return lightmap;
}

uint64_t puzzle_16_1(std::ifstream &in_file)
{
	uint32_t max_energy{0};

	// Parse map
	auto lightmap = get_grid(in_file);

	std::vector<LightBeam> start_beams;
	start_beams.push_back({Direction::Right, {0, 0}});

	// Single beam right from top left
	while (!start_beams.empty())
	{
		auto sbeam = start_beams.back();
		start_beams.pop_back();

		max_energy = get_energised_count(lightmap, sbeam);
	}

	return max_energy;
}

uint64_t puzzle_16_2(std::ifstream &in_file)
{
	uint32_t max_energy{0};

	auto lightmap = get_grid(in_file);

	std::vector<LightBeam> start_beams;

	for (int i = 0; i < lightmap.width; i++)
	{
		// Start at top row
		start_beams.push_back({Direction::Down, {i, 0}});
	}
	for (int i = 0; i < lightmap.width; i++)
	{
		// Start at bottom row
		start_beams.push_back({Direction::Up, {i, static_cast<int>(lightmap.height) - 1}});
	}
	for (int j = 0; j < lightmap.height; j++)
	{
		// Start at left column
		start_beams.push_back({Direction::Right, {0, j}});
	}
	for (int j = 0; j < lightmap.height; j++)
	{
		// Start at right column
		start_beams.push_back({Direction::Left, {static_cast<int>(lightmap.width) - 1, j}});
	}

	while (!start_beams.empty())
	{
		auto sbeam = start_beams.back();
		start_beams.pop_back();

		max_energy = std::max(max_energy, get_energised_count(lightmap, sbeam));
	}

	return max_energy;
}
}        // namespace

uint64_t puzzle_16(std::ifstream &in_file)
{
	return puzzle_16_2(in_file);
}
