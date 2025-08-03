#include <iostream>
#include <vector>
using namespace std;


enum class Direction
{
    IDLE,
    UP,
    DOWN
};

class Elevator
{
    int capacity;
    int curr; // current number of people
    int curr_floor;
    Direction d;

public:
    Elevator(int cap = 5) : capacity(cap), curr(0), curr_floor(0), d(Direction::IDLE) {}

    Direction getDirection() const
    {
        return d;
    }

    bool isFull() const
    {
        return curr >= capacity;
    }

    void addUser()
    {
        if (!isFull())
            curr++;
    }

    void removeUser()
    {
        if (curr > 0)
            curr--;
        if (curr == 0)
            d = Direction::IDLE;
    }

    bool isPossible(Direction reqDir, int start, int end) const
    {
        if (isFull())
            return false;

        if (d == Direction::IDLE)
            return true;

        if (d == Direction::UP && reqDir == Direction::UP && curr_floor <= start)
            return true;

        if (d == Direction::DOWN && reqDir == Direction::DOWN && curr_floor >= start)
            return true;

        return false;
    }

    void moveToFloor(int floor)
    {
        curr_floor = floor;
    }

    int getCurrentFloor() const
    {
        return curr_floor;
    }
};

class ElevatorSystem
{
    vector<Elevator *> elevators;
    int min_floor, max_floor;

public:
    ElevatorSystem(int numElevators, int capacity = 5, int minF = 0, int maxF = 10)
        : min_floor(minF), max_floor(maxF)
    {
        for (int i = 0; i < numElevators; ++i)
            elevators.push_back(new Elevator(capacity));
    }

    bool request(int start, int end)
    {
        Direction reqDir = (start < end) ? Direction::UP : Direction::DOWN;

        for (Elevator *e : elevators)
        {
            if (e->isPossible(reqDir, start, end))
            {
                e->addUser();
                e->moveToFloor(end);// This is very important , can be used better
                cout << "Request served by elevator at floor " << e->getCurrentFloor() << "\n";
                return true;
            }
        }

        cout << "No elevator available for request from " << start << " to " << end << "\n";
        return false;
    }

    ~ElevatorSystem()
    {
        for (Elevator *e : elevators)
            delete e;
    }
};

int main()
{
    ElevatorSystem system(3); // 3 elevators

    cout << "Request: Floor 2 -> Floor 7\n";
    system.request(2, 7);

    cout << "Request: Floor 4 -> Floor 1\n";
    system.request(4, 1);

    cout << "Request: Floor 6 -> Floor 9\n";
    system.request(6, 9);

    cout << "Request: Floor 3 -> Floor 5\n";
    system.request(3, 5); // Might fail if elevators are full

    return 0;
}
