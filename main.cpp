#include"Accountsystem.h"
#include"Librarysystem.h"
extern Librarysystem library;

bool ispriviledged(int neededprivilege,const Accountsystem& account){
    if(account.logged.top()<neededprivilege){    
        std::cout<<"Invalid\n";
        return 0;
    }else{
        return 1;
    }
}
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
            if(!ispriviledged(CUSTOMER,account))continue;
            if(cmd_cnt==1){
                isvalid(account.Logout());
            }else{
                isvalid(-1);
            }
        }else if(instruction=="register"){
            if(cmd_cnt==4){
                isvalid(account.Register(cmd[2].c_str(),cmd[3].c_str(),cmd[4].c_str()));
            }else{
                isvalid(-1);
            }
        }else if(instruction=="passwd"){
            if(!ispriviledged(CUSTOMER,account))continue;
            if(cmd_cnt==3){
                isvalid(account.Revicepwd(cmd[2].c_str(),cmd[3].c_str()));
            }else if(cmd_cnt==4){
                isvalid(account.Revicepwd(cmd[2].c_str(),cmd[3].c_str(),cmd[4].c_str()));
            }else{
                isvalid(-1);
            }
        }else if(instruction=="useradd"){
            if(!ispriviledged(WORKER,account))continue;
            if(cmd_cnt==5){
                isvalid(account.Addaccount(cmd[2].c_str(),cmd[3].c_str(),std::stoi(cmd[4]),cmd[5].c_str()));
            }else{
                isvalid(-1);
            }
        }else if(instruction=="delete"){
            if(!ispriviledged(BOSS,account))continue;
            isvalid(account.Removeaccount(cmd[2].c_str()));
        }else if(instruction=="modify"){
            if(cmd_cnt>6){
                isvalid(-1);
            }   
            else{
                std::string which[5],content[5];
                for(int i=2;i<1+cmd_cnt;i++){
                    if (cmd[i].size() >= 6 && cmd[i].substr(0, 6) == "-ISBN=") {
                        std::string str = cmd[i].substr(6);
                        which[i-2]="ISBN";
                        content[i-2]=str;
                    }
                    else if (cmd[i].size() >= 7 && cmd[i].substr(0, 7) == "-name=\"") {
                        std::string str = cmd[i].substr(7);
                        if (str.size() == 0 || str[str.size() - 1] != '\"') {
                            isvalid(-1);
                        }else{
                            str.pop_back();
                            // if (!CheckBookStr(str)) {
                            // throw error();
                            // }
                            which[i-2]="name";
                            content[i-2]=str;
                        }
                    }else if (cmd[i].size() >= 9 && cmd[i].substr(0, 9) == "-author=\"") {
                        std::string str = cmd[i].substr(9);
                        if (str.size() == 0 || str[str.size() - 1] != '\"') {
                            isvalid(-1);
                        }else{
                            str.pop_back();
                            // if (!CheckBookStr(str)) {
                            //     throw error();
                            // }
                            which[i-2]="author";
                            content[i-2]=str;
                        }
                    }
                    else if (cmd[i].size() >= 10 && cmd[i].substr(0, 10) == "-keyword=\"") {
                        std::string str = cmd[i].substr(10);
                        if (str.size() == 0 || str[str.size() - 1] != '\"') {
                            isvalid(-1);
                        }else{
                            str.pop_back();
                            // if (!CheckBookStr(str)) {
                            //     throw error();
                            // }
                            which[i-2]="keyword";
                            content[i-2]=str;
                        }
                    }else if (cmd[i].size() >= 7 && cmd[i].substr(0, 7) == "-price=") {
                        std::string str = cmd[i].substr(7);
                        which[i-2]="price";
                        content[i-2]=str;
                    }
                    else {
                        isvalid(-1);
                    } 
                }
                isvalid(library.Modify(which,content,cmd_cnt-1));
                
            }
        }else if(instruction=="select"){
            if(cmd_cnt!=2){
                isvalid(-1);
            }else{
                library.Select(cmd[2].c_str());
            }
        }else if(instruction=="show"){
            if(cmd_cnt==1){
                library.Showall();
            }else if(cmd[2]=="finance"){

            }else{
                if(cmd_cnt>=3){
                    isvalid(-1);
                }
                else if (cmd[2].size() >= 6 && cmd[2].substr(0, 6) == "-ISBN=") {
                    std::string str = cmd[2].substr(6);
                    library.Show("ISBN", str.c_str());
                }
                else if (cmd[2].size() >= 7 && cmd[2].substr(0, 7) == "-name=\"") {
                    std::string str = cmd[2].substr(7);
                    if (str.size() == 0 || str[str.size() - 1] != '\"') {
                        isvalid(-1);
                    }else{
                        str.pop_back();
                        // if (!CheckBookStr(str)) {
                        // throw error();
                        // }
                        library.Show("name", str.c_str());
                    }
                }else if (cmd[2].size() >= 9 && cmd[2].substr(0, 9) == "-author=\"") {
                    std::string str = cmd[2].substr(9);
                    if (str.size() == 0 || str[str.size() - 1] != '\"') {
                        isvalid(-1);
                    }else{
                        str.pop_back();
                        // if (!CheckBookStr(str)) {
                        //     throw error();
                        // }
                        library.Show("author", str.c_str());
                    }
                }
                else if (cmd[2].size() >= 10 && cmd[2].substr(0, 10) == "-keyword=\"") {
                    std::string str = cmd[2].substr(10);
                    if (str.size() == 0 || str[str.size() - 1] != '\"') {
                        isvalid(-1);
                    }else{
                        str.pop_back();
                        // if (!CheckBookStr(str)) {
                        //     throw error();
                        // }
                        if (str.find('|') != std::string::npos) {
                            isvalid(-1);
                        }else{
                            library.Show("keyword", str.c_str());
                        }
                    }
                }
                else {
                    isvalid(-1);
                }
            }
            
        }
        
        
        
        else{
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
    