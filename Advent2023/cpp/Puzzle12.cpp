#include "Puzzles.hpp"

namespace
{
constexpr uint32_t UNFOLD_FACTOR = 5;

using Record = std::pair<std::string, std::vector<uint32_t>>;

uint64_t get_arrangement_count(Record &record, uint32_t cur_size = 0)
{
	auto &springs = record.first;
	auto &sizes   = record.second;

	for (size_t i = 0; i < springs.size(); i++)
	{
		switch (springs[i])
		{
			case '.':
			{
				if (0 == cur_size)
				{
					// No group of broken springs started
					continue;
				}
				else if (cur_size == sizes[0])
				{
					// Reached the max number of broken springs in this group
					if (sizes.size() > 1)
					{
						// There are more groups
						// Keep counting arrangements for rest of record
						Record remaining_record;
						remaining_record.first  = springs.substr(i + 1);
						remaining_record.second = std::vector<uint32_t>(sizes.begin() + 1, sizes.end());

						return get_arrangement_count(remaining_record);
					}
					else
					{
						// If there are any more broken springs, this arrangement is incorrect
						// Otherwise this is the 1 correct arrangement
						bool more_broken = springs.substr(i + 1).find('#') != std::string::npos;
						return more_broken ? 0 : 1;
					}
				}

				// Group ended but size is incorrect
				// This arrangement is not valid, stop here
				return 0;
			}
			case '#':
			{
				cur_size++;
				break;
			}
			case '?':
			{
				// So far the arrangement is valid, but now there is a branching
				// path of possible arrangements, add count for both
				Record remaining_record_option_1;
				remaining_record_option_1.first  = "." + springs.substr(i + 1);
				remaining_record_option_1.second = sizes;

				Record remaining_record_option_2;
				remaining_record_option_2.first  = "#" + springs.substr(i + 1);
				remaining_record_option_2.second = sizes;

				return get_arrangement_count(remaining_record_option_1, cur_size) +
				       get_arrangement_count(remaining_record_option_2, cur_size);
			}
			default:
			{
				std::cout << "Error: unhandled char " << springs[i] << std::endl;
				break;
			}
		}
	}

	// Finished counting arrangements, only trailing '.' remain
	return 0;
}

std::vector<Record> parse_records(std::ifstream &in_file)
{
	std::string line;

	std::string str, tmp;
	uint32_t    num;
	const char  sep = ',';

	std::vector<Record> records;

	while (!in_file.eof())
	{
		while (std::getline(in_file, line))
		{
			Record new_record;

			std::stringstream ss(line);

			// Springs
			ss >> new_record.first;

			// Sizes
			ss >> str;
			std::istringstream iss(str);
			while (std::getline(iss, tmp, sep))
			{
				if (std::istringstream(tmp) >> num)
				{
					new_record.second.push_back(num);
				}
			}

			records.push_back(new_record);
		}
	}

	return records;
}

uint64_t puzzle_12_1(std::ifstream &in_file)
{
	auto records = parse_records(in_file);

	uint64_t sum{0};

	for (auto &record : records)
	{
		// Ensure all # groups terminate with '.'
		// since we evaluate arrangement correctness
		// when reaching these delimiters
		record.first += ".";

		sum += get_arrangement_count(record);
	}

	return sum;
}

uint64_t puzzle_12_2(std::ifstream &in_file)
{
	auto records = parse_records(in_file);

	uint64_t sum{0};

	for (auto &record : records)
	{
		Record unfolded;
		unfolded.second.reserve(UNFOLD_FACTOR * record.second.size());
		for (uint32_t i = 0; i < UNFOLD_FACTOR; i++)
		{
			unfolded.first += record.first;
			if (i < UNFOLD_FACTOR - 1)
			{
				unfolded.first += "?";
			}
			else
			{
				unfolded.first += ".";
			}
			unfolded.second.insert(unfolded.second.end(), record.second.begin(), record.second.end());
		}

		sum += get_arrangement_count(unfolded);
	}

	return sum;
}
}        // namespace

uint64_t puzzle_12(std::ifstream &in_file)
{
	return puzzle_12_2(in_file);
}
