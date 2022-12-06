#include "Puzzles.hpp"

#include <deque>
#include <map>
#include <stack>

using Move      = std::pair<uint32_t, uint32_t>;
using CountMove = std::pair<uint32_t, Move>;
using StackMap  = std::map<uint32_t, std::deque<char>>;

struct Inputs
{
	std::vector<CountMove> moves;
	StackMap               stacks;
};

namespace
{
void print_stacks(StackMap stacks)
{
	for (auto &stack : stacks)
	{
		std::cout << "stack " << stack.first << ": ";
		for (auto &item : stack.second)
		{
			std::cout << item << " ";
		}
		std::cout << std::endl;
	}
}

Inputs parse_input(std::ifstream &in_file)
{
	Inputs      inputs;
	std::string line;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (line[0] == 'm')
			{
				// Parse moves
				std::stringstream ss(line);
				std::string       bin;        // Discard separators
				uint32_t          count, from, to;
				ss >> bin >> count >> bin >> from >> bin >> to;

				Move      new_move       = {from, to};
				CountMove new_count_move = {count, new_move};
				inputs.moves.push_back(new_count_move);
			}
			else if (line[1] != '1')
			{
				// Parse stacks
				for (int i = 0; i < line.size(); i++)
				{
					if (line[i] == '[')
					{
						char     item = line[i + 1];
						uint32_t pos  = i / 4 + 1;

						inputs.stacks[pos].push_back(item);
					}
				}
			}
		}
	}

	return inputs;
}

uint64_t puzzle_05_1(std::ifstream &in_file)
{
	Inputs inputs = parse_input(in_file);
	auto & moves  = inputs.moves;
	auto & stacks = inputs.stacks;

	// Perform rearrangements
	for (auto &move : moves)
	{
		auto count = move.first;
		auto from  = move.second.first;
		auto to    = move.second.second;

		for (int i = 0; i < count; i++)
		{
			auto item = stacks[from].front();
			stacks[from].pop_front();
			stacks[to].push_front(item);
		}
	}

	std::string answer;
	for (auto &stack : stacks)
	{
		answer += stack.second.front();
	}

	std::cout << answer << std::endl;

	return 0;
}

uint64_t puzzle_05_2(std::ifstream &in_file)
{
	Inputs inputs = parse_input(in_file);
	auto & moves  = inputs.moves;
	auto & stacks = inputs.stacks;

	// Perform rearrangements
	for (auto &move : moves)
	{
		auto count = move.first;
		auto from  = move.second.first;
		auto to    = move.second.second;

		std::stack<char> temp;
		for (int i = 0; i < count; i++)
		{
			auto item = stacks[from].front();
			temp.push(item);
			stacks[from].pop_front();
		}

		auto temp_size = temp.size();
		for (int i = 0; i < temp_size; i++)
		{
			stacks[to].push_front(temp.top());
			temp.pop();
		}
	}

	std::string answer;
	for (auto &stack : stacks)
	{
		answer += stack.second.front();
	}

	std::cout << answer << std::endl;

	return 0;
}
}        // namespace

uint64_t puzzle_05(std::ifstream &in_file)
{
	return puzzle_05_2(in_file);
}
