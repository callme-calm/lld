#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
using namespace std;

// ------------ User Profile ------------
class Profile
{
    string profilePic, headline, summary;
    vector<string> experience, education, skills;

public:
    void updateProfile(const string &h, const string &s)
    {
        headline = h;
        summary = s;
    }

    void addExperience(const string &exp)
    {
        experience.push_back(exp);
    }

    void addEducation(const string &edu)
    {
        education.push_back(edu);
    }

    void addSkill(const string &skill)
    {
        skills.push_back(skill);
    }

    void display() const
    {
        cout << "Headline: " << headline << "\nSummary: " << summary << "\n";
        cout << "Experience:\n";
        for (const auto &e : experience)
            cout << "- " << e << "\n";
        cout << "Education:\n";
        for (const auto &e : education)
            cout << "- " << e << "\n";
        cout << "Skills:\n";
        for (const auto &s : skills)
            cout << "- " << s << "\n";
    }
};

// ------------ Notification System (Observer) ------------
class Observer
{
public:
    virtual void notify(const string &message) = 0;
};

class NotificationService
{
    vector<Observer *> observers;

public:
    void subscribe(Observer *obs)
    {
        observers.push_back(obs);
    }

    void sendNotification(const string &message)
    {
        for (auto obs : observers)
            obs->notify(message);
    }
};

// ------------ Messaging ------------
class Message
{
    string sender, receiver, content;

public:
    Message(const string &s, const string &r, const string &c)
        : sender(s), receiver(r), content(c) {}

    void display() const
    {
        cout << "From: " << sender << "\nTo: " << receiver << "\nMessage: " << content << "\n";
    }

    string getReceiver() const { return receiver; }
};

// ------------ User Base ------------
class User : public Observer
{
protected:
    string name, email, password;
    Profile profile;
    vector<User *> connections;
    vector<Message> inbox, sent;

public:
    User(const string &n, const string &e, const string &p) : name(n), email(e), password(p) {}

    string getEmail() const { return email; }
    bool authenticate(const string &pw) const { return password == pw; }

    void updateProfile()
    {
        string headline, summary;
        cout << "Enter headline: ";
        getline(cin, headline);
        cout << "Enter summary: ";
        getline(cin, summary);
        profile.updateProfile(headline, summary);
    }

    void showProfile() const
    {
        cout << "Name: " << name << ", Email: " << email << "\n";
        profile.display();
    }

    void addConnection(User *u)
    {
        if (find(connections.begin(), connections.end(), u) == connections.end())
        {
            connections.push_back(u);
        }
    }

    void viewConnections() const
    {
        cout << "Connections of " << name << ":\n";
        for (auto *c : connections)
            cout << "- " << c->name << "\n";
    }

    void sendMessage(User *to, const string &msg)
    {
        Message m(email, to->getEmail(), msg);
        sent.push_back(m);
        to->receiveMessage(m);
    }

    void receiveMessage(const Message &m)
    {
        inbox.push_back(m);
    }

    void viewInbox() const
    {
        cout << "Inbox:\n";
        for (const auto &m : inbox)
            m.display();
    }

    void viewSent() const
    {
        cout << "Sent:\n";
        for (const auto &m : sent)
            m.display();
    }

    void notify(const string &message) override
    {
        cout << "[Notification] " << message << "\n";
    }
};

// ------------ Employer (Special User) ------------
class Employer : public User
{
public:
    Employer(const string &n, const string &e, const string &p) : User(n, e, p) {}

    // Employers can post jobs
};

// ------------ Job Posting ------------
class Job
{
    string title, description, requirements, location;

public:
    Job(const string &t, const string &d, const string &r, const string &l)
        : title(t), description(d), requirements(r), location(l) {}

    void display() const
    {
        cout << "Job: " << title << "\nDesc: " << description << "\nReq: " << requirements << "\nLocation: " << location << "\n";
    }

    string getTitle() const { return title; }
};

// ------------ Job Board ------------
class JobBoard
{
    vector<Job> jobs;

public:
    void postJob(const Job &job)
    {
        jobs.push_back(job);
    }

