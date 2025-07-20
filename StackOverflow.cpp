#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
using namespace std;

class GenerateId
{
public:
    static string generateid()
    {
        string res = "";
        for (int i = 0; i < 6; i++)
        {
            res += char('A' + rand() % 26);
        }
        return res;
    }
};

class Voteable
{
    int count = 0;

public:
    void addVote() { count++; }
    int getCount() const { return count; }
};

class User
{
    string id;
    string name;

public:
    User(const string &name) : name(name)
    {
        id = GenerateId::generateid();
    }
    string getName() const { return name; }
    string getId() const { return id; }
};

class Comment
{
    string id;
    string text;
    User *author;

public:
    Comment(User *author, const string &text) : author(author), text(text)
    {
        id = GenerateId::generateid();
    }
    void showComment() const
    {
        cout << author->getName() << ": " << text << endl;
    }
};

class Commentable
{
protected:
    vector<Comment *> comments;

public:
    virtual ~Commentable() {}
    void addComment(Comment *comment)
    {
        comments.push_back(comment);
    }
    void showComments() const
    {
        for (auto c : comments)
        {
            c->showComment();
        }
    }
};

class Question : public Voteable, public Commentable
{
    string id;
    User *author;
    string text;

public:
    Question(User *author, const string &text) : author(author), text(text)
    {
        id = GenerateId::generateid();
    }
    string getId() const { return id; }
    string getText() const { return text; }
    void showQuestion() const
    {
        cout << "[Q] " << author->getName() << ": " << text << " (Votes: " << getCount() << ")" << endl;
        showComments();
    }
};

class Answer : public Voteable, public Commentable
{
    string id;
    User *author;
    string text;

public:
    Answer(User *author, const string &text) : author(author), text(text)
    {
        id = GenerateId::generateid();
    }
    string getId() const { return id; }
    void showAnswer() const
    {
        cout << "[A] " << author->getName() << ": " << text << " (Votes: " << getCount() << ")" << endl;
        showComments();
    }
};

class Post
{
    User *author;
    Question *question;
    vector<Answer *> answers;

public:
    Post(User *author, const string &questionText) : author(author)
    {
        question = new Question(author, questionText);
    }
    Question *getQuestion() const { return question; }
    void addAnswer(Answer *answer)
    {
        answers.push_back(answer);
    }
    void showPost() const
    {
        question->showQuestion();
        cout << "\nAnswers:\n";
        for (auto a : answers)
        {
            a->showAnswer();
        }
    }
};

class StackOverflow
{
    vector<User *> users;
    vector<Post *> posts;
    static StackOverflow *instance;

    StackOverflow() {}

public:
    static StackOverflow *getInstance()
    {
        if (!instance)
            instance = new StackOverflow();
        return instance;
    }

    void addUser(User *user) { users.push_back(user); }
    void addPost(Post *post) { posts.push_back(post); }

    void showAllPosts() const
    {
        for (auto p : posts)
        {
            p->showPost();
            cout << "\n----------------------------\n";
        }
    }
};

StackOverflow *StackOverflow::instance = nullptr;

int main()
{
    srand(time(0));

    StackOverflow *so = StackOverflow::getInstance();

    User *u1 = new User("Rohan");
    User *u2 = new User("Mohan");
    so->addUser(u1);
    so->addUser(u2);

    Post *p1 = new Post(u1, "What is a vertex?");
    so->addPost(p1);

    Answer *a1 = new Answer(u2, "It's a node in a graph.");
    a1->addVote();
    a1->addVote();
    p1->addAnswer(a1);

    Comment *c1 = new Comment(u1, "Thanks for the explanation!");
    a1->addComment(c1);

    Comment *c2 = new Comment(u2, "You're welcome!");
    p1->getQuestion()->addComment(c2);

    so->showAllPosts();

    return 0;
}
