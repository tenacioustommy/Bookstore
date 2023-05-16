#ifndef ACCOUNT_H
#define ACCOUNT_H
#define BOSS 7
#define WORKER 3
#define CUSTOMER 1
#define GUEST 0
#include"UnrolledLinkedList.h"
#include"Librarysystem.h"
#include<stack>

class Accountsystem
{
    typedef string<30> string30;
private:
    /* data */
    struct User
    {
        int privilege;
        string30 pwd;
        User()=default;
        User(string30 passwd,int privilege):privilege(privilege),pwd(passwd){}
    };
    UnrolledLinkedList<string30,User> account;
    
    bool isprivileged(int i);
    bool isvalidstr(const char* str);
public:
    std::stack<int> logged;
    int Login(const char* userid,const char* pwd=nullptr);
    int Logout();
    int Register(const char* userid,const char* pwd,const char* username);
    int Revicepwd(const char* userid,const char* pwd1,const char* pwd2=nullptr);
    int Addaccount(const char* userid,const char* pwd,int privilege,const char* username);
    int Removeaccount(const char* userid);
    Accountsystem(){
        account.init("Account");
        string30 userid("root");
        User user(string30("sjtu"),7);
        account.insertunique(userid,user);
        logged.push(0);
    }
    ~Accountsystem(){}
};

extern Accountsystem account;

#endif