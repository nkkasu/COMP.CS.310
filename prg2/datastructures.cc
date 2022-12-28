#include "datastructures.hh"

#include <random>

#include <cmath>

#include <climits>

std::minstd_rand rand_engine; // Reasonably quick pseudo-random generator

template <typename Type>
Type random_in_range(Type start, Type end)
{
    auto range = end-start;
    ++range;

    auto num = std::uniform_int_distribution<unsigned long int>(0, range-1)(rand_engine);

    return static_cast<Type>(start+num);
}


Datastructures::Datastructures()
{
}

Datastructures::~Datastructures()
{
}

unsigned int Datastructures::town_count()
{
    return towns_by_id_.size();
}

void Datastructures::clear_all()
{
    clear_roads();
    towns_by_id_ = {};

}

bool Datastructures::add_town(TownID id, const Name &name, Coord coord, int tax)
{
    // Find if town already exists.
    if (towns_by_id_.find(id) != towns_by_id_.end())
    {
        return false;
    }

    // Add town to unordered_map.
    Cost cost = {INT_MAX, INT_MAX};
    Town_info town = {id, name, coord, tax, {}, nullptr, {}, WHITE, nullptr, cost};
    towns_by_id_.insert({id, town});
    return true;
}

Name Datastructures::get_town_name(TownID id)
{
    // Find if town exists.
    auto town = towns_by_id_.find(id);
    if (town == towns_by_id_.end())
    {
        return NO_NAME;
    }
    // Return name of existing town.
    Name town_name = town->second.name;
    return town_name;
}

Coord Datastructures::get_town_coordinates(TownID id)
{
    // Find if town exists.
    auto town = towns_by_id_.find(id);
    if (town == towns_by_id_.end())
    {
        return NO_COORD;
    }

    // Return coords of existing town.
    Coord town_coords = town->second.coords;
    return town_coords;
}

int Datastructures::get_town_tax(TownID id)
{
    // Find if town exists.
    auto town = towns_by_id_.find(id);
    if (town == towns_by_id_.end())
    {
        return NO_VALUE;
    }

    // Return tax of existing town.
    int tax = town->second.tax;
    return tax;
}

std::vector<TownID> Datastructures::all_towns()
{
    std::vector<TownID> towns_vector = {};

    // Check if there are towns.
    if (towns_by_id_.empty())
    {
        return towns_vector;
    }
    // Go through the towns in a map. Add TownID to vector.
    for (const auto& town: towns_by_id_)
    {
        towns_vector.push_back(town.first);
    }
    return towns_vector;
}

std::vector<TownID> Datastructures::find_towns(const Name &name)
{
    std::vector<TownID> towns_vector = {};
    // Loop through all the towns.
    for (const auto& town: towns_by_id_)
    {
        // Check if town name is one we are trying to find
        if (town.second.name == name)
        {
            towns_vector.push_back(town.first);
        }
    }
    return towns_vector;
}

bool Datastructures::change_town_name(TownID id, const Name &newname)
{
    // Check if town name is found
    if (towns_by_id_.find(id) != towns_by_id_.end() )
    {
        // Change town name.
        towns_by_id_.at(id).name = newname;
        return true;
    }
    return false;
}

std::vector<TownID> Datastructures::towns_alphabetically()
{
    // Make a vector and reserve space pre-emptively.
    std::vector<TownID> towns = {};
    towns.reserve(towns_by_id_.size());

    // Add all pair elements to vector.
    std::vector<std::pair<TownID, Town_info>> elems(towns_by_id_.begin(), towns_by_id_.end());

    // Sort all elements by town name.
    std::sort(elems.begin(), elems.end(), [] (auto town1, auto town2)
    {return town1.second.name < town2.second.name; });

    for (const auto& town_pair : elems)
    {
        towns.push_back(town_pair.first);
    }
    return towns;
}

