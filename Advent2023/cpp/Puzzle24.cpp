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

bool get_intersection(Line a, Line b, Pos &intersection)
{
	bool found_intersection = true;

	long double x_1 = a.u.x, x_2 = a.v.x, x_3 = b.u.x, x_4 = b.v.x;
	long double y_1 = a.u.y, y_2 = a.v.y, y_3 = b.u.y, y_4 = b.v.y;

	auto d = (x_1 - x_2) * (y_3 - y_4) - (y_1 - y_2) * (x_3 - x_4);

	if (d != 0)
	{
		long double pre  = (x_1 * y_2 - y_1 * x_2);
		long double post = (x_3 * y_4 - y_3 * x_4);

		intersection.x = (pre * (x_3 - x_4) - (x_1 - x_2) * post) / d;
		intersection.y = (pre * (y_3 - y_4) - (y_1 - y_2) * post) / d;
	}
	else
	{
		found_intersection = false;
	}

	return found_intersection;
}

uint64_t puzzle_24_1(std::ifstream &in_file)
{
	std::vector<Line> hailstones;

	constexpr bool example_input = true;

	constexpr int64_t MIN = example_input ? 7 : 200000000000000;
	constexpr int64_t MAX = example_input ? 27 : 400000000000000;

	// Parse hailstones
	std::string line;
	char        c;
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
				continue;
			}
			hailstone.v.z = hailstone.u.z + hailstone.diff.z;

			hailstones.push_back(hailstone);
		}
	}

	// Count intersections between every pair (ignore z-coordinates)
	uint64_t intersection_count{0};
	for (int i = 0; i < hailstones.size(); i++)
	{
		for (int j = i + 1; j < hailstones.size(); j++)
		{
			auto &a = hailstones[i];
			auto &b = hailstones[j];

			// Scale the coordinates down to avoid overflow
			auto temp_a  = a;
			auto temp_b  = b;
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

			// Find intersection
			Pos  intersection{};
			auto found_intersection = get_intersection(temp_a, temp_b, intersection);

			// Scale intersection coordinates back up
			intersection.x *= divisor;
			intersection.y *= divisor;

			if (found_intersection &&
			    intersection.x >= MIN && intersection.x <= MAX &&
			    intersection.y >= MIN && intersection.y <= MAX)
			{
				// Within bounds
				if (((a.u.x < a.v.x && intersection.x >= a.u.x) ||
				     (a.u.x > a.v.x && intersection.x <= a.u.x)) &&
				    ((b.u.x < b.v.x && intersection.x >= b.u.x) ||
				     (b.u.x > b.v.x && intersection.x <= b.u.x)) &&
				    ((a.u.y < a.v.y && intersection.y >= a.u.y) ||
				     (a.u.y > a.v.y && intersection.y <= a.u.y)) &&
				    ((b.u.y < b.v.y && intersection.y >= b.u.y) ||
				     (b.u.y > b.v.y && intersection.y <= b.u.y)))
				{
					// After the hailstone start position
					intersection_count++;
				}
			}
		}
	}

	return intersection_count;
}

uint64_t puzzle_24_2(std::ifstream &in_file)
{
	// Solved externally with Z3 Theorem Prover
	return 0;
}
}        // namespace

uint64_t puzzle_24(std::ifstream &in_file)
{
	return puzzle_24_1(in_file);
}
