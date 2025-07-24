// observer , strategy , factory

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

// Abstract Coffee class
class Coffee
{
protected:
    vector<string> ingredients;
    int price;

public:
    Coffee(vector<string> x, int p) : ingredients(x), price(p) {}

    virtual vector<string> getIngredient() = 0;
    virtual int getPrice() = 0;

    virtual ~Coffee() {}
};

// Concrete Coffee types
class Macha : public Coffee
{
public:
    Macha(vector<string> x, int p) : Coffee(x, p) {}

    vector<string> getIngredient() override
    {
        return ingredients;
    }

    int getPrice() override
    {
        return price;
    }
};

class Dalgona : public Coffee
{
public:
    Dalgona(vector<string> x, int p) : Coffee(x, p) {}

    vector<string> getIngredient() override
    {
        return ingredients;
    }

    int getPrice() override
    {
        return price;
    }
};

// Observer interface
class Observer
{
public:
    virtual void notify() = 0;
    virtual ~Observer() {}
};

// Concrete Observer
class Warehouse : public Observer
{
public:
    void notify() override
    {
        cout << "Warehouse notified: Ingredient missing!" << endl;
    }
};

// Inventory class
class Inventory
{
    unordered_map<string, int> mp;
    vector<Observer *> obv;

public:
    void addIngredient(string item)
    {
        mp[item]++;
    }

    void addObserver(Observer *o)
    {
        obv.push_back(o);
    }

    void notify()
    {
        for (auto x : obv)
        {
            x->notify();
        }
    }

    bool hasIngredient(string item)
    {
        return mp[item] > 0;
    }

    void consumeIngredient(string item)
    {
        if (mp[item] > 0)
            mp[item]--;
    }
};

// Factory class
class CoffeeFactory
{
public:
    static Coffee *getCoffee(string name)
    {
        if (name == "macha")
            return new Macha({"milk", "matcha", "sugar"}, 120);
        if (name == "dalgona")
            return new Dalgona({"coffee", "milk", "sugar", "foam"}, 150);
        return nullptr;
    }
};

// VendingMachine class
class VendingMachine
{
    Inventory inventory;

public:
    void addIngredient(string item)
    {
        inventory.addIngredient(item);
    }

    void addObserver(Observer *obv)
    {
        inventory.addObserver(obv);
    }

    Coffee *buyCoffee(string name)
    {
        Coffee *c = CoffeeFactory::getCoffee(name);
        if (!c)
        {
            cout << "Invalid coffee type.\n";
            return nullptr;
        }

        vector<string> v = c->getIngredient();
        for (string x : v)
        {
            if (!inventory.hasIngredient(x))
            {
                notify();
                delete c;
                return nullptr;
            }
        }

        for (string x : v)
        {
            inventory.consumeIngredient(x);
        }

        cout << name << " served. Price: ₹" << c->getPrice() << endl;
        return c;
    }

    void notify()
    {
        inventory.notify();
    }
};

// Main function
int main()
{
    VendingMachine vm;

    // Adding ingredients
    vm.addIngredient("milk");
    vm.addIngredient("sugar");
    vm.addIngredient("matcha");
    vm.addIngredient("coffee");
    vm.addIngredient("foam");

    // Register warehouse observer
    Warehouse w;
    vm.addObserver(&w);

    // Try buying Macha
    Coffee *c1 = vm.buyCoffee("macha");
    if (c1)
        delete c1;

    // Try buying Dalgona
    Coffee *c2 = vm.buyCoffee("dalgona");
    if (c2)
        delete c2;

    // Try again without enough ingredients
    Coffee *c3 = vm.buyCoffee("dalgona"); // will notify
    if (c3)
        delete c3;

    return 0;
}
