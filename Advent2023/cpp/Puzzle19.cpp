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

using Workflow = std::vector<Rule>;

struct Part
{
	uint32_t x{0};
	uint32_t m{0};
	uint32_t a{0};
	uint32_t s{0};
};

const std::string START    = "in";
const std::string ACCEPTED = "A";
const std::string REJECTED = "R";

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

void parse_input(std::ifstream &in_file, std::unordered_map<std::string, Workflow> &workflows, std::vector<Part> &parts)
{
	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if ('{' == line[0])
			{
				// Parse part
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
				// Parse workflow
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

				workflows[flow_name] = rules;
			}
		}
	}
}

uint64_t puzzle_19_1(std::ifstream &in_file)
{
	std::unordered_map<std::string, Workflow> flows;
	std::vector<Part>                         parts;

	parse_input(in_file, flows, parts);

	uint64_t accepted_ratings{0};

	for (auto &p : parts)
	{
		// Process workflow
		std::string workflow_name = START;
		int         rule_index    = 0;

		while (workflow_name != ACCEPTED && workflow_name != REJECTED)
		{
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

		if (ACCEPTED == workflow_name)
		{
			accepted_ratings += p.x + p.m + p.a + p.s;
		}
	}

	return accepted_ratings;
}

struct Range
{
	uint32_t min{};
	uint32_t max{};
};

void split_ranges(std::unordered_map<std::string, Workflow> &flows, std::string next, std::unordered_map<Category, Range> &ranges, uint64_t &accepted_combinations)
{
	if (next == REJECTED)
	{
		// Workflow end, not accepted
		return;
	}

	if (next == ACCEPTED)
	{
		// Workflow end, calculate all
		// combinations that are accepted
		uint64_t prod{1};
		for (auto &cat : ranges)
		{
			auto &range = cat.second;
			if (range.max < range.min)
			{
				std::cout << "Error" << std::endl;
				return;
			}

			prod *= (range.max - range.min + 1);
		}

		// Add to the total
		accepted_combinations += prod;

		return;
	}

	// Process workflow
	auto &workflow = flows[next];
	for (auto &rule : workflow)
	{
		if (0 == rule.comparison)
		{
			// Last rule of this workflow
			split_ranges(flows, rule.destination, ranges, accepted_combinations);
			return;
		}

		if (rule.greater_than)
		{
			auto greater_ranges = ranges;

			auto &range = ranges[rule.cat];

			// For values in the range that pass this rule, calculate
			// how many will be accepted
			if (range.max > rule.comparison)
			{
				greater_ranges[rule.cat] = {std::max(range.min, rule.comparison + 1), range.max};
			}
			split_ranges(flows, rule.destination, greater_ranges, accepted_combinations);

			// For those that fail, continue processing rules in this workflow
			if (range.min <= rule.comparison)
			{
				ranges[rule.cat] = {range.min, std::min(range.max, rule.comparison)};
			}
		}
		else
		{
			auto lower_ranges = ranges;

			auto &range = ranges[rule.cat];

			// For values in the range that pass this rule, calculate
			// how many will be accepted
			if (range.min < rule.comparison)
			{
				lower_ranges[rule.cat] = {range.min, std::min(range.max, rule.comparison - 1)};
			}
			split_ranges(flows, rule.destination, lower_ranges, accepted_combinations);

			// For those that fail, continue processing rules in this workflow
			if (range.max >= rule.comparison)
			{
				ranges[rule.cat] = {std::max(range.min, rule.comparison), range.max};
			}
		}
	}
}

uint64_t puzzle_19_2(std::ifstream &in_file)
{
	std::unordered_map<std::string, Workflow> flows;
	std::vector<Part>                         parts;

	parse_input(in_file, flows, parts);

	// Ignore input parts, use ranges of possible inputs
	constexpr int MIN = 1;
	constexpr int MAX = 4000;

	std::unordered_map<Category, Range> ranges;
	ranges[Category::X] = {MIN, MAX};
	ranges[Category::M] = {MIN, MAX};
	ranges[Category::A] = {MIN, MAX};
	ranges[Category::S] = {MIN, MAX};

	uint64_t accepted_combinations{0};

	split_ranges(flows, START, ranges, accepted_combinations);

	return accepted_combinations;
}
}        // namespace

uint64_t puzzle_19(std::ifstream &in_file)
{
	return puzzle_19_2(in_file);
}