    void listJobs() const
    {
        cout << "Available Jobs:\n";
        for (const auto &j : jobs)
            j.display();
    }

    void searchJob(const string &keyword) const
    {
        for (const auto &j : jobs)
        {
            if (j.getTitle().find(keyword) != string::npos)
            {
                j.display();
            }
        }
    }
};

// ------------ App (Orchestrator) ------------
class App
{
    map<string, User *> users;
    NotificationService notifier;
    JobBoard jobBoard;
    User *currentUser = nullptr;

public:
    void registerUser()
    {
        string name, email, pw;
        cout << "Enter Name, Email, Password:\n";
        cin >> ws;
        getline(cin, name);
        getline(cin, email);
        getline(cin, pw);
        users[email] = new User(name, email, pw);
        notifier.subscribe(users[email]);
        cout << "User Registered.\n";
    }

    void loginUser()
    {
        string email, pw;
        cout << "Enter Email & Password:\n";
        cin >> ws;
        getline(cin, email);
        getline(cin, pw);
        if (users.count(email) && users[email]->authenticate(pw))
        {
            currentUser = users[email];
            cout << "Login successful.\n";
        }
        else
        {
            cout << "Invalid credentials.\n";
        }
    }

    void logoutUser()
    {
        currentUser = nullptr;
        cout << "Logged out.\n";
    }

    void postJob()
    {
        if (!currentUser)
            return;
        string t, d, r, l;
        cout << "Enter Title, Description, Requirements, Location:\n";
        cin >> ws;
        getline(cin, t);
        getline(cin, d);
        getline(cin, r);
        getline(cin, l);
        jobBoard.postJob(Job(t, d, r, l));
        notifier.sendNotification("New job posted: " + t);
    }

    void applyToJob()
    {
        jobBoard.listJobs();
        cout << "Feature not implemented: Job application simulation.\n";
    }

    void searchJobs()
    {
        string keyword;
        cout << "Enter keyword to search:\n";
        cin >> ws;
        getline(cin, keyword);
        jobBoard.searchJob(keyword);
    }

    void sendConnectionRequest(const string &email)
    {
        if (!currentUser || users.find(email) == users.end())
            return;
        users[email]->notify(currentUser->getEmail() + " sent you a connection request.");
        users[email]->addConnection(currentUser);
        currentUser->addConnection(users[email]);
    }

    void sendMessageTo(const string &email)
    {
        if (!currentUser || users.find(email) == users.end())
            return;
        string msg;
        cout << "Enter message:\n";
        cin >> ws;
        getline(cin, msg);
        currentUser->sendMessage(users[email], msg);
    }

    void run()
    {
        while (true)
        {
            cout << "\n1.Register 2.Login 3.PostJob 4.ViewJobs 5.Search 6.Profile 7.Connect 8.Message 9.Inbox 10.Connections 11.Logout 12.Exit\n";
            int choice;
            cin >> choice;
            switch (choice)
            {
            case 1:
                registerUser();
                break;
            case 2:
                loginUser();
                break;
            case 3:
                postJob();
                break;
            case 4:
                jobBoard.listJobs();
                break;
            case 5:
                searchJobs();
                break;
            case 6:
                if (currentUser)
                    currentUser->updateProfile();
                break;
            case 7:
            {
                string email;
                cout << "Enter email to connect:\n";
                cin >> ws;
                getline(cin, email);
                sendConnectionRequest(email);
                break;
            }
            case 8:
            {
                string email;
                cout << "Enter email to message:\n";
                cin >> ws;
                getline(cin, email);
                sendMessageTo(email);
                break;
            }
            case 9:
                if (currentUser)
                    currentUser->viewInbox();
                break;
            case 10:
                if (currentUser)
                    currentUser->viewConnections();
                break;
            case 11:
                logoutUser();
                break;
            case 12:
                return;
            default:
                cout << "Invalid.\n";
            }
        }
    }
};

int main()
{
    App app;
    app.run();
    return 0;
}