std::vector<TownID> Datastructures::towns_distance_increasing()
{
    // Vector for sorted id's and reserve space pre-emptively.
    std::vector<TownID> townid_sorted = {};
    std::vector<std::pair<TownID, int>> town_pairs;

    // Vector in which we sort id's. Reserve space pre-emptively.
    townid_sorted.reserve(towns_by_id_.size());
    town_pairs.reserve(towns_by_id_.size());
    for (auto& town : towns_by_id_)
    {
        int dist = sqrt((town.second.coords.x*town.second.coords.x)+(town.second.coords.y*town.second.coords.y));
        town_pairs.push_back(std::make_pair(town.first, dist));
    }
    std::sort(town_pairs.begin(), town_pairs.end(),[](auto a, auto b){return a.second < b.second;});

    for (const auto& town : town_pairs)
    {
        townid_sorted.push_back(town.first);
    }
    return townid_sorted;
}

TownID Datastructures::min_distance()
{
    if (towns_by_id_.empty())
    {
        return NO_TOWNID;
    }
    // Get distance for one town as "starting  point" to start comparing to.
    Coord xy = towns_by_id_.begin()->second.coords;
    TownID min_dist_town = towns_by_id_.begin()->first;
    // Set min distance to first element.
    int min_dist = sqrt(xy.x*xy.x+xy.y*xy.y);
    // Compare elements
    for (const auto& town : towns_by_id_)
    {

        int x = town.second.coords.x;
        int y = town.second.coords.y;
        int current_dist = sqrt(x*x+y*y);
        if (current_dist < min_dist)
        {
            min_dist = current_dist;
            min_dist_town = town.first;
        }
    }
    return min_dist_town;
}

TownID Datastructures::max_distance()
{
    if (towns_by_id_.empty())
    {
        return NO_TOWNID;
    }
    // Get distance for one town as "starting  point" to start comparing to.
    Coord xy = towns_by_id_.begin()->second.coords;
    TownID min_dist_town = towns_by_id_.begin()->first;
    int min_dist = sqrt(xy.x*xy.x+xy.y*xy.y);
    // Compare for smallest
    for (const auto& i : towns_by_id_)
    {
        int x = i.second.coords.x;
        int y = i.second.coords.y;
        int current_dist = sqrt(x*x+y*y);
        if (current_dist > min_dist)
        {
            min_dist = current_dist;
            min_dist_town = i.first;
        }
    }
    return min_dist_town;
}

bool Datastructures::add_vassalship(TownID vassalid, TownID masterid)
{
    // Find if vassaltown exists.
    auto vassal = towns_by_id_.find(vassalid);
    if (vassal == towns_by_id_.end())
    {
        return false;
    }

    // Find if vassal already has a master (check nullptr)
    if (vassal->second.master)
    {
        return false;
    }
    // Find if mastertown exists.
    auto master = towns_by_id_.find(masterid);
    if (master == towns_by_id_.end())
    {
        return false;
    }

    // Make vassalship
    master->second.vassals.push_back(&vassal->second);
    vassal->second.master = &master->second;
    return true;
    // Replace the line below with your implementation
    // Also uncomment parameters ( /* param */ -> param )

}

std::vector<TownID> Datastructures::get_town_vassals(TownID id)
{
    // Find if town which vassals we are checking exists.
    auto masternode = towns_by_id_.find(id);
    if (masternode == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }

    // Get vassals of the town.
    std::vector<Town_info*> vassals = masternode->second.vassals;
    std::vector<TownID> vassal_ids = {};

    // Check if there are no vassals.
    if (vassals.size() == 0)
    {
        return {};
    }

    // Put vassals in a vector.
    for (Town_info* vassal : vassals)
    {
        vassal_ids.push_back(vassal->id);
    }
    return vassal_ids;
}

std::vector<TownID> Datastructures::taxer_path(TownID id)
{
    // Find if town exists.
    auto vassalnode = towns_by_id_.find(id);
    if (vassalnode == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }
    std::vector<TownID> taxer_town_ids = {};

    // Add vassaltown to vector.
    taxer_town_ids.push_back(vassalnode->second.id);

    // Add masters to a vector until pointer is nullptr.
    Town_info* next_town = vassalnode->second.master;
    while (next_town != nullptr)
    {
        taxer_town_ids.push_back(next_town->id);
        next_town = next_town->master;
    }
    return taxer_town_ids;
}

