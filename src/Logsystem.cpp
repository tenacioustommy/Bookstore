#include"Logsystem.h"
Logsystem logsys;
void Logsystem::Add(double inc,double outc){
    opt tmp(inc,outc);
    file.seekp(4+sizeof(tmp)*totalcount);
    file.write(reinterpret_cast<char*>(&tmp),sizeof(tmp));
    totalcount++;
}
void Logsystem::show(){
    if(totalcount==0){
        std::cout<<"+ 0.00 - 0.00\n";
        return;
    }
    file.seekg(4);
    double totalin=0,totalout=0;
    opt tmp;
    for(int i=0;i<totalcount;++i){
        file.read(reinterpret_cast<char*>(&tmp),sizeof(tmp));
        totalin+=tmp.inc;
        totalout+=tmp.outc;
    }
    std::cout<<"+ "<<totalin<<" - "<<totalout<<'\n';
}
int Logsystem::show(int count){
    if(count>totalcount){
        return -1;
    }else if(count==0){
        std::cout<<"\n";
    }else{
        double totalin=0,totalout=0;
        opt tmp;
        file.seekg(4+sizeof(tmp)*(totalcount-count));
        for(int i=totalcount-count;i<totalcount;++i){
            file.read(reinterpret_cast<char*>(&tmp),sizeof(tmp));
            totalin+=tmp.inc;
            totalout+=tmp.outc;
        }   
        std::cout<<"+ "<<totalin<<" - "<<totalout<<'\n';
    }
    return 0;
}
Logsystem::Logsystem(/* args */)
{   
    std::cout<<std::fixed<<std::setprecision(2);
    file.open(filename,std::ios::in|std::ios::binary|std::ios::out );
    if (!file) {
        // 创建一个文件
        file.open(filename, std::fstream::out);
        file.close();
        file.open(filename);
        totalcount = 0;
        file.seekp(0);
        file.write(reinterpret_cast<char *>(&totalcount), sizeof(totalcount));
    }
    else {
        file.seekg(0);
        file.read(reinterpret_cast<char *>(&totalcount), sizeof(totalcount));
    }
}

Logsystem::~Logsystem()
{
    file.seekp(0);
    file.write(reinterpret_cast <char *> (&totalcount), sizeof(totalcount));
    file.close();
}
