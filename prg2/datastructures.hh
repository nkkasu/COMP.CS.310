// Datastructures.hh

#ifndef DATASTRUCTURES_HH
#define DATASTRUCTURES_HH

#include <string>
#include <vector>
#include <tuple>
#include <utility>
#include <limits>
#include <functional>
#include <exception>
#include <set>
#include <queue>
#include <stack>

// Types for IDs
using TownID = std::string;
using Name = std::string;

// Return values for cases where required thing was not found
TownID const NO_TOWNID = "----------";

// Return value for cases where integer values were not found
int const NO_VALUE = std::numeric_limits<int>::min();

// Return value for cases where name values were not found
Name const NO_NAME = "!!NO_NAME!!";

// Type for a coordinate (x, y)
struct Coord
{
    int x = NO_VALUE;
    int y = NO_VALUE;
};

enum Colour { WHITE, GRAY, BLACK };

struct Cost
{
    int d;
    int de;
};
struct Town_info
{
    TownID id{};
    Name name{};
    Coord coords{};
    int tax{};

    std::vector<Town_info*> vassals{};
    Town_info* master{};

    std::vector<Town_info*> roads_to{};
    Colour colour;
    Town_info* pi{};
    Cost cost;
};

// Example: Defining == and hash function for Coord so that it can be used
// as key for std::unordered_map/set, if needed
inline bool operator==(Coord c1, Coord c2) { return c1.x == c2.x && c1.y == c2.y; }
inline bool operator!=(Coord c1, Coord c2) { return !(c1==c2); } // Not strictly necessary

struct CoordHash
{
    std::size_t operator()(Coord xy) const
    {
        auto hasher = std::hash<int>();
        auto xhash = hasher(xy.x);
        auto yhash = hasher(xy.y);
        // Combine hash values (magic!)
        return xhash ^ (yhash + 0x9e3779b9 + (xhash << 6) + (xhash >> 2));
    }
};

// Example: Defining < for Coord so that it can be used
// as key for std::map/set
inline bool operator<(Coord c1, Coord c2)
{
    if (c1.y < c2.y) { return true; }
    else if (c2.y < c1.y) { return false; }
    else { return c1.x < c2.x; }
}

// Return value for cases where coordinates were not found
Coord const NO_COORD = {NO_VALUE, NO_VALUE};

// Type for a distance (in metres)
using Distance = int;

// Return value for cases where Distance is unknown
Distance const NO_DISTANCE = NO_VALUE;

// This exception class is there just so that the user interface can notify
// about operations which are not (yet) implemented
class NotImplemented : public std::exception
{
public:
    NotImplemented() : msg_{} {}
    explicit NotImplemented(std::string const& msg) : msg_{msg + " not implemented"} {}

    virtual const char* what() const noexcept override
    {
        return msg_.c_str();
    }
private:
    std::string msg_;
};


// This is the class you are supposed to implement

class Datastructures
{
public:
    Datastructures();
    ~Datastructures();

    // Estimate of performance: Constant
    // Short rationale for estimate: According to documentation std::unordered_map::size
    // is constant (cppreference).
    unsigned int town_count();

    // Estimate of performance: Linear in the size N of container.
    // Short rationale for estimate: Deleting all elements from unordered_map with
    // assignment = {}; is linear in the size of container. N objects have to be removed
    // from the container. Linear similarly to unordered_map::clear.
    void clear_all();

    // Estimate of performance: Constant on average.
    // Short rationale for estimate: We can assume creating a struct always takes a
    // constant time. Unordered_map::find operator is constant in average according to
    // cppreference.
    bool add_town(TownID id, Name const& name, Coord coord, int tax);

    // Estimate of performance: Constant on average.
    // Short rationale for estimate: Unordered_map::find operator is constant in
    // average according to cppreference. Getting struct.name is constant, therefore
    // overall constant on average.
    Name get_town_name(TownID id);

    // Estimate of performance: Constant on average.
    // Short rationale for estimate: Unordered_map::find operator is constant in
    // average according to cppreference. Getting struct.coords is constant, therefore
    // overall constant on average.
    Coord get_town_coordinates(TownID id);

    // Estimate of performance: Constant on average.
    // Short rationale for estimate: Unordered_map::find operator is constant in
    // average according to cppreference. Getting struct.tax is constant, therefore
    // overall constant on average.
    int get_town_tax(TownID id);

