#include "Puzzles.hpp"

namespace
{
struct Pos
{
	int32_t x{0};
	int32_t y{0};

	bool operator==(const Pos &other) const
	{
		return x == other.x &&
		       y == other.y;
	}
};

struct Node
{
	Pos      pos{};
	int      heat{0};
	bool     visited;
	uint32_t min_distance{std::numeric_limits<uint32_t>::max()};
	Node    *prev_node{nullptr};
	int      same_direction_count{0};
};

using Grid = std::unordered_map<uint64_t, Node>;

struct HeatMap
{
	Grid     grid;
	uint32_t width{0};
	uint32_t height{0};
};

inline uint64_t key(uint32_t x, uint32_t y)
{
	return static_cast<uint64_t>(x) << 32 | y;
}

inline uint64_t key(Pos pos)
{
	return key(static_cast<uint32_t>(pos.x), static_cast<uint32_t>(pos.y));
}

enum Direction : uint16_t
{
	Right = 0,
	Down  = 1,
	Left  = 2,
	Up    = 3,
};

struct NodeState
{
	Pos       pos{};
	Direction dir{};
	int       same_direction_count{0};

	bool operator==(const NodeState &other) const
	{
		return pos == other.pos &&
		       dir == other.dir &&
		       same_direction_count == other.same_direction_count;
	}
};

struct NodeStateHasher
{
	std::size_t operator()(const NodeState &n) const
	{
		using std::hash;
		using std::size_t;
		using std::string;

		return ((hash<string>()(std::to_string(key(n.pos))) ^ (hash<int>()(n.dir) << 1)) >> 1) ^
		       (hash<int>()(n.same_direction_count) << 1);
	}
};

uint64_t puzzle_17_1(std::ifstream &in_file)
{
	std::string line;

	HeatMap heatmap;

	while (std::getline(in_file, line))
	{
		if (line.size() > 0)
		{
			if (0 == heatmap.width)
			{
				heatmap.width = line.size();
			}

			for (int32_t i = 0; i < heatmap.width; i++)
			{
				Pos pos{i, static_cast<int32_t>(heatmap.height)};
				heatmap.grid[key(pos)] = {pos, line[i] - '0'};
			}

			heatmap.height++;
		}
	}

	auto &source        = heatmap.grid[key(0, 0)];
	source.min_distance = 0;

	std::vector<Node *> nodes_with_distance;
	nodes_with_distance.push_back(&source);

	std::unordered_map<NodeState, bool, NodeStateHasher> visited;

	while (!nodes_with_distance.empty())
	{
		uint32_t min_distance{std::numeric_limits<uint32_t>::max()};
		uint32_t min_index;
		for (int v = 0; v < nodes_with_distance.size(); v++)
		{
			auto node = nodes_with_distance[v];
			if (!node->visited && node->min_distance < min_distance)
			{
				min_distance = node->min_distance;
				min_index    = v;
			}
		}

		auto node = nodes_with_distance[min_index];
		// node->visited = true;

		NodeState state;
		state.pos                  = node->pos;
		state.same_direction_count = node->same_direction_count + 1;

		std::cout << "Processing node at (" << node->pos.x << ", " << node->pos.y << ")" << std::endl;

		Node *node_up{nullptr};
		Node *node_down{nullptr};
		Node *node_left{nullptr};
		Node *node_right{nullptr};

		if (node->pos.x - 1 >= 0)
		{
			node_left = &heatmap.grid[key(node->pos.x - 1, node->pos.y)];
		}
		if (node->pos.x + 1 < heatmap.width)
		{
			node_right = &heatmap.grid[key(node->pos.x + 1, node->pos.y)];
		}
		if (node->pos.y - 1 >= 0)
		{
			node_up = &heatmap.grid[key(node->pos.x, node->pos.y - 1)];
		}
		if (node->pos.y + 1 < heatmap.height)
		{
			node_down = &heatmap.grid[key(node->pos.x, node->pos.y + 1)];
		}

		// Define 'forward' as left-to-right in x-axis
		Node *forward_node{nullptr};
		Node *left_node{nullptr};
		Node *right_node{nullptr};

		if (!node->prev_node || node->prev_node->pos.y == node->pos.y)
		{
			// Coming from left/right (default at start)
			bool coming_from_left = node->prev_node ? node->prev_node->pos.x < node->pos.x : true;

			left_node    = coming_from_left ? node_up : node_down;
			forward_node = coming_from_left ? node_right : node_left;
			right_node   = coming_from_left ? node_down : node_up;

			state.dir = coming_from_left ? Direction::Right : Direction::Left;
		}
		else if (node->prev_node->pos.x == node->pos.x)
		{
			// Coming from above/below
			bool coming_from_above = node->prev_node->pos.y < node->pos.y;

			left_node    = coming_from_above ? node_right : node_left;
			forward_node = coming_from_above ? node_down : node_up;
			right_node   = coming_from_above ? node_left : node_right;

			state.dir = coming_from_above ? Direction::Down : Direction::Up;
		}

		std::vector<Node *> connected;
		connected.push_back(forward_node);
		connected.push_back(left_node);
		connected.push_back(right_node);
		for (auto n : connected)
		{
			if (n && !visited.count({n->pos, state.dir, n->same_direction_count}) &&
			    node->min_distance != std::numeric_limits<uint32_t>::max())
			{
				bool smaller_distance = node->min_distance + n->heat < n->min_distance;
				if (smaller_distance && state.same_direction_count < 4)
				{
					n->min_distance         = node->min_distance + n->heat;
					n->prev_node            = node;
					n->same_direction_count = state.same_direction_count;

					auto it = std::find(nodes_with_distance.begin(), nodes_with_distance.end(), n);
					if (it == nodes_with_distance.end())
					{
						std::cout << "\t\tAdd node at (" << n->pos.x << ", " << n->pos.y << ")" << std::endl;
						nodes_with_distance.push_back(n);
					}
				}
			}
		}

		/*
		        for (auto n : connected)
		        {
		            if (n)
		            {
		                std::cout << "\tLooking at (" << n->pos.x << ", " << n->pos.y << ") - "
		                          << node->min_distance << " + " << n->heat << " < " << n->min_distance << "?" << std::endl;
		            }

		            if (n && !n->visited && node->min_distance != std::numeric_limits<uint32_t>::max())
		            {
		                bool two_consecutive = (node->prev_node &&
		                                        n->pos.x == node->prev_node->pos.x) ||
		                                       (node->prev_node &&
		                                        n->pos.y == node->prev_node->pos.y);

		                bool three_consecutive = two_consecutive &&
		                                         ((node->prev_node->prev_node &&
		                                           n->pos.x == node->prev_node->prev_node->pos.x) ||
		                                          (node->prev_node->prev_node &&
		                                           n->pos.y == node->prev_node->prev_node->pos.y));

		                bool four_consecutive = three_consecutive &&
		                                        ((node->prev_node->prev_node->prev_node &&
		                                          n->pos.x == node->prev_node->prev_node->prev_node->pos.x) ||
		                                         (node->prev_node->prev_node->prev_node &&
		                                          n->pos.y == node->prev_node->prev_node->prev_node->pos.y));

		                bool smaller_distance          = node->min_distance + n->heat < n->min_distance;
		                bool equal_or_smaller_distance = node->min_distance + n->heat <= n->min_distance;

		                if (((smaller_distance && !four_consecutive) ||
		                     (equal_or_smaller_distance && !three_consecutive) ||
		                     (equal_or_smaller_distance && !two_consecutive)))
		                {
		                    n->min_distance = node->min_distance + n->heat;
		                    n->prev_node    = node;

		                    auto it = std::find(nodes_with_distance.begin(), nodes_with_distance.end(), n);
		                    if (it == nodes_with_distance.end())
		                    {
		                        std::cout << "\t\tAdd node at (" << n->pos.x << ", " << n->pos.y << ")" << std::endl;
		                        nodes_with_distance.push_back(n);
		                    }
		                }
		                else
		                {
		                    std::cout << "\t\tfour_consecutive" << std::endl;
		                }
		            }
		            else
		            {
		                if (n && n->visited)
		                {
		                    std::cout << "\t\tAlready visited" << std::endl;
		                }
		            }
		        }

		*/

		visited[state] = true;

		nodes_with_distance.erase(nodes_with_distance.begin() + min_index);
	}

	auto node         = &heatmap.grid[key(heatmap.width - 1, heatmap.height - 1)];
	auto min_distance = node->min_distance;

	while (node->prev_node)
	{
		std::cout << node->prev_node->heat << std::endl;
		node = node->prev_node;
	}

	return min_distance;
}

uint64_t puzzle_17_2(std::ifstream &in_file)
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

uint64_t puzzle_17(std::ifstream &in_file)
{
	return puzzle_17_1(in_file);
}
