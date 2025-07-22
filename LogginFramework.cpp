#include<bits/stdc++.h>
using namespace std;

enum class LogLevel{
    DEBUG,
    INFO,
    WARN
};

class LogMessage{
  string message;
  public:
  LogMessage(string & message):message(message){}
};
class LogAppender{
    public:
  virtual void append(LogMessage* msg)=0;
  virtual ~LogAppender(){};
};

class DB:public LogAppender{
    public:
    void append(LogMessage* msg){
        cout<<"Appeding the message to the Db"<<endl;
    }
};

class File:public LogAppender{
  public:
  void append(LogMessage* msg){
      cout<<"Appending the message to the File"<<endl;
  }
};
class Logger{
  vector<LogAppender*>appender;
  int level;
  static Logger* instance;
  Logger();
  public:
  static Logger* getinstance(){
      if(instance==nullptr){
          instance=new Logger();
      }
      return instance;
  }
  void log(string &msg){
      LogMessage* m=new LogMessage(msg);
      for(auto x:appender){
          x->append(m);
      }
  }
  void debug(LogLevel l,string& msg){
      log(msg);
  }
  void info(LogLevel l,string& msg){
      log(msg);
  }
  void warn(LogLevel l,string& msg){
      log(msg);
  }
};
Logger::Logger() {
    // Initialize default values
    level = 0; // or some default log level
    appender.push_back(new File()); // Add default appender(s)
    appender.push_back(new DB());
}

Logger* Logger::instance=nullptr;
int main(){
    Logger* l=Logger::getinstance();
    string msg="hi";
    l->debug(LogLevel::DEBUG,msg);
}