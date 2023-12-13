#include "Puzzles.hpp"

namespace
{
constexpr uint32_t UNFOLD_FACTOR = 5;

using Record = std::pair<std::string, std::vector<uint32_t>>;

inline uint64_t key(uint32_t a, uint32_t b, uint32_t c)
{
	return static_cast<uint64_t>(a) << 20 | static_cast<uint64_t>(b) << 10 | c;
}

using Cache = std::unordered_map<uint64_t, uint64_t>;

Cache cache{};

uint64_t get_arrangement_count(Record &record, uint32_t i_springs = 0, uint32_t i_sizes = 0, uint32_t cur_size = 0)
{
	uint64_t ret{0};

	auto &springs = record.first;
	auto &sizes   = record.second;

	uint64_t cache_key = key(i_springs, i_sizes, cur_size);
	if (cache.count(cache_key))
	{
		return cache[cache_key];
	}

	if (i_springs == springs.size())
	{
		// Reached the end of the springs
		if (i_sizes == sizes.size() && cur_size == 0)
		{
			// No more pending groups, arrangement correct
			return 1;
		}
		else if (i_sizes == sizes.size() - 1 && sizes[i_sizes] == cur_size)
		{
			// Last pending group of broken springs matches expected size,
			// arrangement correct
			return 1;
		}
		else
		{
			// Otherwise this arrangement is incorrect
			return 0;
		}
	}

	if ('.' == springs[i_springs] || '?' == springs[i_springs])
	{
		if (0 == cur_size)
		{
			// Calculate arrangements assuming no group of broken springs started
			ret += get_arrangement_count(record, i_springs + 1, i_sizes, 0);
		}
		else if (cur_size > 0 && i_sizes < sizes.size() && cur_size == sizes[i_sizes])
		{
			// Reached the max number of broken springs in this group
			// There are more groups, keep counting arrangements for rest of record
			ret += get_arrangement_count(record, i_springs + 1, i_sizes + 1, 0);
		}
	}

	if ('#' == springs[i_springs] || '?' == springs[i_springs])
	{
		// Calculate arrangements assuming current group is correct and growing
		ret += get_arrangement_count(record, i_springs + 1, i_sizes, cur_size + 1);
	}

	cache[cache_key] = ret;
	return ret;
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
		sum += get_arrangement_count(record);

		cache.clear();
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

			unfolded.second.insert(unfolded.second.end(), record.second.begin(), record.second.end());
		}

		sum += get_arrangement_count(unfolded);

		cache.clear();
	}

	return sum;
}
}        // namespace

uint64_t puzzle_12(std::ifstream &in_file)
{
	return puzzle_12_2(in_file);
}