    // Estimate of performance: Linear on the size N of container.
    // Short rationale for estimate: vector::push_back is constant operation since we
    // allocate enough memory for all elements in vector. Push_back is done N times in a loop
    // on the size N of a container, therefore performance is linear on the size of container.
    std::vector<TownID> all_towns();

    // Estimate of performance: Linear on the size N of the container.
    // Short rationale for estimate: Goes through all the elements of container and
    // compares each element to the name of the town. If town name is found, push_back
    // into a vector. Since we loop N times, operation is at least linear. Since push_back
    // is amortized constant, we can assume operation is linear on the size N of container.
    std::vector<TownID> find_towns(Name const& name);

    // Estimate of performance: Constant on average.
    // Short rationale for estimate: unordered_map::at and unordered_map::find operations
    // are both constant on average according to documentation (cppreference). Therefore
    // operation is constant on average.
    bool change_town_name(TownID id, Name const& newname);

    // Estimate of performance: Worst case N*log(N), where N is first-last comparisons.
    // Short rationale for estimate: std::sort is at worst (N*log(N)) according to
    // cppreference, where N is range of container (first-last comparisons).
    // push_back is constant, therefore push_backing in a loop is linear on
    // average. Copying unordered map to vector is linear in the size of container.
    // Therefore for worst asymptotically N*log(N).
    std::vector<TownID> towns_alphabetically();

    // Estimate of performance: Worst case N*log(N), where N is first-last comparisons.
    // Short rationale for estimate: We can assume cmath operation sqrt is fairly close
    // to constant, especially when we work with fairly small numbers in the program.
    // std::sort is at worst (N*log(N)) according to cppreference, where N is range of
    // container (first-last comparisons). Push_back is constant and in a loop it is
    // adding all elements, so adding all elements in a vector is linear overall.
    // Therefore asymptotically N*log(N)
    std::vector<TownID> towns_distance_increasing();

    // Estimate of performance: Linear on average, where N is size of container.
    // Short rationale for estimate: Goes through a container of size N in a for loop.
    // Does if-comparisons which are constant in time complexity. Also assigns values
    // which is constant.
    TownID min_distance();

    // Estimate of performance: Linear on average, where N is size of container.
    // Short rationale for estimate: Goes through a container of size N in a for loop.
    // Does if-comparisons which are constant in time complexity. Also assigns values
    // which is constant.
    TownID max_distance();

    // Estimate of performance: Constant.
    // Short rationale for estimate: unordered_map::find is on average constant
    // operation. Push_back is amortized constant, since we dont reserve memory.
    // If conditions are constant. Overall constant.
    bool add_vassalship(TownID vassalid, TownID masterid);

    // Estimate of performance: Worst case linear in the size N of vassal container.
    // Best case constant, no town found.
    // Short rationale for estimate: Best case: No town found, unordered_map::find
    // is linear. Worst case: town and vassals are found. Looping N vassals and pushing
    // back in a vector. Amortized linear.
    std::vector<TownID> get_town_vassals(TownID id);

    // Estimate of performance: Worst case linear in vassal towns N found.
    // Best case constant, town doesnt exist.
    // Short rationale for estimate: Best case: No town found, unordered_map::find
    // is constant. Worst case: town is found. Looping in a while loop until
    // nullptr is found. Done N times and pushed back into a vector. Linear
    // operation overall.
    std::vector<TownID> taxer_path(TownID id);


    // Estimate of performance: Worst case: Linear in N vassals of old master)
    // Best case constant, towns dont exist.
    // Short rationale for estimate: Best case: No town found, unordered_map::find
    // is constant. Worst case: Both towns exist. Looping is linear operation.
    // updating vassal's master is constant. Overall linear. Push_back constant.
    bool remove_town(TownID id);

    // Estimate of performance: Worst case: N*log(N), best case constant, N is first-last elements
    // Short rationale for estimate: Worst case: std::sort is N*log(N) in time
    // complexity, where N is first-last elements, according to cppreference.
    std::vector<TownID> towns_nearest(Coord coord);

    // Estimate of performance: Linear in the total of towns N called recursively.
    // Short rationale for estimate: Goes through all vassals, subvassals etc. of a node
    // recursively. Visits all N vassalnodes once, therefore linear in the total of towns
    // as vassals or subvassals. Operations inside recursion are constant in time complexity
    // so overall, operation is linear.
    std::vector<TownID> longest_vassal_path(TownID id);

