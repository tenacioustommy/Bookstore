#ifndef STORE_H
#define STORE_H
#include<iostream>
#include<fstream>
#include<cstring>
#include<sstream>
#include<vector>
#include<iomanip>
#define N 600

template<int SIZE>
struct string {
    char buf[SIZE+1];
    string(){buf[0]='\0';}
    string(const char* str){
        strncpy(buf,str,SIZE);
        buf[SIZE]='\0';}
    string(const std::string str){
        strcpy(buf,str.c_str());
    }
    bool operator< (const string &rhs) const {
        return strcmp(buf, rhs.buf) < 0;
    }
    bool operator> (const string &rhs) const {
        return strcmp(buf, rhs.buf) > 0;
    }
    bool operator>= (const string &rhs) const {
        return strcmp(buf, rhs.buf) >= 0;
    }
    bool operator<= (const string &rhs) const {
        return strcmp(buf, rhs.buf) <= 0;
    }
    bool operator== (const string &rhs) const {
        return strcmp(buf, rhs.buf) == 0;
    }
    bool operator!= (const string &rhs) const {
        return strcmp(buf, rhs.buf) != 0;
    }
    
    void operator= (const string &rhs){
        strncpy(buf,rhs.buf,SIZE);
        buf[SIZE]='\0';
    }
};
template<int SIZE>
std::ostream& operator<<(std::ostream& out,const string<SIZE>& str){
    out<<str.buf;
    return out;
}
//key and value must have fixed size
template<class Key,class Value>
class UnrolledLinkedList{
private: 
    typedef std::pair<Key,Value> Mypair;
    std::string filename;
    std::fstream file;
    int sizeofpair=sizeof(Mypair);

    std::pair<int,bool> insert(const Mypair& tmp,Mypair* vec,int& size){
        int i,index;
        bool flag=false;
        for(i=0;i<size+1;++i){
            if(i==size||tmp.first<vec[i].first){
                if(i==0||vec[i-1].first<tmp.first){
                    for(int j=size;j>i;--j){
                        vec[j]=vec[j-1];
                    }
                    size++;
                    vec[i]=tmp;
                    flag=true;
                    index=i;
                    //insert success
                }else{
                    index=i-1;
                }
                break;
            }
        }
        return std::pair<int,bool>(index,flag);
    }
    void read(Mypair* vec,int& next,int& size){
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        file.read(reinterpret_cast<char*>(&size),sizeof(size));
        file.read(reinterpret_cast<char*>(vec), sizeofpair*size);
        file.seekg(-sizeof(int)*2-size*sizeofpair,std::ios::cur);
    }


    void write(Mypair* vec,int& next,int& size,bool isndsupplement){
        file.write(reinterpret_cast<char*>(&next),sizeof(next));
        file.write(reinterpret_cast<char*>(&size),sizeof(size));
        file.write(reinterpret_cast<char*>(vec), sizeofpair*size);
        if(isndsupplement){
            file.write(reinterpret_cast<char*>(vec), sizeofpair*(N-size));
        }
        
    }

public:
    UnrolledLinkedList()=default;
    // UnrolledLinkedList(const char* name){
        
