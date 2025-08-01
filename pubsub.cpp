// important thing to keep in mind

// 1. dont use interface for topics, just use a class
// 2.create a map of string and topics in the interface class

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
using namespace std;

// ------------------------ Message ------------------------
class Message
{
public:
    string content;
    Message(string msg) : content(msg) {}
};

// ------------------------ Subscriber ------------------------
class Subscriber
{
public:
    string name;
    Subscriber(string name) : name(name) {}
    virtual void notify(const Message &msg) = 0;
    virtual ~Subscriber() {}
};

class Log : public Subscriber
{
public:
    Log(string name) : Subscriber(name) {}
    void notify(const Message &msg) override
    {
        cout << "[Log] " << name << " received: " << msg.content << endl;
    }
};

class Human : public Subscriber
{
public:
    Human(string name) : Subscriber(name) {}
    void notify(const Message &msg) override
    {
        cout << "[Human] " << name << " received: " << msg.content << endl;
    }
};

// ------------------------ Topic ------------------------
class Topic
{
    string topicName;
    vector<Subscriber *> subscribers;

public:
    Topic(string name) : topicName(name) {}

    void addSubscriber(Subscriber *s)
    {
        subscribers.push_back(s);
    }

    void publish(const Message &msg)
    {
        for (auto sub : subscribers)
        {
            sub->notify(msg);
        }
    }

    string getName() const
    {
        return topicName;
    }
};

// ------------------------ Interface ------------------------
class Interface
{
    unordered_map<string, Topic *> topicMap;

public:
    void addTopic(string name)
    {
        if (topicMap.find(name) == topicMap.end())
        {
            topicMap[name] = new Topic(name);
        }
    }

    void addSubscriberToTopic(string topicName, Subscriber *subscriber)
    {
        if (topicMap.find(topicName) != topicMap.end())
        {
            topicMap[topicName]->addSubscriber(subscriber);
        }
        else
        {
            cout << "Topic '" << topicName << "' does not exist.\n";
        }
    }

    void publishToTopic(string topicName, string message)
    {
        if (topicMap.find(topicName) != topicMap.end())
        {
            topicMap[topicName]->publish(Message(message));
        }
        else
        {
            cout << "Topic '" << topicName << "' does not exist.\n";
        }
    }

    ~Interface()
    {
        for (auto &pair : topicMap)
        {
            delete pair.second;
        }
    }
};

// ------------------------ main ------------------------
int main()
{
    Interface system;

    // Create topics
    system.addTopic("Sports");
    system.addTopic("Tech");

    // Create subscribers
    Subscriber *alice = new Human("Alice");
    Subscriber *bob = new Log("Logger-Bob");

    // Subscribe to topics
    system.addSubscriberToTopic("Sports", alice);
    system.addSubscriberToTopic("Tech", bob);
    system.addSubscriberToTopic("Tech", alice); // Alice subscribes to Tech too

    // Publish messages
    system.publishToTopic("Sports", "India won the match!");
    system.publishToTopic("Tech", "New AI model released!");

    // Clean up
    delete alice;
    delete bob;

    return 0;
}
