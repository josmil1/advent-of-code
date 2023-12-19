#include "Puzzles.hpp"

namespace
{
enum Category
{
	X = 0,
	M = 1,
	A = 2,
	S = 3,
};

struct Rule
{
	Category    cat{};
	bool        greater_than{};
	uint32_t    comparison{};
	std::string destination{};
};

using Workflows = std::unordered_map<std::string, std::vector<Rule>>;

struct Part
{
	uint32_t x{0};
	uint32_t m{0};
	uint32_t a{0};
	uint32_t s{0};
};

std::string remove_string_before(std::string &str, char del)
{
	std::string ret = "";

	auto pos = str.find(del);
	if (pos != std::string::npos)
	{
		ret = str.substr(0, pos);
		str = str.substr(pos + 1);
	}

	return ret;
}

uint64_t puzzle_19_1(std::ifstream &in_file)
{
	std::string line;

	Workflows         flows;
	std::vector<Part> parts;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if ('{' == line[0])
			{
				// Part
				Part p{};

				uint32_t    num{};
				std::string str;

				str = remove_string_before(line, '=');
				num = std::stol(remove_string_before(line, ','));
				p.x = num;

				str = remove_string_before(line, '=');
				num = std::stol(remove_string_before(line, ','));
				p.m = num;

				str = remove_string_before(line, '=');
				num = std::stol(remove_string_before(line, ','));
				p.a = num;

				str = remove_string_before(line, '=');
				num = std::stol(remove_string_before(line, '}'));
				p.s = num;

				parts.push_back(p);
			}
			else
			{
				// Workflow
				std::string       flow_name;
				std::vector<Rule> rules;

				// Name
				flow_name = remove_string_before(line, '{');

				// Rules
				std::string str = "";
				while (line.size() > 0)
				{
					auto rule_str = remove_string_before(line, ',');

					if (!rule_str.empty())
					{
						Rule r{};

						switch (rule_str[0])
						{
							case 'x':
							{
								r.cat = Category::X;
								break;
							}
							case 'm':
							{
								r.cat = Category::M;
								break;
							}
							case 'a':
							{
								r.cat = Category::A;
								break;
							}
							case 's':
							{
								r.cat = Category::S;
								break;
							}
							default:
							{
								std::cout << "Error" << std::endl;
							}
						}

						r.greater_than = '>' == rule_str[1];

						rule_str = rule_str.substr(2);

						r.comparison = std::stol(remove_string_before(rule_str, ':'));

						r.destination = rule_str;

						rules.push_back(r);
					}
					else
					{
						Rule last_rule{};

						last_rule.destination = remove_string_before(line, '}');

						rules.push_back(last_rule);
					}
				}

				flows[flow_name] = rules;
			}
		}
	}

	uint64_t accepted_ratings{0};

	for (auto &p : parts)
	{
		std::string workflow_name = "in";
		int         rule_index    = 0;

		while (workflow_name != "A" && workflow_name != "R")
		{
			// std::cout << workflow_name << " -> ";
			auto &rule = flows[workflow_name][rule_index];

			if (rule.comparison > 0)
			{
				uint32_t cat{0};

				switch (rule.cat)
				{
					case Category::X:
					{
						cat = p.x;
						break;
					}
					case Category::M:
					{
						cat = p.m;
						break;
					}
					case Category::A:
					{
						cat = p.a;
						break;
					}
					case Category::S:
					{
						cat = p.s;
						break;
					}
					default:
					{
						std::cout << "Error" << std::endl;
					}
				}

				if ((rule.greater_than && cat > rule.comparison) ||
				    (!rule.greater_than && cat < rule.comparison))
				{
					workflow_name = rule.destination;
					rule_index    = 0;
				}
				else
				{
					rule_index++;
				}
			}
			else
			{
				workflow_name = rule.destination;
				rule_index    = 0;
			}
		}

		if ("A" == workflow_name)
		{
			// std::cout << "Accepted" << std::endl;
			accepted_ratings += p.x + p.m + p.a + p.s;
		}
		else
		{
			// std::cout << "Rejected" << std::endl;
		}

		// std::cout << std::endl;
	}

	return accepted_ratings;
}

uint64_t puzzle_19_2(std::ifstream &in_file)
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

uint64_t puzzle_19(std::ifstream &in_file)
{
	return puzzle_19_1(in_file);
}
