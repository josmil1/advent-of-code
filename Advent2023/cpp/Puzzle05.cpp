#include "Puzzles.hpp"

namespace
{
struct Mapping
{
	uint64_t src{0};
	uint64_t dst{0};
	uint64_t range{0};
};

class SeedMap
{
  public:
	inline void add_range(uint64_t src, uint64_t dst, uint64_t range)
	{
		mappings.push_back({src, dst, range});
	}

	uint64_t get(uint64_t input)
	{
		uint64_t output = input;

		for (auto &mapping : mappings)
		{
			if (input >= mapping.src && input <= mapping.src + mapping.range)
			{
				output = mapping.dst + (input - mapping.src);
				break;
			}
		}

		return output;
	}

  private:
	std::vector<Mapping> mappings;
};

using Seedmap = std::unordered_map<uint64_t, uint64_t>;

std::vector<uint64_t> seeds;

SeedMap seedToSoil;
SeedMap soilToFertilizer;
SeedMap fertilizerToWater;
SeedMap waterToLight;
SeedMap lightToTemperature;
SeedMap temperatureToHumidity;
SeedMap humidityToLocation;

inline void parse_map(std::ifstream &in_file, SeedMap &seed_map)
{
	std::string line;
	uint64_t    num;

	std::getline(in_file, line);        // Title
	while (std::getline(in_file, line) && line.size() > 0)
	{
		std::stringstream ss(line);

		uint64_t src{0}, dst{0}, range{0};
		ss >> dst >> src >> range;

		seed_map.add_range(src, dst, range);
	}
}

void parse_maps(std::ifstream &in_file)
{
	seeds.clear();

	std::string line;
	std::string str;
	uint64_t    num;

	// Seeds
	{
		std::getline(in_file, line);
		std::stringstream ss(line);

		ss >> str;
		while (ss >> num)
		{
			seeds.push_back(num);
		}

		std::getline(in_file, line);        // Empty
	}

	// Seed to Soil
	parse_map(in_file, seedToSoil);
	parse_map(in_file, soilToFertilizer);
	parse_map(in_file, fertilizerToWater);
	parse_map(in_file, waterToLight);
	parse_map(in_file, lightToTemperature);
	parse_map(in_file, temperatureToHumidity);
	parse_map(in_file, humidityToLocation);
}

uint64_t puzzle_05_1(std::ifstream &in_file)
{
	parse_maps(in_file);

	uint64_t lowest_location{std::numeric_limits<uint64_t>::max()};

	for (auto &seed : seeds)
	{
		auto soil        = seedToSoil.get(seed);
		auto fertilizer  = soilToFertilizer.get(soil);
		auto water       = fertilizerToWater.get(fertilizer);
		auto light       = waterToLight.get(water);
		auto temperature = lightToTemperature.get(light);
		auto humidity    = temperatureToHumidity.get(temperature);
		auto location    = humidityToLocation.get(humidity);

		lowest_location = std::min(lowest_location, location);
	}

	return lowest_location;
}

uint64_t puzzle_05_2(std::ifstream &in_file)
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

uint64_t puzzle_05(std::ifstream &in_file)
{
	return puzzle_05_1(in_file);
}
