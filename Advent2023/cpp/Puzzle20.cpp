#include "Puzzles.hpp"

#include <deque>
#include <set>

// Lowest common multiple functions from https://stackoverflow.com/a/4229930
namespace
{
uint64_t gcd(uint64_t a, uint64_t b)
{
	while (true)
	{
		if (0 == a)
		{
			return b;
		}

		b %= a;

		if (0 == b)
		{
			return a;
		}

		a %= b;
	}
}

uint64_t lcm(uint64_t a, uint64_t b)
{
	uint64_t temp = gcd(a, b);

	return temp ? (a / temp * b) : 0;
}

uint64_t lcm_vector(std::vector<uint64_t> v)
{
	uint64_t result = std::accumulate(v.begin(), v.end(), 1ULL, lcm);

	return result;
}

enum class ModuleType
{
	BC   = 0,
	FF   = 1,
	Conj = 2,
	Out  = 3,
};

struct Pulse
{
	std::string from;
	std::string to;
	bool        high{false};
};

using PulseList = std::deque<Pulse>;

void send_pulse(Pulse pulse, PulseList &pulse_list);

class Module
{
  public:
	virtual ModuleType get_type() = 0;

	virtual void process_pulse(Pulse &pulse, PulseList &pulse_list) = 0;

	inline std::string get_name()
	{
		return name;
	}

	inline void set_name(std::string n)
	{
		name = n;
	}

	inline std::vector<std::string> get_outputs()
	{
		return outputs;
	}

	void add_output(std::string name)
	{
		outputs.push_back(name);
	}

  protected:
	std::string name{};

  private:
	std::vector<std::string> outputs;
};

class Broadcaster : public Module
{
  public:
	ModuleType get_type() override
	{
		return ModuleType::BC;
	}

	void process_pulse(Pulse &pulse, PulseList &pulse_list) override
	{
		for (auto &out : get_outputs())
		{
			// std::cout << "Broadcaster sends " << pulse.high << " to " << out << std::endl;

			send_pulse({name, out, pulse.high}, pulse_list);
		}
	}
};

class FlipFlop : public Module
{
  public:
	ModuleType get_type() override
	{
		return ModuleType::FF;
	}

	void process_pulse(Pulse &pulse, PulseList &pulse_list) override
	{
		if (!pulse.high)
		{
			state = !state;

			for (auto &out : get_outputs())
			{
				// std::cout << "FlipFlop " << name << " sends " << state << " to " << out << std::endl;

				send_pulse({name, out, state}, pulse_list);
			}
		}
	}

  private:
	bool state{false};
};

class Conjuction : public Module
{
  public:
	ModuleType get_type() override
	{
		return ModuleType::Conj;
	}

	void process_pulse(Pulse &pulse, PulseList &pulse_list) override
	{
		inputs[pulse.from] = pulse.high;

		auto all_high = std::accumulate(inputs.begin(), inputs.end(), true,
		                                [](bool value, const std::unordered_map<std::string, bool>::value_type &p) { return value & p.second; });

		if (!all_high)
		{
			sent_high = true;
		}

		for (auto &out : get_outputs())
		{
			// std::cout << "Conjuction " << name << " sends " << !all_high << " to " << out << std::endl;

			send_pulse({name, out, !all_high}, pulse_list);
		}
	}

	inline std::unordered_map<std::string, bool> get_inputs()
	{
		return inputs;
	}

	inline void add_input(std::string in)
	{
		inputs[in] = false;
	}

	inline uint64_t get_cycle()
	{
		return cycle;
	}

	void set_cycle(uint64_t c)
	{
		cycle = c;
	}

	inline bool get_sent_high()
	{
		return sent_high;
	}

  private:
	std::unordered_map<std::string, bool> inputs;

	uint64_t cycle{0};

	bool sent_high{false};
};

class OutputModule : public Module
{
  public:
	ModuleType get_type() override
	{
		return ModuleType::Out;
	}

