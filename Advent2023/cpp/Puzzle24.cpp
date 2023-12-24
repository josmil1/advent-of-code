#include "Puzzles.hpp"

namespace
{
struct Pos
{
	long double x{0};
	long double y{0};
	long double z{0};

	Pos operator+(const Pos &other) const
	{
		return {x + other.x,
		        y + other.y,
		        z + other.z};
	}
};

struct Line
{
	Pos u{};
	Pos v{};
	Pos diff{};
};

Pos get_intersection(Line a, Line b, bool &valid)
{
	Pos intersection{};

	long double x1 = a.u.x, x2 = a.v.x, x3 = b.u.x, x4 = b.v.x;
	long double y1 = a.u.y, y2 = a.v.y, y3 = b.u.y, y4 = b.v.y;

	auto d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
	// If d is zero, there is no intersection
	if (d == 0)
	{
		std::cout << "No intersection" << std::endl;
		valid = false;
		return intersection;
	}

	// Get the x and y
	long double pre = (x1 * y2 - y1 * x2), post = (x3 * y4 - y3 * x4);
	long double x = (pre * (x3 - x4) - (x1 - x2) * post) / d;
	long double y = (pre * (y3 - y4) - (y1 - y2) * post) / d;

	// Check if the x and y coordinates are within both lines
	/*
	if (x < std::min(x1, x2) || x > std::max(x1, x2) ||
	    x < std::min(x3, x4) || x > std::max(x3, x4))
	{
	    std::cout << "Out of bounds at x";
	    return intersection;
	}
	if (y < std::min(y1, y2) || y > std::max(y1, y2) ||
	    y < std::min(y3, y4) || y > std::max(y3, y4))
	{
	    std::cout << "Out of bounds at y";
	    return intersection;
	}
	*/

	// Return the point of intersection
	intersection.x = x;
	intersection.y = y;
	return intersection;
}

uint64_t puzzle_24_1(std::ifstream &in_file)
{
	std::vector<Line> hailstones;

	std::string line;
	char        c;

	constexpr int64_t MIN = 200000000000000;
	constexpr int64_t MAX = 400000000000000;

	// constexpr int64_t MIN = 7;
	// constexpr int64_t MAX = 27;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			Line hailstone;

			std::stringstream ss(line);

			ss >> hailstone.u.x >> c >> hailstone.u.y >> c >> hailstone.u.z >> c;

			ss >> hailstone.diff.x >> c;

			if (hailstone.u.x < MIN && hailstone.diff.x < 0 ||
			    hailstone.u.x > MAX && hailstone.diff.x > 0)
			{
				continue;
			}
			hailstone.v.x = hailstone.u.x + hailstone.diff.x;

			ss >> hailstone.diff.y >> c;

			if (hailstone.u.y < MIN && hailstone.diff.y < 0 ||
			    hailstone.u.y > MAX && hailstone.diff.y > 0)
			{
				continue;
			}
			hailstone.v.y = hailstone.u.y + hailstone.diff.y;

			ss >> hailstone.diff.z;

			if (hailstone.u.z < MIN && hailstone.diff.z < 0 ||
			    hailstone.u.z > MAX && hailstone.diff.z > 0)
			{
				// continue;
			}
			hailstone.v.z = hailstone.u.z + hailstone.diff.z;

			hailstones.push_back(hailstone);
		}
	}

	uint64_t intersection_count{0};
	for (int i = 0; i < hailstones.size(); i++)
	{
		for (int j = i + 1; j < hailstones.size(); j++)
		{
			auto &a = hailstones[i];
			auto &b = hailstones[j];

			// std::cout << a.u.x << ":" << a.u.y << ":" << a.u.z << std::endl;
			// std::cout << b.u.x << ":" << b.u.y << ":" << b.u.z << std::endl;

			a.v = a.u + a.diff;
			b.v = b.u + b.diff;

			auto temp_a = a;
			auto temp_b = b;

			auto divisor = std::max(a.u.x, b.u.x);

			temp_a.u.x = a.u.x / divisor;
			temp_a.u.y = a.u.y / divisor;
			temp_a.u.z = a.u.z / divisor;

			temp_a.v.x = a.v.x / divisor;
			temp_a.v.y = a.v.y / divisor;
			temp_a.v.z = a.v.z / divisor;

			temp_b.u.x = b.u.x / divisor;
			temp_b.u.y = b.u.y / divisor;
			temp_b.u.z = b.u.z / divisor;

			temp_b.v.x = b.v.x / divisor;
			temp_b.v.y = b.v.y / divisor;
			temp_b.v.z = b.v.z / divisor;

			// std::cout << temp_a.u.x << ":" << temp_a.u.y << ":" << temp_a.u.z << std::endl;
			// std::cout << temp_b.u.x << ":" << temp_b.u.y << ":" << temp_b.u.z << std::endl;

			bool valid        = true;
			auto intersection = get_intersection(temp_a, temp_b, valid);
			intersection.x *= divisor;
			intersection.y *= divisor;

			// std::cout << "Intersection " << intersection.x << ":" << intersection.y << std::endl;

			if (valid &&
			    intersection.x >= MIN && intersection.x <= MAX &&
			    intersection.y >= MIN && intersection.y <= MAX)
			{
				if (((a.u.x < a.v.x && intersection.x >= a.u.x) ||
				     (a.u.x > a.v.x && intersection.x <= a.u.x)) &&
				    ((b.u.x < b.v.x && intersection.x >= b.u.x) ||
				     (b.u.x > b.v.x && intersection.x <= b.u.x)) &&
				    ((a.u.y < a.v.y && intersection.y >= a.u.y) ||
				     (a.u.y > a.v.y && intersection.y <= a.u.y)) &&
				    ((b.u.y < b.v.y && intersection.y >= b.u.y) ||
				     (b.u.y > b.v.y && intersection.y <= b.u.y)))
				{
					// std::cout << "intersection at " << intersection.x << ", " << intersection.y << std::endl;
					intersection_count++;
				}
			}

			// std::cout << std::endl;
		}
	}

	// 11577 too low
	// 10918 too low
	return intersection_count;
}

uint64_t puzzle_24_2(std::ifstream &in_file)
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

uint64_t puzzle_24(std::ifstream &in_file)
{
	return puzzle_24_1(in_file);
}
