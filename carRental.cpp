#include <bits/stdc++.h>
using namespace std;

class Car
{
public:
    string type;
    int price;
    bool isAvailable;

    Car(string type, int price, bool avail)
        : type(type), price(price), isAvailable(avail) {}
};

class System
{
    vector<Car> allCars;
    vector<Car *> workingList; // stores filtered pointers

public:
    System &addCar(string type, int price, bool avail)
    {
        allCars.emplace_back(type, price, avail);
        return *this;
    }

    // Reset working list to all cars
    void resetFilters()
    {
        workingList.clear();
        for (auto &car : allCars)
            workingList.push_back(&car);
    }

    // Filter by type
    System &filterType(const string &type)
    {
        auto it = remove_if(workingList.begin(), workingList.end(),
                            [&](Car *c)
                            { return c->type != type; });
        workingList.erase(it, workingList.end());
        return *this;
    }

    // Filter by price range
    System &filterPrice(int minP, int maxP)
    {
        auto it = remove_if(workingList.begin(), workingList.end(),
                            [&](Car *c)
                            { return c->price < minP || c->price > maxP; });
        workingList.erase(it, workingList.end());
        return *this;
    }

    // Filter by availability
    System &filterAvailability(bool avail)
    {
        auto it = remove_if(workingList.begin(), workingList.end(),
                            [&](Car *c)
                            { return c->isAvailable != avail; });
        workingList.erase(it, workingList.end());
        return *this;
    }

    // Get the final results
    vector<Car *> getResults()
    {
        return workingList;
    }
};

int main()
{
    System sys;
    sys.addCar("SUV", 3000, true)
        .addCar("Sedan", 2000, false)
        .addCar("SUV", 5000, true)
        .addCar("Hatchback", 1500, true);

    // Start filtering
    sys.resetFilters();
    vector<Car *> res = sys.filterType("SUV")
                            .filterPrice(2500, 4000)
                            .filterAvailability(true)
                            .getResults();

    cout << "Filtered cars:\n";
    for (auto car : res)
    {
        cout << car->type << " - Price: " << car->price
             << " - Available: " << (car->isAvailable ? "Yes" : "No") << "\n";
    }

    return 0;
}
