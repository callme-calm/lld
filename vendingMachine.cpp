#include <bits/stdc++.h>
using namespace std;

class Product {
    string id;
    int quantity;
    int price;
public:
    Product(string id, int quantity, int price) : id(id), quantity(quantity), price(price) {}
    string getId() { return id; }
    int getQuantity() { return quantity; }
    int getPrice() { return price; }
};

class PaymentStrategy {
public:
    virtual ~PaymentStrategy() {}
    virtual void pay() = 0;
};

class CashPayment : public PaymentStrategy {
public:
    void pay() override {
        cout << "Paying via Cash" << endl;
    }
};

class UpiPayment : public PaymentStrategy {
public:
    void pay() override {
        cout << "Paying via UPI" << endl;
    }
};

// Forward declaration
class VendingMachine;

class State {
public:
    virtual void selectProduct(VendingMachine &vm, int id) = 0;
    virtual void choosePaymentMethod(VendingMachine &vm, int id) = 0;
    virtual void pay(VendingMachine &vm, int amount) = 0;
    virtual void dispenseProduct(VendingMachine &vm) = 0;
};

class Idle;
class ProductChosen;
class Paying;
class Dispense;

class VendingMachine {
    unordered_map<int, Product*> products;
    int earning = 0;
    State* current;
    int selected = -1;
    PaymentStrategy* strategy = nullptr;

public:
    VendingMachine() ;
        

    void addProduct(Product* p) {
        static int productId = 0;
        products[productId++] = p;
    }

    Product* getSelectedProduct() {
        if (products.find(selected) != products.end()) {
            return products[selected];
        }
        return nullptr;
    }

    void selectProduct(int id) {
        current->selectProduct(*this, id);
    }

    void chooseMethod(int id) {
        current->choosePaymentMethod(*this, id);
    }

    void pay(int amount) {
        earning += amount;
        current->pay(*this, amount);
    }

    void dispense() {
        current->dispenseProduct(*this);
    }

    void setSelected(int id) {
        selected = id;
    }

    int getSelected() {
        return selected;
    }

    void setState(State* x) {
        delete current;
        current = x;
    }

    void setPaymentStrategy(PaymentStrategy* s) {
        if (strategy) delete strategy;
        strategy = s;
    }

    PaymentStrategy* getPaymentStrategy() {
        return strategy;
    }

    ~VendingMachine() {
        delete current;
        if (strategy) delete strategy;
        for (auto &p : products) delete p.second;
    }
};

class Idle : public State {
public:
    void selectProduct(VendingMachine &vm, int id) override;
    void choosePaymentMethod(VendingMachine &, int) override {
        cout << "Select product first." << endl;
    }
    void pay(VendingMachine &, int) override {
        cout << "Choose payment method first." << endl;
    }
    void dispenseProduct(VendingMachine &) override {
        cout << "Do the payment first." << endl;
    }
};

class ProductChosen : public State {
public:
    void selectProduct(VendingMachine &, int) override {
        cout << "Product already chosen." << endl;
    }
    void choosePaymentMethod(VendingMachine &vm, int id) override;
    void pay(VendingMachine &, int) override {
        cout << "Choose payment method first." << endl;
    }
    void dispenseProduct(VendingMachine &) override {
        cout << "Insert coin first." << endl;
    }
};

class Paying : public State {
public:
    void selectProduct(VendingMachine &, int) override {
        cout << "Product already selected." << endl;
    }
    void choosePaymentMethod(VendingMachine &, int) override {
        cout << "Payment method already chosen." << endl;
    }
    void pay(VendingMachine &vm, int amount) override;
    void dispenseProduct(VendingMachine &) override {
        cout << "First pay the amount." << endl;
    }
};

class Dispense : public State {
public:
    void selectProduct(VendingMachine &, int) override {
        cout << "Product already selected." << endl;
    }
    void choosePaymentMethod(VendingMachine &, int) override {
        cout << "Payment method already chosen." << endl;
    }
    void pay(VendingMachine &, int) override {
        cout << "Already paid." << endl;
    }
    void dispenseProduct(VendingMachine &vm) override;
};

// === State Method Definitions ===

void Idle::selectProduct(VendingMachine &vm, int id) {
    vm.setSelected(id);
    vm.setState(new ProductChosen());
    cout << "Product selected." << endl;
}

void ProductChosen::choosePaymentMethod(VendingMachine &vm, int id) {
    if (id == 1) vm.setPaymentStrategy(new CashPayment());
    else vm.setPaymentStrategy(new UpiPayment());
    vm.setState(new Paying());
    cout << "Payment method chosen." << endl;
}

void Paying::pay(VendingMachine &vm, int amount) {
    cout << "Paying amount: " << amount << endl;
    if (vm.getPaymentStrategy()) vm.getPaymentStrategy()->pay();
    vm.setState(new Dispense());
}

void Dispense::dispenseProduct(VendingMachine &vm) {
    Product* p = vm.getSelectedProduct();
    if (p) {
        cout << "Dispensing: " << p->getId() << endl;
    } else {
        cout << "Product not found." << endl;
    }
    vm.setState(new Idle());
}
//very very Important
VendingMachine::VendingMachine(){
    current=new Idle();
}
// === Main ===

int main() {
    VendingMachine vm;

    vm.addProduct(new Product("Coke", 5, 70));   // ID = 0
    vm.addProduct(new Product("Chips", 5, 20));  // ID = 1

    vm.selectProduct(1);      // Selecting "Chips"
    vm.chooseMethod(1);       // Choosing Cash
    vm.pay(20);               // Paying
    vm.dispense();            // Dispensing

    return 0;
}