    // Estimate of performance: Linear in the total of towns N called recursively.
    // Short rationale for estimate: Goes through all vassals, subvassals etc of a node
    // recursively. Visits all N vassalnodes once, therefore operation is linear in the total
    // of vassals called recursively. Inside recursion operations are constant, so
    // overall operation is linear.
    int total_net_tax(TownID id);


    // Phase 2 operations

    // Estimate of performance: O(N*K), where N is number of
    // roads. Best case constant, when towns_by_id_ is empty.
    // Short rationale for estimate: We have to remove
    // K roads from N towns. Therefore, we have to do O(N*K)
    // operations. Case if we have towns but no roads is
    // O(N). Case if we have empty data structure is Omega(1)
    void clear_roads();

    // Estimate of performance: Constant on average.
    // Short rationale for estimate: Only returns a vector.
    // Returning a vector is constant time
    std::vector<std::pair<TownID, TownID>> all_roads();

    // Estimate of performance: O(N), where N is amount of roads.
    // Short rationale for estimate: Best case is constant time,
    // since if town doesn't exist we return. unordered_map find
    // is constant time. Most demanding operation
    // in time complexity in function is looping through all
    // town's roads. Looping is O(N) if road doesn't exist.
    // Vector push_back is constant. Therefore, O(N)
    bool add_road(TownID town1, TownID town2);

    // Estimate of performance: Best case constant, worst
    // case O(N) where N number of roads (of town).
    // Short rationale for estimate: Best case constant if
    // town is not found. Worst case is linear in N, because we
    // have to push_back N towns which we then return.
    std::vector<TownID> get_roads_from(TownID id);

    // Estimate of performance: Returns least_towns_route,
    // Which is O(N+K) at worst, where N roads and K edges.
    // Short rationale for estimate: Check least towns route.
    // function returns from least_towns_route.
    std::vector<TownID> any_route(TownID fromid, TownID toid);

    // Non-compulsory phase 2 operations

    // Estimate of performance: Best case constant. Worst case
    // O(N)
    // Short rationale for estimate: Best case is when town is
    // not found. Unordered_map is average then. Worst case is
    // when both towns are found. We loop 2 times, checking for
    // existing road. If it exists, both operations are separate
    // and O(N). Lastly we loop all roads K from another data structure
    // and remove it. Efficiency is based on assumption that there are
    // always more towns than roads.
    bool remove_road(TownID town1, TownID town2);

    // Estimate of performance: O(N+K) BFS. Best case constant.
    // is O(N+K), where N is number of nodes and K is number of edges.
    // Short rationale for estimate: We at worst have to visit all edges
    // and all nodes. Queue pop and queue push are constant in time.
    // Therefore, at worst O(N+K). BFS is O(N+K) anyways according
    // to documentations. Best case: unordereded map find didnt find.
    std::vector<TownID> least_towns_route(TownID fromid, TownID toid);

    // Estimate of performance: Algorithm is based on DFS. DFS at worst
    // is O(N+K), where N is number of nodes and K number of edges.
    // Short rationale for estimate: Removing from stack is constant time,
    // as well as pushing. We push and pop to stack in loops loop.
    // This is why operation is at worst O(N+K). We at worst have to visit
    // all N nodes and all K edges.
    std::vector<TownID> road_cycle_route(TownID startid);

    // Estimate of performance: Algorithm is based on A*-algorithm.
    // Worst case situation for A* is O((N+K)log(N+K)), where N
    // is number of nodes and K is number of edges.
    // Short rationale for estimate: A* is based on Dijsktra, which
    // is O((N+K)log(N+K)) at worst. A* works a little bit more efficiently
    // compared to Dijkstra at best case though, but it highly depends. Log comes from
    // priority_queue.top() and push being O(logN). At worst N nodes and K
    // edges have to be visited.
    std::vector<TownID> shortest_route(TownID fromid, TownID toid);

    // Estimate of performance:
    // Short rationale for estimate:
    Distance trim_road_network();

private:

    int get_distance_from_coord(std::pair<TownID, Town_info> const &town, Coord coord);

    std::vector<TownID> recursive_find_longest(Town_info*);

    int recursive_total_net_tax(Town_info* node);

    std::unordered_map<TownID, Town_info> towns_by_id_;

    std::vector<std::pair<TownID, TownID>> roads_;

    int get_road_length(Town_info*, Town_info*);

    void relax_A(Town_info*, Town_info*, Town_info*);

    int min_est(Town_info*, Town_info*);
};

#endif // DATASTRUCTURES_HH
