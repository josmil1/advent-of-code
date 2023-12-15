#include "Puzzles.hpp"

namespace
{
uint32_t get_hash(std::string str)
{
	uint32_t hash{0};

	for (auto &c : str)
	{
		// Determine the ASCII code for the current character of the string.
		// Increase the current value by the ASCII code you just determined.
		hash += static_cast<int>(c);

		// Set the current value to itself multiplied by 17.
		hash *= 17;

		// Set the current value to the remainder of dividing itself by 256.
		hash = hash % 256;
	}

	return hash;
}

uint64_t puzzle_15_1(std::ifstream &in_file)
{
	uint64_t sum{0};

	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			std::string       str;

			while (std::getline(ss, str, ','))
			{
				sum += get_hash(str);
			}
		}
	}

	return sum;
}

struct Lens
{
	std::string label;
	uint32_t    focal_length;
};

uint64_t get_focusing_power(std::unordered_map<uint32_t, std::vector<Lens>> &boxes)
{
	uint64_t sum{0};

	for (auto &box : boxes)
	{
		auto &lenses = box.second;
		for (uint32_t i = 0; i < lenses.size(); i++)
		{
			uint64_t power{1};
			// The focusing power of a single lens is the result of multiplying together:
			// 1 + the box number of the lens in question.
			power *= (1 + box.first);
			// The slot number of the lens within the box: 1 for the first lens, 2 for the second lens, and so on.
			power *= (1 + i);
			// The focal length of the lens.
			power *= lenses[i].focal_length;

			sum += power;
		}
	}

	return sum;
}

uint64_t puzzle_15_2(std::ifstream &in_file)
{
	uint64_t sum{0};

	std::string line;

	std::unordered_map<uint32_t, std::vector<Lens>> boxes;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::string str{};
			for (size_t i = 0; i < line.size(); i++)
			{
				switch (line[i])
				{
					case '=':
					{
						// std::cout << "Add " << str << " " << line[i + 1] << " to Box #" << get_hash(str) << std::endl;

						uint32_t focal_length = line[i + 1] - '0';

						auto box = get_hash(str);

						if (boxes.count(box))
						{
							auto it = std::find_if(boxes[box].begin(), boxes[box].end(), [&](Lens const &l) { return l.label == str; });

							if (it != boxes[box].end())
							{
								it->focal_length = focal_length;
							}
							else
							{
								boxes[box].push_back({str, focal_length});
							}
						}
						else
						{
							boxes[box].push_back({str, focal_length});
						}

						break;
					}
					case '-':
					{
						// std::cout << "Remove " << str << " from Box #" << get_hash(str) << std::endl;

						auto box = get_hash(str);

						if (boxes.count(box))
						{
							auto it = std::find_if(boxes[box].begin(), boxes[box].end(), [&](Lens const &l) { return l.label == str; });

							if (it != boxes[box].end())
							{
								boxes[box].erase(it);
							}
						}
						break;
					}
					case ',':
					{
						str.clear();
						break;
					}
					default:
					{
						str += line[i];
						break;
					}
				}
			}
		}
	}

	return get_focusing_power(boxes);
}
}        // namespace

uint64_t puzzle_15(std::ifstream &in_file)
{
	return puzzle_15_2(in_file);
}
