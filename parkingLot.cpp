#include<bits/stdc++.h>
using namespace std;


enum class VType{
  byke,
  car,
  truck
};

class Vehicle{
    string id;
    VType type;
    public:
    Vehicle(string id,VType type):type(type),id(id){};
    VType getType(){
        return type;
    }
};

class Bike:public Vehicle{
  public:
  Bike(string id,VType type):Vehicle(id,type){}
};

class Car:public Vehicle{
  public:
  Car(string id,VType type):Vehicle(id,type){}
};

class Truck:public Vehicle{
  public:
  Truck(string id,VType type):Vehicle(id,type){}
};

class ParkingSpot{
    string id;
    VType type;
    bool Available;
    public:
    ParkingSpot(string id,VType type):id(id),type(type){
        Available=true;
    }
    VType getType(){
        return type;
    }
    bool isAvailable(){
        return Available;
    }
    void setUnAvailble(){
        Available=false;
    }
};


class ParkingFloor{
    string id;
    vector<ParkingSpot*>parkingspots;
    public:
    ParkingFloor(string id,vector<ParkingSpot*>spots):id(id){
        parkingspots=spots;
    }
    
    vector<ParkingSpot*> getSpots(){
        return parkingspots;
    }
    
};

class Ticket{
    string id;
    Vehicle* vehicle;
    ParkingSpot* spot;
    public:
    Ticket(string id,Vehicle* vehicle,ParkingSpot* spot):spot(spot),id(id),vehicle(vehicle){}
    
    
    //complete
    void bookTicket(ParkingSpot* spot,Vehicle* vehicle){
        this->spot=spot;
        this->vehicle=vehicle;
        this->spot->setUnAvailble();
        cout<<"Ticket booked"<<endl;
    }
};


class ParkingLot{
    vector<ParkingFloor*>parkingfloors;
    vector<Ticket*>tickets;
    public:
    //complete
    Ticket* bookSpot(Vehicle* vehicle){
        for(auto y:parkingfloors){
            vector<ParkingSpot*> spots=y->getSpots();
            for(auto x:spots){
                if(x->getType()==vehicle->getType() and x->isAvailable()){
                    
                 Ticket* t=new Ticket("t1",vehicle,x);
                 t->bookTicket(x,vehicle);
                 tickets.push_back(t);
                 return t;
                }
            }
        }
        return nullptr;
    }
    void addFloor(ParkingFloor* f){
        parkingfloors.push_back(f);
    }
    
};

int main(){
    ParkingLot* lot=new ParkingLot();
    ParkingSpot* s1=new ParkingSpot("s1",VType::car);
    ParkingSpot* s2=new ParkingSpot("s2",VType::byke);
    ParkingSpot* s3=new ParkingSpot("s3",VType::truck);
    ParkingFloor* f1=new ParkingFloor("f1",{s1,s2,s3});
    lot->addFloor(f1);
    Vehicle* b=new Bike("b1",VType::byke);
    lot->bookSpot(b);
    return 0;
}