    //     file.open(filename,std::ios::out);
    //     file.close();
    //     file.open(filename,std::ios::binary|std::ios::out|std::ios::in);
    //     if(!file){
    //         std::cout<<"error!";
    //     }
    //     if(file.peek()==std::ios::traits_type::eof()){
    //         int next=-1;
    //         file.clear();
    //         file.seekp(0,std::ios::beg);
    //         file.write(reinterpret_cast<char*>(&next),sizeof(next));
    //     }
    // }
    void init(const char* name){
        filename=std::string(name);
        file.open(filename,std::ios::binary|std::ios::out|std::ios::in);
        if(!file){
            file.open(filename,std::ios::out);
            file.close();
            file.open(filename,std::ios::binary|std::ios::out|std::ios::in);
            int next=-1;
            file.seekp(0,std::ios::beg);
            file.write(reinterpret_cast<char*>(&next),sizeof(next));
        }
        if(!file){
            std::cout<<"error!";
        }
    }
    void readone(const int pos,Key& index,Value& value){
        Mypair mypair;
        file.seekg(pos,std::ios::beg);
        file.read(reinterpret_cast<char*>(&mypair),sizeofpair);
        index=mypair.first;
        value=mypair.second;
    }
    //index can be repeated,but can't both the same
    void insert(const Key& index,const Value& value){
        std::pair<Key,Value> vec[N];
        std::pair<Key,Value> tmp(index,value);
        int next,size=0;
        file.seekg(0,std::ios::beg);
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        if(next==-1){
                vec[0]=tmp;
            // 将向量数据本身写入文件
                next=4;
                int tmpint=-1;
                size=1;
                write(vec,tmpint,size,1);
            //change init pos
                file.seekp(0,std::ios::beg);
                file.write(reinterpret_cast<char*>(&next),sizeof(next));
                return;
                
        }else{
            file.seekg(next,std::ios::beg);
        }
        while(1){
                read(vec,next,size);
                int readpos=file.tellg();
            // 从文件中读取向量数据
                if(size==N){
                    std::pair<Key,Value> newvec[N];
                    for(int i=0;i<N/2;++i){
                        newvec[i]=vec[i+N/2];
                    }
                    size=N/2;
                    file.seekp(0,std::ios::end);
                    int newnext=file.tellp();
                    write(newvec,next,size,1);
                    
                    file.seekp(readpos,std::ios::beg);
                    file.write(reinterpret_cast<char*>(&newnext),sizeof(newnext));
                    file.write(reinterpret_cast<char*>(&size),sizeof(size));
                    next=newnext;
                }
                if(next==-1||index<=vec[size-1].first){
                    int i;
                    for(i=0;i<size;i++){
                        if(index<vec[i].first){
                            break;
                        }
                    }
                    for(int j=size;j>i;--j){
                        vec[j]=vec[j-1];
                    }
                    vec[i]=tmp;
                    size++;
                    file.seekp(readpos+sizeof(next),std::ios::beg);
                    file.write(reinterpret_cast<char*>(&size),sizeof(size));
                    file.seekp(i*sizeof(std::pair<Key,Value>),std::ios::cur);
                    file.write(reinterpret_cast<char*>(vec+i), sizeof(std::pair<Key,Value>)*(size-i));
                    return;
                }else{
                    file.seekg(next,std::ios::beg);
                }
        }
    }
    /*maybe insert repeated data, index is unique
    success then return pair of pos of element 
    fail then return pair of pos of existed element
    both including key and value!!
    */
    std::pair<int,bool> insertunique(const Key& index,const Value& value){
        Mypair vec[N];
        Mypair tmp(index,value);
        int next,size=0;
        file.seekg(0,std::ios::beg);
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        if(next==-1){
                vec[0]=tmp;
            // 将向量数据本身写入文件
                next=4;
                int tmpint=-1;
                size=1;
                write(vec,tmpint,size,1);
            //change init pos
                file.seekp(0,std::ios::beg);
                file.write(reinterpret_cast<char*>(&next),sizeof(next));
                return std::pair<int,bool>(next+2*sizeof(int),true);
                
        }else{
            file.seekg(next,std::ios::beg);
        }
        while(1){
                read(vec,next,size);
                int readpos=file.tellg();
            // 从文件中读取向量数据
                if(size==N){
                    Mypair newvec[N];
                    for(int i=0;i<N/2;++i){
                        newvec[i]=vec[i+N/2];
                    }
                    size=N/2;
                    file.seekp(0,std::ios::end);
                    int newnext=file.tellp();
                    write(newvec,next,size,1);
                    
                    file.seekp(readpos,std::ios::beg);
                    file.write(reinterpret_cast<char*>(&newnext),sizeof(newnext));
                    file.write(reinterpret_cast<char*>(&size),sizeof(size));
                    next=newnext;
                }
                if(next==-1||tmp.first<vec[size-1].first||tmp.first==vec[size-1].first){
                    std::pair<int,bool> tmppair=insert(tmp,vec,size);
                    int pos,i=tmppair.first;
                    if(tmppair.second){
                        file.seekp(readpos+sizeof(next),std::ios::beg);
                        file.write(reinterpret_cast<char*>(&size),sizeof(size));
                        file.seekp(i*sizeofpair,std::ios::cur);
                        pos=file.tellp();
                        file.write(reinterpret_cast<char*>(vec+i), sizeofpair*(size-i));                        
                    }else{
                        file.seekp(readpos+2*sizeof(int)+(i-1)*sizeofpair,std::ios::cur);
                        pos=file.tellp();
                    }
                    return std::pair<int,bool>(pos,tmppair.second);
                
                }else{
                    file.seekg(next,std::ios::beg);
                }
        }
    }
    //remove certain one
    void remove(const Key& index,const Value& value){
        int lastpos=0;
        int next,size;
        std::pair<Key,Value> vec[N];
        file.seekg(0,std::ios::beg);
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        if(next==-1){
            return;
        }else{
            file.seekg(next,std::ios::beg);
        }
        while(1){
            // 从文件中读取向量数据
                read(vec,next,size);
                int readpos=file.tellg();
                if(next==-1||index<=vec[size-1].first){
                    for(int i=0;i<size;i++){
                        if(index==vec[i].first&&value==vec[i].second){
                            size--;
                            if(size==0){
                                file.seekp(lastpos,std::ios::beg);
                                file.write(reinterpret_cast<char*>(&next),sizeof(next));
                            }else{
                                for(int j=i;j<size;j++){
                                    vec[j]=vec[j+1];
                                }
                                file.seekp(readpos,std::ios::beg);
                                file.write(reinterpret_cast<char*>(&next),sizeof(next));
                                file.write(reinterpret_cast<char*>(&size),sizeof(size));
                                file.seekp(sizeof(std::pair<Key,Value>)*i,std::ios::cur);
                                file.write(reinterpret_cast<char*>(vec+i), sizeof(std::pair<Key,Value>)*(size-i));
                            }
                            break;
                        }
                    }
                    // write(vec,next,size,0);
                    return;
                }else{
                    file.seekg(next,std::ios::beg);
                    lastpos=next;
                }
        }
    }
    //maybe don't exist, remove nothing is possible

