#include "Puzzles.hpp"

#include <random>
#include <set>

namespace
{
struct Connection
{
	std::string a;
	std::string b;
};

uint64_t puzzle_25_1(std::ifstream &in_file)
{
	std::set<std::string>   components;
	std::vector<Connection> connections;

	std::unordered_map<std::string, bool> connections_helper;

	std::string line;
	std::string str;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);

			std::string comp_1{}, helper{};

			while (ss >> str)
			{
				// comp_1: comp_2 comp_3 ...
				if (str.size() > 3)
				{
					str = str.substr(0, 3);

					comp_1 = str;
				}

				// Add component
				if (!components.count(str))
				{
					components.insert(str);
				}

				if (!comp_1.empty() && comp_1 != str)
				{
					// Add connection, avoiding duplicates
					helper = comp_1 + str;
					if (!connections_helper.count(helper))
					{
						connections_helper[helper] = true;

						helper = str + comp_1;
						if (!connections_helper.count(helper))
						{
							connections_helper[helper] = true;

							connections.push_back({comp_1, str});
						}
					}
				}
			}
		}
	}

	// Random number generator
	std::random_device                                       dev;
	std::mt19937                                             rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> dist(0, connections.size() - 1);

	std::vector<std::set<std::string>> groups;

	bool cut_three{false};
	while (!cut_three)
	{
		// Initialize 1 group per component
		groups.clear();
		for (auto &comp : components)
		{
			groups.push_back({comp});
		}

		// Randomly contract connections until all components are in one of exactly 2 groups
		std::set<std::string> group_1, group_2;
		while (groups.size() > 2)
		{
			auto rand_i = dist(rng);

			auto it_1 = std::find_if(groups.begin(), groups.end(),
			                         [&](std::set<std::string> &group) { return group.count(connections[rand_i].a); });
			auto it_2 = std::find_if(groups.begin(), groups.end(),
			                         [&](std::set<std::string> &group) { return group.count(connections[rand_i].b); });

			if (it_1 == it_2)
			{
				// Both components are in the same group
				continue;
			}

			// Merge the groups
			it_1->insert(it_2->begin(), it_2->end());
			groups.erase(it_2);
		}

		// Check that these two groups were the result of cutting exactly 3 wires
		uint32_t cut_wires{0};
		for (int i = 0; i < connections.size(); ++i)
		{
			auto it_1 = std::find_if(groups.begin(), groups.end(),
			                         [&](std::set<std::string> &group) { return group.count(connections[i].a); });
			auto it_2 = std::find_if(groups.begin(), groups.end(),
			                         [&](std::set<std::string> &group) { return group.count(connections[i].b); });

			if (it_1 != it_2)
			{
				// Components are now in different groups, this connection (wire) was cut
				cut_wires++;
			}
		}

		cut_three = (3 == cut_wires);
	}

	// Return product of the 2 group sizes
	return std::accumulate(groups.begin(), groups.end(), 1,
	                       [](uint64_t p, const std::vector<std::set<std::string>>::value_type &g) { return p * g.size(); });
}

uint64_t puzzle_25_2(std::ifstream &in_file)
{
	// No Part 2, Merry Christmas!
	return 0;
}
}        // namespace

uint64_t puzzle_25(std::ifstream &in_file)
{
	return puzzle_25_1(in_file);
}
