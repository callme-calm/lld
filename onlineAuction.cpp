#include <bits/stdc++.h>
using namespace std;

class Observer
{
public:
    virtual ~Observer() {}
    virtual void notify(const string &msg) = 0;
};

class User : public Observer
{
    string name;
    int id;
    int pass;

public:
    User(int id, string name, int pass)
    {
        this->id = id;
        this->name = name;
        this->pass = pass;
    }

    bool login(int pass)
    {
        return this->pass == pass;
    }

    int getId()
    {
        return this->id;
    }

    void notify(const string &msg)
    {
        cout << "User " << id << " notified: " << msg << endl;
    }
};

class Listing
{
    string name;
    string description;
    int duration; // seconds
public:
    Listing(string name, string description, int duration)
        : name(name), description(description), duration(duration) {}

    string getName()
    {
        return name;
    }

    string getDescription()
    {
        return description;
    }

    int getDuration()
    {
        return duration;
    }
};

class System
{
    vector<User *> users;
    vector<Listing *> listings;

    int current_user;
    int current_amount;
    Listing *current_listing;
    int start_time;

public:
    static int current_time;
    mutex mtx;

    System()
    {
        current_user = -1;
        current_amount = 0;
        current_listing = nullptr;
        start_time = 0;
    }

    bool login(int id, int pass)
    {
        for (auto u : users)
        {
            if (u->getId() == id && u->login(pass))
                return true;
        }
        return false;
    }

    void start(Listing *l)
    {
        current_listing = l;
        start_time = current_time;
        cout << "Auction for " << l->getName() << " started\n";
    }

    void addListing(Listing *l)
    {
        listings.push_back(l);
    }

    void addUser(User *u)
    {
        users.push_back(u);
    }

    void addBid(User *u, int amount)
    {
        mtx.lock();
        if (current_listing == nullptr)
        {
            cout << "No auction running!\n";
            mtx.unlock();
            return;
        }

        if (amount > current_amount)
        {
            current_amount = amount;
            current_user = u->getId();
            cout << "Bid accepted: User " << current_user << " bid " << current_amount << endl;
            notifyAll("New highest bid: " + to_string(current_amount));
        }
        else
        {
            cout << "Bid too low!\n";
        }

        check_auction();
        mtx.unlock();
    }

    void check_auction()
    {
        if (current_time - start_time >= current_listing->getDuration())
        {
            getResult();
            current_listing = nullptr;
        }
    }

    void getResult()
    {
        if (current_user != -1)
            cout << "User " << current_user << " won bid, amount " << current_amount << endl;
        else
            cout << "No bids placed.\n";
    }

    void notifyAll(const string &msg)
    {
        for (auto u : users)
        {
            u->notify(msg);
        }
    }
};

int System::current_time = 0;

int main()
{
    User *u1 = new User(1, "Alice", 1234);
    User *u2 = new User(2, "Bob", 5678);
    Listing *l1 = new Listing("Laptop", "Gaming Laptop", 5); // duration 5 sec

    System s;
    s.addUser(u1);
    s.addUser(u2);
    s.addListing(l1);

    s.start(l1);

    // Simulate auction
    for (int t = 0; t < 7; t++)
    {
        System::current_time = t;
        if (t == 1)
            s.addBid(u1, 500);
        if (t == 2)
            s.addBid(u2, 600);
        if (t == 4)
            s.addBid(u1, 550); // too low
        if (t == 6)
            s.check_auction();
    }

    delete u1;
    delete u2;
    delete l1;
}