bool Datastructures::remove_town(TownID id)
{
    // Check if town is found.
    auto pair_to_remove = towns_by_id_.find(id);
    if (pair_to_remove == towns_by_id_.end())
    {
        return false;
    }

    // Node to remove.
    Town_info* node_to_remove = &pair_to_remove->second;
    // Master of node to be removed.
    Town_info* masternode = pair_to_remove->second.master;
    // Vassals of node we want to remove.
    std::vector<Town_info*> vassal_nodes = pair_to_remove->second.vassals;

    // Case when node to remove doesn't have a masternode.
    if (masternode == nullptr)
    {
        // Now all vassalnodes point to null.
        for (Town_info* vassal : vassal_nodes)
        {
            vassal->master = nullptr;
        }
    }
    // Case when node to remove has a master.
    else if (masternode != nullptr)
    {
        // Add new master to vassals.
        for (Town_info* vassal : vassal_nodes)
        {
            vassal->master = masternode;
            masternode->vassals.push_back(vassal);
        }
        // Remove old master from masternodes vassals.
        auto res = std::find(masternode->vassals.begin(),masternode->vassals.end(), node_to_remove);
        masternode->vassals.erase(res);
    }
    // Delete pair.
    pair_to_remove->second.master = nullptr;
    pair_to_remove->second.vassals = {};
    towns_by_id_.erase(pair_to_remove);
    return true;
}

std::vector<TownID> Datastructures::towns_nearest(Coord coord)
{
    // Vector for sorted id's.
    std::vector<TownID> towns_by_distance = {};
    towns_by_distance.reserve(towns_by_id_.size());
    // Copy all pairs into new vector which we then sort.
    std::vector<std::pair<TownID, Town_info>> elems(towns_by_id_.begin(), towns_by_id_.end());
    std::sort(elems.begin(), elems.end(), [this, coord] (auto a, auto b)
    {return get_distance_from_coord(a, coord) < get_distance_from_coord(b, coord);});

    for (const auto& i : elems)
    {
        towns_by_distance.push_back(i.first);
    }
    return towns_by_distance;
}

std::vector<TownID> Datastructures::longest_vassal_path(TownID id)
{
    // Find if town exists
    auto node = towns_by_id_.find(id);
    if (node == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }
    // Find longest vassal path recursively.
    std::vector<TownID> longest = recursive_find_longest(&node->second);

    // Reverse vector elements
    std::reverse(longest.begin(), longest.end());
    return longest;
}

int Datastructures::total_net_tax(TownID id)
{
    // Find if town exists.
    auto node = towns_by_id_.find(id);
    if (node == towns_by_id_.end())
    {
        return NO_VALUE;
    }
    // Calculate total_net_tax recursively.
    int total_net_tax = recursive_total_net_tax(&node->second);
    // Tax paid to master (if exists) isn't calculated and sub-
    // tracted yet. Calculate and subtract it.
    int tax_to_master;
    if (node->second.master != nullptr)
    {
        tax_to_master = total_net_tax * 0.1;
        total_net_tax -= tax_to_master;
    }
    return total_net_tax;
}

int Datastructures::get_distance_from_coord(const std::pair<TownID, Town_info> &town, Coord coord)
{
    // Get x and y of town.
    int x1 = town.second.coords.x;
    int y1 = town.second.coords.y;
    // Get x and y of coord to compare to.
    int x2 = coord.x;
    int y2 = coord.y;
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));

}

std::vector<TownID> Datastructures::recursive_find_longest(Town_info* node)
{
    std::vector<TownID> best;
    // Go to leafs recursively
    for (Town_info* child : node->vassals)
    {
        // Get next vassal's path and compare it to current longest.
        auto next = recursive_find_longest(child);
        if (next.size() > best.size())
        {
            best = std::move(next);
        }
    }
    best.push_back(node->id);
    return best;
}

