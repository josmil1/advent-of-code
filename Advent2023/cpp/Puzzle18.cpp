#include "Puzzles.hpp"

namespace
{
struct Pos
{
	int64_t x{0};
	int64_t y{0};

	bool operator==(const Pos &other) const
	{
		return x == other.x &&
		       y == other.y;
	}
};

enum Direction : uint16_t
{
	Right = 0,
	Down  = 1,
	Left  = 2,
	Up    = 3,
};

struct Hole
{
	std::string color;
	Direction   dir;
	bool        north_connection{false};
};

struct pair_hash
{
	template <class T1, class T2>
	std::size_t operator()(const std::pair<T1, T2> &p) const
	{
		auto h1 = std::hash<T1>{}(p.first);
		auto h2 = std::hash<T2>{}(p.second);

		return h1 ^ h2;
	}
};

using Pool = std::unordered_map<std::pair<int, int>, Hole, pair_hash>;

void draw_hole(Hole *hole = nullptr, uint64_t up_count = 0)
{
	if (hole)
	{
		if (hole->north_connection)
		{
			// Holes with a north connection,
			// useful to check even-odd rule
			std::cout << "O";
		}
		else
		{
			switch (hole->dir)
			{
				case Direction::Up:
				{
					std::cout << "^";
					break;
				}
				case Direction::Down:
				{
					std::cout << "|";
					break;
				}
				case Direction::Right:
				{
					std::cout << ">";
					break;
				}
				case Direction::Left:
				{
					std::cout << "<";
					break;
				}
				default:
				{
					std::cout << "Error" << std::endl;
				}
			}
		}
	}
	else
	{
		if (up_count % 2 != 0)
		{
			// Enclosed
			std::cout << '#';
		}
		else
		{
			// No hole
			std::cout << '.';
		}
	}
}

uint64_t puzzle_18_1(std::ifstream &in_file)
{
	std::string line;
	std::string str;
	char        cdir;
	uint32_t    num;

	// Parse pool
	Pool      pool;
	Pos       cur{};
	Direction cur_dir{Direction::Right};

	// Account for moving into the negative axis
	int64_t min_x{std::numeric_limits<int64_t>::max()};
	int64_t min_y{std::numeric_limits<int64_t>::max()};
	int64_t max_x{0};
	int64_t max_y{0};

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);

			ss >> cdir >> num >> str;

			for (int i = 0; i < num; i++)
			{
				bool  north_connection = false;
				auto &prev_node        = pool[std::make_pair(cur.x, cur.y)];

				switch (cdir)
				{
					case 'R':
					{
						prev_node.north_connection = prev_node.dir == Direction::Up ? false : prev_node.north_connection;

						cur_dir = Direction::Right;
						cur.x++;
						break;
					}
					case 'L':
					{
						prev_node.north_connection = prev_node.dir == Direction::Up ? false : prev_node.north_connection;

						cur_dir = Direction::Left;
						cur.x--;
						break;
					}
					case 'D':
					{
						north_connection = true;

						cur_dir = Direction::Down;
						cur.y++;
						break;
					}
					case 'U':
					{
						north_connection           = true;
						prev_node.north_connection = true;

						cur_dir = Direction::Up;
						cur.y--;
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}

				pool[std::make_pair(cur.x, cur.y)] = {str, cur_dir, north_connection};

				max_x = std::max(max_x, cur.x);
				max_y = std::max(max_y, cur.y);

				min_x = std::min(min_x, cur.x);
				min_y = std::min(min_y, cur.y);
			}
		}
	}

	// Fix last node
	auto &last_node = pool[std::make_pair(cur.x, cur.y)];
	if (last_node.dir == Direction::Up)
	{
		last_node.north_connection = false;
	}

	// Draw the pool and calculate volume
	std::uint64_t hole_count{0};
	for (int64_t j = min_y; j <= max_y; j++)
	{
		int up_count{0};

		for (int64_t i = min_x; i <= max_x; i++)
		{
			Hole *hole{nullptr};
			auto  coords = std::make_pair(i, j);
			if (pool.count(coords))
			{
				hole_count++;

				hole = &pool[coords];
				if (hole->north_connection)
				{
					// Perimeter
					up_count++;
				}
			}
			else
			{
				if (up_count % 2 != 0)
				{
					// Area
					hole_count++;
				}
			}

			draw_hole(hole, up_count);
		}

		std::cout << " " << hole_count << std::endl;
	}

	return hole_count;
}

struct PoolPoint
{
	Direction dir{};
	uint64_t  distance{0};
};

using PoolMap = std::vector<PoolPoint>;

uint64_t get_volume(PoolMap &poolmap)
{
	// Save collection of vertices from
	// list of direction and distance
	std::vector<Pos> vertices;

	Pos cur{0, 0};
	vertices.push_back(cur);
	for (auto &p : poolmap)
	{
		switch (p.dir)
		{
			case Direction::Right:
			{
				cur.x += p.distance;
				break;
			}
			case Direction::Left:
			{
				cur.x -= p.distance;
				break;
			}
			case Direction::Up:
			{
				cur.y -= p.distance;
				break;
			}
			case Direction::Down:
			{
				cur.y += p.distance;
				break;
			}
			default:
			{
				std::cout << "Error" << std::endl;
			}
		}

		vertices.push_back(cur);
	}

	// Calculate volume as sum of inside and edge holes
	int64_t area{0}, perimeter{0};
	for (uint64_t i = 1; i < vertices.size(); i++)
	{
		auto v_1 = vertices[i - 1];
		auto v_2 = vertices[i];

		// Shoelace formula: 2A = Sum(determinants)
		area += (v_2.x * v_1.y) - (v_1.x * v_2.y);

		perimeter += std::abs(v_2.x - v_1.x) + std::abs(v_2.y - v_1.y);
	}
	area = std::abs(area) / 2;

	// Pick's theorem: A = inside + perimeter / 2 - 1
	auto inside = area + 1 - (perimeter / 2);

	return inside + perimeter;
}

uint64_t puzzle_18_2(std::ifstream &in_file)
{
	std::string line;
	std::string str;
	Direction   direction;
	uint32_t    num, distance;

	// Parse pool as a collection of
	// direction and distance
	PoolMap pool;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);

			ss >> str >> num >> str;

			// Distance given as hex
			distance = std::stoi(str.substr(2, 5), 0, 16);

			// Direction given as digit
			switch (str.substr(7, 1)[0] - '0')
			{
				case 0:
				{
					direction = Direction::Right;
					break;
				}
				case 1:
				{
					direction = Direction::Down;
					break;
				}
				case 2:
				{
					direction = Direction::Left;
					break;
				}
				case 3:
				{
					direction = Direction::Up;
					break;
				}
				default:
				{
					std::cout << "Error" << std::endl;
				}
			}

			pool.push_back({direction, distance});
		}
	}

	return get_volume(pool);
}
}        // namespace

uint64_t puzzle_18(std::ifstream &in_file)
{
	return puzzle_18_2(in_file);
}
