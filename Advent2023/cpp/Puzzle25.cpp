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

	uint32_t safe{0};

	while (!cut_three)
	{
		safe++;
		if (safe > 1000000000)
			break;

		groups.clear();

		for (auto &comp : components)
		{
			groups.push_back({comp});
		}

		std::set<std::string> group_1, group_2;

		// std::cout << "Groups " << groups.size() << std::endl;

		uint32_t safe_count{0};

		while (groups.size() > 2)
		{
			safe_count++;
			if (safe_count > 1000000000)
				break;

			auto rand_i = dist(rng);

			// std::cout << connections[rand_i].a << " <-> " << connections[rand_i].b << std::endl;

			auto it_1 = std::find_if(groups.begin(), groups.end(),
			                         [&](std::set<std::string> &group) { return group.count(connections[rand_i].a); });
			auto it_2 = std::find_if(groups.begin(), groups.end(),
			                         [&](std::set<std::string> &group) { return group.count(connections[rand_i].b); });

			// std::cout << "it1 " << it_1->size() << " it2 " << it_2->size() << std::endl;

			if (it_1 == it_2)
			{
				continue;
			}

			// std::cout << "it_1" << std::endl;
			for (auto &s : *it_1)
			{
				// std::cout << "\t" << s << std::endl;
			}
			// std::cout << "it_2" << std::endl;
			for (auto &s : *it_2)
			{
				// std::cout << "\t" << s << std::endl;
			}

			it_1->insert(it_2->begin(), it_2->end());

			// std::cout << "After insert it_1" << std::endl;
			for (auto &s : *it_1)
			{
				// std::cout << "\t" << s << std::endl;
			}

			groups.erase(it_2);
		}

		// std::cout << "check wires" << std::endl;

		uint32_t cut_wires{0};
		for (int i = 0; i < connections.size(); ++i)
		{
			auto left  = std::find_if(groups.begin(), groups.end(),
			                          [&](std::set<std::string> &group) { return group.count(connections[i].a); });
			auto right = std::find_if(groups.begin(), groups.end(),
			                          [&](std::set<std::string> &group) { return group.count(connections[i].b); });

			if (left != right)
			{
				// std::cout << "\t" << connections[i].a << " and " << connections[i].b << " are in different groups, wire cut" << std::endl;
				cut_wires++;
			}
		}

		// std::cout << cut_wires << std::endl;

		if (3 == cut_wires)
		{
			cut_three = true;
		}
	}

	std::cout << cut_three << ": " << groups.size() << std::endl;

	return std::accumulate(groups.begin(), groups.end(), 1,
	                       [](uint64_t p, const std::vector<std::set<std::string>>::value_type &g) { return p * g.size(); });
}

uint64_t puzzle_25_2(std::ifstream &in_file)
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

uint64_t puzzle_25(std::ifstream &in_file)
{
	return puzzle_25_1(in_file);
}