    bool remove(const Key& index){
        int lastpos=0;
        int next,size;
        Mypair vec[N];
        file.seekg(0,std::ios::beg);
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        if(next==-1){
            return 0;
        }else{
            file.seekg(next,std::ios::beg);
        }
        while(1){
            // 从文件中读取向量数据
                read(vec,next,size);
                int readpos=file.tellg();
                if(next==-1||index<vec[size-1].first||index==vec[size-1].first){
                    for(int i=0;i<size;i++){
                        if(index==vec[i].first){
                            size--;
                            if(size==0){
                                file.seekp(lastpos,std::ios::beg);
                                file.write(reinterpret_cast<char*>(&next),sizeof(next));
                            }else{
                                for(int j=i;j<size;j++){
                                    vec[j]=vec[j+1];
                                }
                                file.seekp(readpos,std::ios::beg);
                                file.write(reinterpret_cast<char*>(&next),sizeof(next));
                                file.write(reinterpret_cast<char*>(&size),sizeof(size));
                                file.seekp(sizeofpair*i,std::ios::cur);
                                file.write(reinterpret_cast<char*>(vec+i), sizeofpair*(size-i));
                            }
                            return 1;
                            // break;
                        }
                    } 
                    return 0;
                }else{
                    file.seekg(next,std::ios::beg);
                    lastpos=next;
                }
        }
    }
    void revise(int pos,const Key& index,const Value& value){
        file.seekp(pos,std::ios::beg);
        Mypair tmp(index,value);
        file.write(reinterpret_cast<char*>(&tmp),sizeofpair);
    }
    void findall(std::vector<Value>& vec){
        int next,size;
        Mypair pair[N];
        file.clear();
        file.seekg(0,std::ios::beg);
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        if(next==-1){
            return ;
        }else{
            file.seekg(next,std::ios::beg);
        }
        while(1){
            // 从文件中读取向量数据
                read(pair,next,size);
                for(int i=0;i<size;i++){
                        vec.push_back(pair[i].second);
                    }
                if(next==-1){
                    break;
                }else{
                    file.seekg(next,std::ios::beg);
                }
        }
        return ;
    }
    //find all value in vec,give no pos
    void findall(const Key& index,std::vector<Value>& vec){
        int next,size;
        bool flag=false;
        Mypair pair[N];
        file.clear();
        file.seekg(0,std::ios::beg);
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        if(next==-1){
            return ;
        }else{
            file.seekg(next,std::ios::beg);
        }
        while(1){
            // 从文件中读取向量数据
                read(pair,next,size);
                if(next==-1||(index<=pair[size-1].first&&index>=pair[0].first)){
                    for(int i=0;i<size;i++){
                        if(index==pair[i].first){
                            vec.push_back(pair[i].second);
                            flag=true;
                        }
                    }
                }else if(flag==true){
                    return;
                }
                if(next==-1){
                    break;
                }else{
                    file.seekg(next,std::ios::beg);
                }
        }
        return ;
    }
    //success then return pos of element,fail then return -1
    int find(const Key& index,Value& value){
        int next,size;
        bool flag=false;
        Mypair pair[N];
        file.clear();
        file.seekg(0,std::ios::beg);
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        if(next==-1){
            return -1;
        }else{
            file.seekg(next,std::ios::beg);
        }
        while(1){
            // 从文件中读取向量数据
                read(pair,next,size);
                if(next==-1||index<=pair[size-1].first){
                    for(int i=0;i<size;i++){
                        if(index==pair[i].first){
                            value=pair[i].second;
                            return file.tellg()+2*sizeof(next)+i*sizeofpair;
                        }
                    }
                    return -1;
                }
                if(next==-1){
                    break;
                }else{
                    file.seekg(next,std::ios::beg);
                }
        }
        return -1;
    }
    ~UnrolledLinkedList(){file.close();}
    
};
#endif