//idealy , this code should use singleton , state and strategy design patterns
//but for simplicity , we are not using them here

#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Account
{
    string name;
    int pin;
    int balance;

public:
    Account(string name, int pin, int balance) : name(name), pin(pin), balance(balance) {}
    int checkBalance()
    {
        return balance;
    }

    bool validate(int p)
    {
        return pin == p;
    }

    string getName()
    {
        return name;
    }
    // in general , these codes should be in separate class
    bool withDraw(int amt)
    {
        if (balance < amt)
            return false;
        balance -= amt;
        return true;
    }

    void deposit(int amt)
    {
        balance += amt;
    }
};

class ATM
{
    vector<Account *> accounts;
    bool isAuthenticated;
    Account *current;

public:
    ATM() : isAuthenticated(false), current(nullptr) {}

    void addAccount(Account *acc)
    {
        accounts.push_back(acc);
    }

    bool authenticate(string acc, int pin)
    {
        for (auto x : accounts)
        {
            if (x->getName() == acc && x->validate(pin))
            {
                current = x;
                isAuthenticated = true;
                return true;
            }
        }
        return false;
    }

    void logout()
    {
        current = nullptr;
        isAuthenticated = false;
    }

    void checkBalance()
    {
        if (isAuthenticated && current)
        {
            cout << "Balance: " << current->checkBalance() << endl;
        }
        else
        {
            cout << "Not authenticated." << endl;
        }
    }

    void addMoney()
    {
        if (isAuthenticated && current)
        {
            int amt;
            cout << "Enter amount to add: ";
            cin >> amt;
            // Assuming Account class has a method to add money
            current->deposit(amt);
            cout << "Amount added successfully." << endl;
        }
        else
        {
            cout << "Not authenticated." << endl;
        }
    }

    void withDraw()
    {
        if (isAuthenticated && current)
        {
            int amt;
            cout << "Enter amount to withdraw: ";
            cin >> amt;
            if (current->withDraw(amt))
            {
                cout << "Withdrawal successful." << endl;
            }
            else
            {
                cout << "Insufficient balance." << endl;
            }
        }
        else
        {
            cout << "Not authenticated." << endl;
        }
    }

    void display()
    {
        cout << "These are the options" << endl;
        cout << "1. CheckBalance" << endl;
        cout << "2. AddMoney" << endl;
        cout << "3. WithdrawMoney" << endl;
        cout << "4. Logout" << endl;
    }

    void start()
    {
        cout << "Please authenticate yourself by entering account name and pin" << endl;
        string account;
        int pin;
        cin >> account >> pin;
        if (!authenticate(account, pin))
        {
            cout << "Wrong pin or account name." << endl;
            return;
        }

        int choice;
        do
        {
            display();
            cout << "Enter your choice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                checkBalance();
                break;
            case 2:
                addMoney();
                break;
            case 3:
                withDraw();
                break;
            case 4:
                logout();
                cout << "Logged out." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
            }
        } while (choice != 4);
    }
};

int main()
{
    ATM atm;

    atm.addAccount(new Account("a1", 1111, 11111));
    atm.addAccount(new Account("a2", 1111, 1111111));

    atm.start();

    return 0;
}
