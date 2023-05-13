#ifndef ACCOUNT_H
#define ACCOUNT_H
#define BOSS 7
#define WORKER 3
#define CUSTOMER 1
#define GUEST 0
#include"UnrolledLinkedList.h"
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
    std::stack<int> logged;
    bool isprivileged(int i);
    bool isvalidstr(const char* str);
public:
    int Login(const char* userid,const char* pwd);
    int Logout();
    int Register(const char* userid,const char* pwd,const char* username);
    int Revicepwd(const char* userid,const char* pwd1,const char* pwd2);
    int Addaccount(const char* userid,const char* pwd,const char* privilege,const char* username);
    int Removeaccount(const char* userid);
    Accountsystem(){
        account.init("./Account");
        string30 userid("root");
        User user(string30("sjtu"),7);
        account.insert(userid,user);
    }
    ~Accountsystem(){}
};
int Accountsystem::Removeaccount(const char* userid){
    if(!account.remove(string30(userid))){
        return -1;
    }else{
        return 0;
    }
}
bool Accountsystem::isvalidstr(const char* str){
        int len=strlen(str);
        char tmp;
        if(len>30){
            return 0;
        }else{
            for(int i=0;i<len;++i){
                tmp=str[i];
                if(!((tmp>='0'&&tmp<='9')||(tmp>='a'&&tmp<='z')||tmp>='A'&&tmp<='Z'||tmp=='_')){
                    return 0;
                }
            }
        }
        return 1;
    }
int Accountsystem::Logout(){
        if(logged.size()==0){
            return -1;
        }
        logged.pop();
        return 0;
    }
int Accountsystem::Login(const char* userid,const char* pwd=nullptr){
        // if(!(isvalidstr(userid))){
        //     return;
        // }
        User user;
        if(account.find(string30(userid),user)==-1){
            return -1;
        }else{
            if(logged.size()!=0&&logged.top()>user.privilege){
                logged.push(user.privilege);
                return 0;
            }else{
                if(string30(pwd)==user.pwd){
                    logged.push(user.privilege);
                    return 0;
                }
                return -1;
            }
        }

    }
int Accountsystem::Revicepwd(const char* userid,const char* pwd1,const char* pwd2=nullptr){
        User user;
        int pos=account.find(string30(userid),user);
        if(pos==-1){
            return -1;
        }else{
            if(user.privilege==BOSS){
                user.pwd=string30(pwd1);
                account.revise(pos,string30(userid),user);
                return 0;
            }else{
                if(user.pwd!=string30(pwd1)){
                    return -1;
                }else{
                    user.pwd=string30(pwd2);
                    account.revise(pos,string30(userid),user);
                    return 0;
                }
            }
            
        }
    }
int Accountsystem::Addaccount(const char* userid,const char* pwd,const char* privilege,const char* username){
        User user;
        string<30> usrid(userid),passwd(pwd);
        int priv=privilege[0]-'0';
        if(!isprivileged(priv)){
            return -1;
        }else{
            account.insert(usrid,User(passwd,priv));
            return 0;
        } 
    }
bool Accountsystem::isprivileged(int i){
    if(logged.size()==0||logged.top()<i){
        return 0;
    }else{
        return 1;
    }
}
int Accountsystem::Register(const char* userid,const char* pwd,const char* username){
    User user;
    string<30> usrid(userid),passwd(pwd);
    if(!account.insert(usrid,User(passwd,1))){
        return -1;
    }else{
        return 0;
    }
}

#endif