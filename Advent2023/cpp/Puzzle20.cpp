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

const std::string BROADCASTER = "broadcaster";

enum class ModuleType
{
	BC   = 0,
	FF   = 1,
	Conj = 2,
	Test = 3,
};

struct Pulse
{
	std::string from;
	std::string to;
	bool        high{false};
};

using PulseList = std::deque<Pulse>;

class Module
{
  public:
	virtual ModuleType get_type() = 0;

	inline void set_name(std::string n)
	{
		name = n;
	}

	inline std::string get_name()
	{
		return name;
	}

	void add_output(std::string name)
	{
		outputs.push_back(name);
	}

	inline std::vector<std::string> get_outputs()
	{
		return outputs;
	}

	/*
	    void receive(Pulse &pulse)
	    {
	        last_pulse_high = pulse.high;
	    }
	*/

	virtual void process_pulse(Pulse &pulse, PulseList &pulse_list) = 0;

  protected:
	// bool last_pulse_high{false};

	std::string name{};

  private:
	std::vector<std::string> outputs;
};

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

	/*
	void receive(bool high_pulse) override
	{
	    Module::receive(high_pulse);
	}
	*/

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

			pulse_count++;
		}
	}

	inline void add_input(std::string in)
	{
		inputs[in] = false;
	}

	inline std::unordered_map<std::string, bool> get_inputs()
	{
		return inputs;
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

	uint64_t pulse_count{0};

	bool sent_high{false};
};

class TestModule : public Module
{
  public:
	ModuleType get_type() override
	{
		return ModuleType::Test;
	}

	void process_pulse(Pulse &pulse, PulseList &pulse_list) override
	{
		// std::cout << "TestModule " << name << " does nothing" << std::endl;
	}
};

std::unordered_map<std::string, bool> visited;

void get_parents(std::string &name, std::set<std::string> &parents, std::unordered_map<std::string, std::vector<std::string>> &inputs)
{
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
					get_parents(in, parents, inputs);
				}
			}
		}
	}
}

uint64_t puzzle_20_1(std::ifstream &in_file)
{
	std::string line;

	std::unordered_map<std::string, std::vector<std::string>> inputs;

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

			module = modules[name];
			module->set_name(name);

			// Module inputs/outputs
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
				// inputs[module->get_name()].push_back(output);
				inputs[output].push_back(module->get_name());
			}

			if (module->get_type() == ModuleType::Conj)
			{
				auto *conj = dynamic_cast<Conjuction *>(module);
				for (auto in : inputs[module->get_name()])
				{
					conj->add_input(in);
				}

				/*
				    std::cout << name << std::endl;
				    for (auto m : conj->get_inputs())
				    {
				        std::cout << "\t" << m << std::endl;
				    }
				*/
			}

			/*
			std::cout << name << std::endl;
			for (auto m : module->get_outputs())
			{
			    std::cout << "\t" << m << std::endl;
			}
			*/
		}
	}

	// Add test modules
	for (auto &in : inputs)
	{
		if (!modules.count(in.first))
		{
			modules.insert(std::make_pair(in.first, new TestModule()));
			modules[in.first]->set_name(in.first);
		}
	}

	// Find rx and all parents
	std::set<std::string> parents;
	// std::string           target = "output";
	std::string target        = "rx";
	std::string target_parent = inputs[target][0];

	get_parents(target_parent, parents, inputs);

	// for (auto &mod : modules)
	//{
	//	if (mod.second->get_type() == ModuleType::Conj)
	//	{
	//		parents.insert(mod.first);
	//	}
	// }

	parents.erase(target_parent);

	// Broadcast pulses
	// constexpr int BUTTON_PRESSES = 1000;

	uint64_t button_presses{0};

	// for (int i = 0; i < BUTTON_PRESSES; i++)
	bool stop{false};
	while (!stop)
	{
		button_presses++;

		PulseList pulse_list;
		send_pulse({"button", BROADCASTER, false}, pulse_list);

		while (!pulse_list.empty())
		{
			auto p = pulse_list.front();
			pulse_list.pop_front();

			// modules[p.name]->receive(p.high);
			modules[p.to]->process_pulse(p, pulse_list);
		}

		// std::cout << high_count << " * " << low_count << std::endl;
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
				// std::cout << "no cycle for " << parent << std::endl;
				stop = false;
			}
		}
		// std::cout << std::endl;
	}

	std::vector<uint64_t> cycles;

	for (auto &parent : parents)
	{
		auto cycle = dynamic_cast<Conjuction *>(modules[parent])->get_cycle();
		cycles.push_back(cycle);
		std::cout << modules[parent]->get_name() << " -> " << cycle << std::endl;
	}

	std::cout << button_presses << std::endl;

	return lcm_vector(cycles);
	// return low_count * high_count;
}

uint64_t puzzle_20_2(std::ifstream &in_file)
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

uint64_t puzzle_20(std::ifstream &in_file)
{
	return puzzle_20_1(in_file);
}
