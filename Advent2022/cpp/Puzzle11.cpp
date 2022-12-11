#include "Puzzles.hpp"

namespace
{
struct Monkey
{
	uint32_t              id;
	std::vector<uint64_t> items;
	char                  op;
	std::string           factor_a;
	std::string           factor_b;
	uint32_t              div;
	uint32_t              true_id;
	uint32_t              false_id;
	uint32_t              inspection_count;
};

using MonkeyMap = std::unordered_map<uint32_t, Monkey>;

MonkeyMap get_monkeys(std::ifstream &in_file)
{
	MonkeyMap   monkeys;
	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			Monkey            new_monkey{};
			std::stringstream ss(line);

			// Monkey <id>:
			std::string bin_str;
			ss >> bin_str >> new_monkey.id;

			// Starting items: <item>, ..., <item>
			std::getline(in_file, line);
			ss = std::stringstream(line);
			char     bin_c;
			uint32_t item;
			ss >> bin_str >> bin_str;
			while (ss >> item)
			{
				new_monkey.items.push_back(item);
				ss >> bin_c;
			}

			// Operation: new = <factor_a> <op> <factor_b>
			std::getline(in_file, line);
			ss = std::stringstream(line);
			ss >> bin_str >> bin_str >> bin_c;
			ss >> new_monkey.factor_a >> new_monkey.op >> new_monkey.factor_b;

			// Test: divisible by <div>
			std::getline(in_file, line);
			ss = std::stringstream(line);
			ss >> bin_str >> bin_str >> bin_str >> new_monkey.div;

			// If true: throw to monkey 2
			std::getline(in_file, line);
			ss = std::stringstream(line);
			ss >> bin_str >> bin_str >> bin_str >> bin_str >> bin_str >> new_monkey.true_id;

			// If false: throw to monkey 3
			std::getline(in_file, line);
			ss = std::stringstream(line);
			ss >> bin_str >> bin_str >> bin_str >> bin_str >> bin_str >> new_monkey.false_id;

			monkeys[new_monkey.id] = std::move(new_monkey);
		}
	}

	return monkeys;
}

uint64_t get_new_worry(Monkey &monkey, uint32_t old_worry)
{
	uint64_t new_worry{0};
	uint64_t factor_a = (monkey.factor_a == "old") ? old_worry : std::stol(monkey.factor_a);
	uint64_t factor_b = (monkey.factor_b == "old") ? old_worry : std::stol(monkey.factor_b);

	switch (monkey.op)
	{
		case '*':
		{
			new_worry = factor_a * factor_b;
			break;
		}
		case '+':
		{
			new_worry = factor_a + factor_b;
			break;
		}
		default:
		{
			std::cout << "Error, unknown operation " << monkey.op << std::endl;
			break;
		}
	}

	return new_worry;
}

uint64_t puzzle_11_1(std::ifstream &in_file)
{
	auto monkeys = get_monkeys(in_file);

	constexpr uint32_t relief{3};
	constexpr uint32_t rounds{20};

	uint32_t most_inspections{0}, second_most_inspections{0};
	for (size_t r = 0; r < rounds; r++)
	{
		for (size_t i = 0; i < monkeys.size(); i++)
		{
			Monkey &monkey = monkeys[i];

			// Game round
			for (auto item : monkey.items)
			{
				auto new_worry = get_new_worry(monkey, item) / relief;
				if (new_worry % monkey.div == 0)
				{
					monkeys[monkey.true_id].items.push_back(new_worry);
				}
				else
				{
					monkeys[monkey.false_id].items.push_back(new_worry);
				}

				monkey.inspection_count++;
			}

			monkey.items.clear();

			// Monkey business
			if (monkey.inspection_count > second_most_inspections)
			{
				if (monkey.inspection_count > most_inspections)
				{
					second_most_inspections = most_inspections;
					most_inspections        = monkey.inspection_count;
				}
				else
				{
					second_most_inspections = monkey.inspection_count;
				}
			}
		}
	}

	uint64_t monkey_business = most_inspections * second_most_inspections;

	// DEBUG
	/*
	for (auto m : monkeys)
	{
	    auto &monkey = m.second;
	    std::cout << "Monkey " << monkey.id << ": ";
	    for (auto i : monkey.items)
	    {
	        std::cout << i << " ";
	    }
	    std::cout << std::endl;
	    std::cout << "\tInspected " << monkey.inspection_count << " items" << std::endl;
	    std::cout << "\tOperation " << monkey.factor_a << monkey.op << monkey.factor_b << std::endl;
	    std::cout << "\tDivisible by " << monkey.div << std::endl;
	    std::cout << "\tTrue: " << monkey.true_id << " False: " << monkey.false_id << std::endl;
	}
	*/

	return monkey_business;
}

uint64_t puzzle_11_2(std::ifstream &in_file)
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

uint64_t puzzle_11(std::ifstream &in_file)
{
	return puzzle_11_1(in_file);
}
