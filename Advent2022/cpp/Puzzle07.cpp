#include "Puzzles.hpp"

#include <cctype>

namespace
{
class Directory
{
  public:
	Directory(std::string name) :
	    name(name)
	{}

	~Directory() = default;

	void add_file(uint64_t s)
	{
		if (!initialized)
		{
			file_size += s;
		}
	}

	Directory *add_directory(std::string child_name)
	{
		Directory *ret = nullptr;

		if (!initialized)
		{
			auto new_dir = std::make_unique<Directory>(child_name);
			new_dir->set_parent(this);
			ret = new_dir.get();

			children.push_back(std::move(new_dir));
		}

		return ret;
	}

	Directory *get_parent()
	{
		return parent;
	}

	void set_parent(Directory *dir)
	{
		parent = dir;
	}

	uint64_t get_total_size()
	{
		auto total_size = file_size;
		for (auto &c : children)
		{
			total_size += c->get_total_size();
		}
		return total_size;
	}

	std::string get_name()
	{
		return name;
	}

	void print(uint32_t level = 0)
	{
		std::cout << name << "(" << get_total_size() << ")" << std::endl;

		if (children.size() > 0)
		{
			auto indent = std::string(level, '\t');
			std::cout << indent << "|" << std::endl;

			for (auto &c : children)
			{
				std::cout << indent;
				c->print(level + 1);
			}
		}
	}

	void set_initialized(bool init)
	{
		initialized = init;
	}

  private:
	uint64_t                                file_size{};
	std::vector<std::unique_ptr<Directory>> children;
	Directory *                             parent{nullptr};
	std::string                             name{};
	bool                                    initialized{false};
};

using DirectoryList = std::vector<Directory *>;

std::unique_ptr<Directory> process_input(std::ifstream &in_file, DirectoryList &dirs)
{
	std::string line;

	std::unique_ptr<Directory> root{};

	Directory *cur{nullptr};

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (line[0] == '$')
			{
				if ('c' == line[2])
				{
					std::string name = line.substr(5);

					if ('.' == name[0])
					{
						// cd out
						if (cur && cur->get_parent())
						{
							cur = cur->get_parent();
						}
						else
						{
							std::cout << "Error, missing parent directory for " << name << std::endl;
						}
					}
					else
					{
						// cd in
						if (cur)
						{
							//cur->set_initialized(true);        // Any ls for the previous directory is done
						}

						if (!root)
						{
							root = std::make_unique<Directory>(name);
							cur  = root.get();
						}
						else
						{
							auto new_dir = cur->add_directory(name);
							dirs.push_back(new_dir);
							cur = new_dir;
						}
					}
				}
			}
			else
			{
				// Processing lines from ls
				std::stringstream ss(line);
				if (isdigit(line[0]))
				{
					// File
					uint64_t file_size;
					ss >> file_size;

					cur->add_file(file_size);
				}
				else
				{
					// Dir
					std::string dir_name, bin;
					ss >> bin >> dir_name;
					//dirs.push_back(cur->add_directory(dir_name));
				}
			}
		}
	}

	return root;
}

uint64_t puzzle_07_1(std::ifstream &in_file)
{
	DirectoryList dirs;
	auto          root = process_input(in_file, dirs);

	constexpr uint64_t max_size = 100000;

	uint64_t total_size{0};

	for (auto &d : dirs)
	{
		auto size = d->get_total_size();
		if (size <= max_size)
		{
			total_size += size;
		}
	}

	return total_size;
}

uint64_t puzzle_07_2(std::ifstream &in_file)
{
	DirectoryList dirs;
	auto          root = process_input(in_file, dirs);

	constexpr uint64_t total_space     = 70000000;
	constexpr uint64_t required_space  = 30000000;
	const uint64_t     available_space = total_space - root->get_total_size();
	const uint64_t     needed_space    = required_space - available_space;

	uint64_t min_size = total_space;
	for (auto &d : dirs)
	{
		auto size = d->get_total_size();
		if (size >= needed_space)
		{
			min_size = std::min(min_size, size);
		}
	}

	return min_size;
}
}        // namespace

uint64_t puzzle_07(std::ifstream &in_file)
{
	return puzzle_07_2(in_file);
}
