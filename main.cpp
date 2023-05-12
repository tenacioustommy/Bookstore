#include"Accountsystem.h"
#include<sstream>
void isvalid(int cmd){
    if(cmd==-1){
        std::cout<<"Invalid\n";
    }
}
int main(){
    std::cin.sync_with_stdio(false);
    std::string line;
    std::stringstream ss;
    std::string cmd[10];
    Accountsystem account;
    std::string instruction;
    while(getline(std::cin,line)){
        int cmd_cnt=0;
        std::string buf;
        ss.clear();
        ss<<line;
        while(ss>>buf){
            cmd[++cmd_cnt]=buf;
        }
        if(cmd_cnt==0){
            continue;
        }
        instruction=cmd[1];
        if(instruction=="quit"||instruction=="exit"){
            if(cmd_cnt>1){
                std::cout<<"Invalid\n";
            }
            break;
        }else if(instruction=="su"){
            if(cmd_cnt==3){
                isvalid(account.Login(cmd[2].c_str(),cmd[3].c_str()));
            }else{
                isvalid(account.Login(cmd[2].c_str()));
            }
        }else if(instruction=="logout"){
            isvalid(account.Logout());
        }else if(instruction=="register"){
            isvalid(account.Register(cmd[2].c_str(),cmd[3].c_str(),cmd[4].c_str()));
        }else if(instruction=="passwd"){
            if(cmd_cnt==3){
                isvalid(account.Revicepwd(cmd[2].c_str(),cmd[3].c_str()));
            }else if(cmd_cnt==4){
                isvalid(account.Revicepwd(cmd[2].c_str(),cmd[3].c_str(),cmd[4].c_str()));
            }
        }else if(instruction=="useradd"){
            isvalid(account.Addaccount(cmd[2].c_str(),cmd[3].c_str(),cmd[4].c_str(),cmd[5].c_str()));
        }else if(instruction=="delete"){
            isvalid(account.Removeaccount(cmd[2].c_str()));
        }else{
            isvalid(-1);
        }
    }
    
    system("pause");
    return 0;
}
// UnrolledLinkedList<string,int> list("./data");
    // int n;
    // std::cin>>n;
    // std::string instruction;
    // string index;
    // for(int i=0;i<n;i++){
    //     std::cin>>instruction;
    //     if(instruction=="insert"){
    //         int value;
    //         std::cin>>index.buf>>value;
    //         list.insert(index,value);
    //     }else if(instruction=="delete"){
    //         string index;
    //         int value;
    //         std::cin>>index.buf>>value;
    //         list.remove(index,value);
    //     }else if(instruction=="find"){
    //         string index;
    //         std::cin>>index.buf;
    //         list.find(index);
    //     }
    // }
    