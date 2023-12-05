#include "Puzzles.hpp"

namespace
{
using Seedmap = std::unordered_map<uint64_t, uint64_t>;

std::vector<uint64_t> seeds;

Seedmap seedToSoil;
Seedmap soilToFertilizer;
Seedmap fertilizerToWater;
Seedmap waterToLight;
Seedmap lightToTemperature;
Seedmap temperatureToHumidity;
Seedmap humidityToLocation;

inline void parse_map(std::ifstream &in_file, Seedmap &seed_map)
{
	std::string line;
	uint64_t    num;

	std::getline(in_file, line);        // Title
	while (std::getline(in_file, line) && line.size() > 0)
	{
		std::stringstream ss(line);

		uint64_t src{0}, dst{0}, range{0};
		ss >> dst >> src >> range;

		for (uint64_t i = 0; i < range; i++)
		{
			seed_map[src + i] = dst + i;
		}
	}
}

void parse_maps(std::ifstream &in_file)
{
	seeds.clear();

	seedToSoil.clear();
	soilToFertilizer.clear();
	fertilizerToWater.clear();
	waterToLight.clear();
	lightToTemperature.clear();
	temperatureToHumidity.clear();
	humidityToLocation.clear();

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
		auto soil        = seedToSoil.count(seed) ? seedToSoil[seed] : seed;
		auto fertilizer  = soilToFertilizer.count(soil) ? soilToFertilizer[soil] : soil;
		auto water       = fertilizerToWater.count(fertilizer) ? fertilizerToWater[fertilizer] : fertilizer;
		auto light       = waterToLight.count(water) ? waterToLight[water] : water;
		auto temperature = lightToTemperature.count(light) ? lightToTemperature[light] : light;
		auto humidity    = temperatureToHumidity.count(temperature) ? temperatureToHumidity[temperature] : temperature;
		auto location    = humidityToLocation.count(humidity) ? humidityToLocation[humidity] : humidity;

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
