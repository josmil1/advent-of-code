#include "Puzzles.hpp"

namespace
{
void new_cycle(uint64_t *cycle, std::vector<int64_t> *strengths, const int64_t X)
{
	// std::cout << "cycle: " << *cycle << " X: " << X << std::endl;
	//  Interested in the strength during the 20th cycle and every 40 cycles after that
	constexpr uint32_t first{20};
	constexpr uint32_t cadence{40};

	if (first == *cycle ||
	    (*cycle >= cadence && (*cycle - first) % cadence == 0))
	{
		// Interesting signal
		strengths->push_back(X * *cycle);
		// std::cout << "New strength " << X * *cycle << std::endl;
	}

	*cycle += 1;
}

uint64_t puzzle_10_1(std::ifstream &in_file)
{
	std::string line;

	int64_t  X{1};
	uint64_t cycle{1};

	std::vector<int64_t> interesting_signal_strengths;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			std::stringstream ss(line);
			std::string       command;
			ss >> command;

			if ("noop" == command)
			{
				// No-op
				// std::cout << "noop" << std::endl;
				new_cycle(&cycle, &interesting_signal_strengths, X);
			}
			else if ("addx" == command)
			{
				int64_t value;
				ss >> value;

				// std::cout << "add " << value << std::endl;
				new_cycle(&cycle, &interesting_signal_strengths, X);
				new_cycle(&cycle, &interesting_signal_strengths, X);
				X += value;
			}
			else
			{
				std::cout << "Error, unknown command " << command << std::endl;
			}
		}
	}

	new_cycle(&cycle, &interesting_signal_strengths, X);
	auto strength_sum = std::accumulate(interesting_signal_strengths.begin(), interesting_signal_strengths.end(), 0);

	return strength_sum;
}

uint64_t puzzle_10_2(std::ifstream &in_file)
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

uint64_t puzzle_10(std::ifstream &in_file)
{
	return puzzle_10_1(in_file);
}
