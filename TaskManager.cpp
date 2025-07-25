//This question focuses on encapsulation principles, each class should be only responsible for only  its methods

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>
#include <mutex>
#include <map>

using namespace std;

// ========== ENUMS ==========

enum class Status
{
    Pending,
    InProgress,
    Completed
};
enum class Priority
{
    Low,
    Medium,
    High
};

string to_string(Status s)
{
    switch (s)
    {
    case Status::Pending:
        return "Pending";
    case Status::InProgress:
        return "In Progress";
    case Status::Completed:
        return "Completed";
    }
    return "";
}

string to_string(Priority p)
{
    switch (p)
    {
    case Priority::Low:
        return "Low";
    case Priority::Medium:
        return "Medium";
    case Priority::High:
        return "High";
    }
    return "";
}

// ========== INTERFACES ==========

class IAssignee
{
public:
    virtual string getName() const = 0;
    virtual void recordHistory(const string &entry) = 0;
    virtual ~IAssignee() = default;
};

// ========== USER CLASS ==========

class User : public IAssignee
{
    string name;
    vector<string> history;

public:
    explicit User(string name) : name(std::move(name)) {}

    string getName() const override
    {
        return name;
    }

    void recordHistory(const string &entry) override
    {
        history.push_back(entry);
    }

    void showHistory() const
    {
        cout << "\nHistory for " << name << ":\n";
        for (const auto &entry : history)
            cout << "- " << entry << "\n";
    }
};

// ========== TASK CLASS ==========

class Task
{
    string title;
    string description;
    int dueDate; // Format: YYYYMMDD
    Priority priority;
    Status status;
    shared_ptr<IAssignee> assignedTo;

public:
    Task(string title, string desc, int date, Priority prio)
        : title(std::move(title)), description(std::move(desc)), dueDate(date), priority(prio), status(Status::Pending) {}

    void assignTo(shared_ptr<IAssignee> user)
    {
        assignedTo = move(user);
        assignedTo->recordHistory("Assigned task: " + title);
    }

    void markCompleted()
    {
        status = Status::Completed;
        if (assignedTo)
            assignedTo->recordHistory("Completed task: " + title);
    }

    void update(string newDesc, int newDate, Priority newPrio)
    {
        description = move(newDesc);
        dueDate = newDate;
        priority = newPrio;
    }

    bool isDue(int today) const
    {
        return dueDate <= today && status != Status::Completed;
    }

    // Getters
    string getTitle() const { return title; }
    string getAssignedTo() const { return assignedTo ? assignedTo->getName() : "None"; }
    Status getStatus() const { return status; }
    Priority getPriority() const { return priority; }
    int getDueDate() const { return dueDate; }

    void print() const
    {
        cout << "\nTask: " << title << "\n"
             << "  Description: " << description << "\n"
             << "  Due Date: " << dueDate << "\n"
             << "  Priority: " << to_string(priority) << "\n"
             << "  Status: " << to_string(status) << "\n"
             << "  Assigned To: " << getAssignedTo() << "\n";
    }
};

// ========== TASK MANAGER ==========

class TaskManager
{
    vector<shared_ptr<Task>> tasks;
    vector<shared_ptr<User>> users;
    mutable mutex taskMutex;

public:
    shared_ptr<User> createUser(const string &name)
    {
        auto user = make_shared<User>(name);
        users.push_back(user);
        return user;
    }

    shared_ptr<Task> createTask(const string &title, const string &desc, int due, Priority p)
    {
        auto task = make_shared<Task>(title, desc, due, p);
        lock_guard<mutex> lock(taskMutex);
        tasks.push_back(task);
        return task;
    }

    void assignTask(const shared_ptr<Task> &task, const shared_ptr<User> &user)
    {
        task->assignTo(user);
    }

    void markTaskComplete(const shared_ptr<Task> &task)
    {
        task->markCompleted();
    }

    void deleteTask(const string &title)
    {
        lock_guard<mutex> lock(taskMutex);
        tasks.erase(remove_if(tasks.begin(), tasks.end(), [&](auto &t)
                              { return t->getTitle() == title; }),
                    tasks.end());
    }

    // Filtering
    void filterByPriority(Priority p) const
    {
        cout << "\nTasks with Priority: " << to_string(p) << "\n";
        for (const auto &t : tasks)
        {
            if (t->getPriority() == p)
                t->print();
        }
    }

    void filterByStatus(Status s) const
    {
        cout << "\nTasks with Status: " << to_string(s) << "\n";
        for (const auto &t : tasks)
        {
            if (t->getStatus() == s)
                t->print();
        }
    }

    void filterByUser(const string &username) const
    {
        cout << "\nTasks assigned to: " << username << "\n";
        for (const auto &t : tasks)
        {
            if (t->getAssignedTo() == username)
                t->print();
        }
    }

    void showReminders(int today) const
    {
        cout << "\nReminder - Tasks due by " << today << ":\n";
        for (const auto &t : tasks)
        {
            if (t->isDue(today))
                t->print();
        }
    }

    void showAllTasks() const
    {
        cout << "\nAll Tasks:\n";
        for (const auto &t : tasks)
        {
            t->print();
        }
    }
};

// ========== MAIN FUNCTION (Usage) ==========

int main()
{
    TaskManager manager;

    auto alice = manager.createUser("Alice");
    auto bob = manager.createUser("Bob");

    auto task1 = manager.createTask("Build UI", "Design the front page", 20250725, Priority::High);
    auto task2 = manager.createTask("API Integration", "Connect UI to backend", 20250726, Priority::Medium);
    auto task3 = manager.createTask("Unit Testing", "Write tests", 20250727, Priority::Low);

    manager.assignTask(task1, alice);
    manager.assignTask(task2, bob);
    manager.assignTask(task3, alice);

    manager.markTaskComplete(task1);

    manager.showAllTasks();
    manager.filterByPriority(Priority::High);
    manager.filterByUser("Alice");
    manager.filterByStatus(Status::Completed);
    manager.showReminders(20250726);

    alice->showHistory();
}