int Datastructures::recursive_total_net_tax(Town_info* node)
{
    int net_tax = 0;
    // Return tax if vassal doesn't have any more vassals.
    if (node->vassals.empty())
    {
        return node->tax;
    }
    // Otherwise, go through all vassals recursively
    for (Town_info* vassal : node->vassals)
    {
        int net_tax_from_vassals = recursive_total_net_tax(vassal) * 0.1;
        // Sum tax from vassals
        net_tax += net_tax_from_vassals;
    }
    // Overall tax income is income from town and income from vassals.
    int money = net_tax + node->tax;
    return money;
}


//
// Phase 2 operations
//

int Datastructures::get_road_length(Town_info* town1, Town_info* town2)
{
    // Get coordinates of town 1
    int x1 = town1->coords.x;
    int y1 = town1->coords.y;

    // Get coordinates of town 2
    int x2 = town2->coords.x;
    int y2 = town2->coords.y;

    // Now calculate road length.
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

void Datastructures::relax_A(Town_info* u, Town_info* v, Town_info* g)
{
    // Calculate new cost estimates for A* algorithm.
    // updates pi pointers if better route is found. Also updates
    // distance estimates.
    if (v->cost.d > (u->cost.d + get_road_length(u, v)) )
    {
        v->cost.d = (u->cost.d + get_road_length(u, v));
        v->cost.de = (v->cost.d + min_est(v, g));
        v->pi = u;
    }
}

int Datastructures::min_est(Town_info* v, Town_info* g)
{
    // Calculates minimum estimate for road length. Calculation is made
    // by using straight line from current town v to goal town g.
    return get_road_length(v, g);
}

void Datastructures::clear_roads()
{
    // Goes through all the towns.
    for (auto& town : towns_by_id_)
    {
        // Goes through roads of town.
        for (auto& road : town.second.roads_to)
        {
            // Make ptr null just in case
            road = nullptr;
        }
        // Empty roads vector.
        town.second.roads_to = {};
    }
    // Empty another data structure.
    roads_ = {};
}

std::vector<std::pair<TownID, TownID>> Datastructures::all_roads()
{
    // Get roads from different data structure.
    return roads_;
}

bool Datastructures::add_road(TownID town1, TownID town2)
{
    // Find if town1 exists.
    auto town1_node = towns_by_id_.find(town1);
    if (town1_node == towns_by_id_.end())
    {
        return false;
    }
    // Find if town2 exists.
    auto town2_node = towns_by_id_.find(town2);
    if (town2_node == towns_by_id_.end())
    {
        return false;
    }
    // Check if adding road to town itself. This is not wanted.
    if (town1 == town2)
    {
        return false;
    }
    // Lastly we have to check if road already exists. If it does, we don't add.
    for (auto& road_to_town : town1_node->second.roads_to)
    {
        if (road_to_town == &town2_node->second)
        {
            return false;
        }
    }
    std::pair<TownID, TownID> road;

    // Add roads to both ways.
    town1_node->second.roads_to.push_back(&town2_node->second);
    town2_node->second.roads_to.push_back(&town1_node->second);

    // Add to different data structure depending on ID
    if (town1_node->second.id < town2_node->second.id)
    {
        road.first = town1_node->second.id;
        road.second = town2_node->second.id;
    }
    else
    {
        // Add in reverse order
        road.first = town2_node->second.id;
        road.second = town1_node->second.id;
    }
    roads_.push_back(road);

    return true;
}

std::vector<TownID> Datastructures::get_roads_from(TownID id)
{
    // Get if town is found.
    auto town_node = towns_by_id_.find(id);
    if (town_node == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }
    // Now we go through the roads!
    std::vector<TownID> all_of_roads = {};

    // Reserve for roads.
    all_of_roads.reserve(town_node->second.roads_to.size());

    // Go through roads.
    for (auto& road : town_node->second.roads_to)
    {
        all_of_roads.push_back(road->id);
    }
    return all_of_roads;
}

std::vector<TownID> Datastructures::any_route(TownID fromid, TownID toid)
{
    return least_towns_route(fromid, toid);
}

bool Datastructures::remove_road(TownID town1, TownID town2)
{
    // Check if towns exist.
    auto town1_node = towns_by_id_.find(town1);
    if (town1_node == towns_by_id_.end())
    {
        return false;
    }
    // Find if town2 exists.
    auto town2_node = towns_by_id_.find(town2);
    if (town2_node == towns_by_id_.end())
    {
        return false;
    }

    bool is_road_found = false;
    int at_index = 0;
    // Find if road exists and remove it
    std::vector<Town_info*>* vector = &town1_node->second.roads_to;
    for (auto& to_town : town1_node->second.roads_to)
    {
        if (to_town == &town2_node->second)
        {
            vector->erase(vector->begin() + at_index);
            is_road_found = true;
        }
        at_index++;
    }
    at_index = 0;
    vector = &town2_node->second.roads_to;
    // Do the same to different town.
    for (auto& to_town : town2_node->second.roads_to)
    {
        if (to_town == &town1_node->second)
        {
            vector->erase(vector->begin() + at_index);
        }
        at_index++;
    }
    at_index = 0;
    for (auto& road : roads_)
    {
        if (road.first == town1 && road.second == town2)
        {
            roads_.erase(roads_.begin() + at_index);
        }
        if (road.first == town2 && road.second == town1)
        {
            roads_.erase(roads_.begin() + at_index);
        }
        at_index++;
    }

    return is_road_found;
}

std::vector<TownID> Datastructures::least_towns_route(TownID fromid, TownID toid)
{
    // Check if towns exist.
    auto town1_node = towns_by_id_.find(fromid);
    auto town2_node = towns_by_id_.find(toid);

    if (town1_node == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }
    if (town2_node == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }

    // Initialize nodes.
    for (auto& town : towns_by_id_)
    {
        town.second.colour = WHITE;
        town.second.pi = nullptr;
    }

    // Queue for town nodes.
    std::queue<Town_info*> town_queue;

    // Vector for TownID.
    std::vector<TownID> route;

    // First node
    Town_info* starting_node = &towns_by_id_.at(fromid);

    // We start processing first node. Push it to queue as well.
    starting_node->colour = GRAY;
    town_queue.push(starting_node);
    Town_info* current_node;
    while (!town_queue.empty())
    {
        // Top element. Get and pop.
        current_node = town_queue.front();
        town_queue.pop();
        for (auto& road_to : current_node->roads_to)
        {
            // Check if visited or not.
            if (road_to->colour == WHITE)
            {
                // Mark visited and update pi
                road_to->colour = GRAY;
                road_to->pi = current_node;
                town_queue.push(road_to);
            }
        }
        current_node->colour = BLACK;
    }
    // End node not reached. Cant find a route.
    if (town2_node->second.colour == WHITE)
    {
        return {};
    }

    // Loop through pi pointers until we get to starting point.
    Town_info* current = &town2_node->second;
    while (current != nullptr)
    {
        // Add them to final route
        route.push_back(current->id);
        current = current->pi;
    }
    // Reverse route.
    std::reverse(route.begin(), route.end());
    return route;
}

std::vector<TownID> Datastructures::road_cycle_route(TownID startid)
{
    // Get first node.
    auto start_node = towns_by_id_.find(startid);
    if (start_node == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }

    // Initialize all towns.
    for (auto& town : towns_by_id_)
    {
        town.second.colour = WHITE;
        town.second.pi = nullptr;
    }

    // Storing cycle information.
    std::vector<Town_info*> cycle_road;
    std::vector<TownID> cycle_id;
    Town_info* current;

    // Stack for road processing.
    std::stack<Town_info*> road_stack = {};
    bool cycle_found = false;
    // Push first to stack
    road_stack.push(&start_node->second);
    Town_info* loop_node = nullptr;
    std::vector<Town_info*> test;

    // Now we can start the DFS
    while (!road_stack.empty())
    {
        if (cycle_found == true)
        {
            break;
        }
        // Top of the stack.
        current = road_stack.top();
        road_stack.pop();
        // Push back elements to different vector.
        cycle_road.push_back(current);

        if (current->colour == WHITE)
        {
            road_stack.push(current);
            current->colour = GRAY;
            // Go through roads of a town.
            for (auto& road_to_town : current->roads_to)
            {
                // Is town visited?
                if (road_to_town->colour == WHITE)
                {
                    road_to_town->pi = current;
                    road_stack.push(road_to_town);
                }
                // Continue if last town.
                if (road_to_town == current->pi)
                {
                    continue;
                }
                // We are in the "loop"
                else if (road_to_town->colour == GRAY)
                {
                    // But only depends on current pi.
                    if (road_to_town->pi == current)
                    {
                        continue;
                    }
                    // Adding elements and breaking out.
                    loop_node = road_to_town;
                    cycle_road.push_back(road_to_town);
                    cycle_id.push_back(loop_node->id);
                    cycle_found = true;
                    break;
                }
            }
        }
        else
        {
            cycle_road.pop_back();
            current->colour = BLACK;
        }
    }

    // If cycle is not found, then return empty.
    if (cycle_found == false)
    {
        return {};
    }

    // Get the loop route.
    Town_info* loop_elem = cycle_road.at(cycle_road.size() - 2);
    while (loop_elem != nullptr)
    {
        cycle_id.push_back(loop_elem->id);
        loop_elem = loop_elem->pi;
    }
    std::reverse(cycle_id.begin(), cycle_id.end());
    return cycle_id;

}


std::vector<TownID> Datastructures::shortest_route(TownID fromid, TownID toid)
{
    // Get start node and last node.
    auto start_node = towns_by_id_.find(fromid);
    auto last_node = towns_by_id_.find(toid);

    // Check if start node and last node are found.
    if (start_node == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }
    if (last_node == towns_by_id_.end())
    {
        return {NO_TOWNID};
    }

    // Initialize variables.
    for (auto& town : towns_by_id_)
    {
        town.second.colour = WHITE;
        town.second.pi = nullptr;
        town.second.cost.d = INT_MAX;
        town.second.cost.de = INT_MAX;
    }
    // Final route.
    std::vector<TownID> route = {};
    // Bool to check if node is found.
    bool node_found = false;
    // Priority queue for A* algorithm.
    std::priority_queue<std::pair<int, Town_info*>> town_queue;
    // Start from the start node. Mark it gray and distance to 0.
    start_node->second.colour = GRAY;
    start_node->second.cost.d = 0;
    // Push starting town to the priority queue.
    town_queue.push({start_node->second.cost.d, &start_node->second});
    // For the current pair in queue.
    std::pair<int, Town_info*> current;
    while (!town_queue.empty())
    {
        // Get cheapest road from priority queue.
        current = town_queue.top();
        town_queue.pop();
        if (current.second->colour == BLACK)
        {
            // Skip if BLACK. No need to process.
            continue;
        }
        // If end node was found, quit.
        if (current.second->id == toid)
        {
            node_found = true;
            break;
        }
        // Go through roads of town.
        for (auto& road_to_town : current.second->roads_to)
        {
            // Make a new estimate.
            relax_A(current.second, road_to_town, &last_node->second);
            // If node is new, add to priority queue.
            if (road_to_town->colour == WHITE)
            {
                road_to_town->colour = GRAY;
                // Negative because c++ priority queue takes biggest value.
                town_queue.push({-(road_to_town->cost.de), road_to_town});
            }
            else
            {
                // Old node, we push it back to queue.
                town_queue.push({-(road_to_town->cost.de), road_to_town});
            }
        }
        current.second->colour = BLACK;
    }

    Town_info* route_iter = &last_node->second;
    while (route_iter != nullptr)
    {
        route.push_back(route_iter->id);
        route_iter = route_iter->pi;
    }

    if (node_found == false)
    {
        return {};
    }
    std::reverse(route.begin(), route.end());
    return route;
}

Distance Datastructures::trim_road_network()
{
    // Replace the line below with your implementation
    throw NotImplemented("trim_road_network()");
}
