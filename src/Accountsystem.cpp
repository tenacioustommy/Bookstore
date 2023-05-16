#include"Accountsystem.h"
Accountsystem account;
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
        if(logged.size()==1){
            return -1;
        }
        logged.pop();
        library.bookstack.pop();
        //update book ,very keng
        if(!library.bookstack.empty()){
            int pos=library.bookstack.top().second;
            if(pos!=-1){
                library.getbook(library.bookstack.top().first,pos);
            }
        
        }
        
        
        return 0;
    }
int Accountsystem::Login(const char* userid,const char* pwd){
        // if(!(isvalidstr(userid))){
        //     return;
        // }
        User user;
        if(account.find(string30(userid),user)==-1){
            return -1;
        }else{
            if(logged.size()!=0&&logged.top()>user.privilege){
                logged.push(user.privilege);
                library.bookstack.push(std::pair<Book,int>(Book(),-1));
                return 0;
            }else{
                if(string30(pwd)==user.pwd){
                    logged.push(user.privilege);
                    library.bookstack.push(std::pair<Book,int>(Book(),-1));
                    return 0;
                }
                return -1;
            }
        }

    }
int Accountsystem::Revicepwd(const char* userid,const char* pwd1,const char* pwd2){
        User user;
        int pos=account.find(string30(userid),user);
        if(pos==-1){
            return -1;
        }else{
            if(logged.top()==BOSS){
                user.pwd=string30(pwd1);
                account.revise(pos,string30(userid),user);
                return 0;
            }else if(pwd2==nullptr){
                return -1;
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
int Accountsystem::Addaccount(const char* userid,const char* pwd,int privilege,const char* username){
        User user;
        string<30> usrid(userid),passwd(pwd);
        if(logged.top()<=privilege){
            return -1;
        }else{
            if(account.insertunique(usrid,User(passwd,privilege)).second)
            {
                return 0;
            }else{
                return -1;
            }
            
        } 
    }
int Accountsystem::Register(const char* userid,const char* pwd,const char* username){
    User user;
    string<30> usrid(userid),passwd(pwd);
    if(!account.insertunique(usrid,User(passwd,1)).second){
        return -1;
    }else{
        return 0;
    }
}