	void process_pulse(Pulse &pulse, PulseList &pulse_list) override
	{
		// std::cout << "OutputModule " << name << " does nothing" << std::endl;
	}
};

// Globals and helper functions
const std::string BROADCASTER = "broadcaster";
const std::string TARGET      = "output";        // "rx" in the real puzzle input

bool PART_TWO{false};

std::unordered_map<std::string, Module *> modules;

uint64_t low_count{0};
uint64_t high_count{0};

void send_pulse(Pulse pulse, PulseList &pulse_list)
{
	if (pulse.high)
	{
		high_count++;
	}
	else
	{
		low_count++;
	}

	pulse_list.push_back(pulse);
}

void get_conj_parents(std::string &name, std::set<std::string> &parents, std::unordered_map<std::string, std::vector<std::string>> &inputs)
{
	static std::unordered_map<std::string, bool> visited;

	if (inputs.count(name))
	{
		if (!visited.count(name) || !visited[name])
		{
			visited[name] = true;

			for (auto &in : inputs[name])
			{
				if (modules[in]->get_type() == ModuleType::Conj)
				{
					parents.insert(in);
					get_conj_parents(in, parents, inputs);
				}
			}
		}
	}
}

uint64_t puzzle_20_1(std::ifstream &in_file)
{
	std::unordered_map<std::string, std::vector<std::string>> inputs;

	// Parse modules
	std::string line;
	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			char    first_char = line[0];
			Module *module{nullptr};

			// Module name
			std::string name{};
			auto        pos = line.find('>');
			if (pos != std::string::npos)
			{
				name = line.substr(1, pos - 3);
				line = line.substr(pos + 2);
			}

			// Module type
			switch (first_char)
			{
				case '%':
				{
					modules.insert(std::make_pair(name, new FlipFlop()));
					break;
				}
				case '&':
				{
					modules.insert(std::make_pair(name, new Conjuction()));
					break;
				}
				case 'b':
				{
					name = BROADCASTER;
					modules.insert(std::make_pair(name, new Broadcaster()));
					break;
				}
				default:
				{
					std::cout << "Error" << std::endl;
				}
			}

			// Module outputs
			module = modules[name];
			module->set_name(name);
			while (!line.empty())
			{
				std::string output{};

				auto pos = line.find(',');
				if (pos != std::string::npos)
				{
					output = line.substr(0, pos);
					line   = line.substr(pos + 2);
				}
				else
				{
					output = line;
					line   = "";
				}

				module->add_output(output);

				inputs[output].push_back(module->get_name());
			}

			// Conjuction module inputs
			if (module->get_type() == ModuleType::Conj)
			{
				auto *conj = dynamic_cast<Conjuction *>(module);
				for (auto in : inputs[module->get_name()])
				{
					conj->add_input(in);
				}
			}
		}
	}

	// Add output modules (modules with no outputs)
	for (auto &in : inputs)
	{
		if (!modules.count(in.first))
		{
			modules.insert(std::make_pair(in.first, new OutputModule()));
			modules[in.first]->set_name(in.first);
		}
	}

	// Part 2: Calculate size of repeating cycle for all
	// ancestors above the target's parent (once the parent
	// is set, the target will receive a low pulse, but
	// this will take too many button presses)
	std::set<std::string> parents;
	std::string           target_parent = inputs[TARGET][0];        // Assumes single parent

	get_conj_parents(target_parent, parents, inputs);
	parents.erase(target_parent);

	uint64_t pulse_prod{0};
	uint64_t button_presses{0};
	bool     stop{false};
	while (!stop)
	{
		button_presses++;

		// Broadcast pulses
		PulseList pulse_list;
		send_pulse({"button", BROADCASTER, false}, pulse_list);

		while (!pulse_list.empty())
		{
			auto p = pulse_list.front();
			pulse_list.pop_front();

			modules[p.to]->process_pulse(p, pulse_list);
		}

		if (!PART_TWO)
		{
			// Part 1: interested in first thousand cycles
			if (1000 == button_presses)
			{
				pulse_prod = high_count * low_count;

				stop = true;
			}
		}
		else
		{
			// Part 2: find cycles of ancestor conjuction nodes
			stop = true;
			for (auto &parent : parents)
			{
				auto parent_mod = dynamic_cast<Conjuction *>(modules[parent]);
				if (parent_mod->get_sent_high() && 0 == parent_mod->get_cycle())
				{
					parent_mod->set_cycle(button_presses);
				}

				if (parent_mod->get_cycle() == 0)
				{
					stop = false;
				}
			}
		}
	}

	// Find LCM of all ancestors' cycles, this will be the number of button presses
	// required for all of them to send a high pulse to the target's parent,
	// which will in turn deliver a low pulse to the target
	std::vector<uint64_t> cycles;
	for (auto &parent : parents)
	{
		auto cycle = dynamic_cast<Conjuction *>(modules[parent])->get_cycle();
		cycles.push_back(cycle);
	}

	return PART_TWO ? lcm_vector(cycles) : pulse_prod;
}

uint64_t puzzle_20_2(std::ifstream &in_file)
{
	PART_TWO = true;

	return puzzle_20_1(in_file);
}
}        // namespace

uint64_t puzzle_20(std::ifstream &in_file)
{
	return puzzle_20_2(in_file);
}
