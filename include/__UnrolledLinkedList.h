#ifndef STORE_H
#define STORE_H
#include<iostream>
#include<fstream>
#include<cstring>
#define N 600

struct string {
  char buf[65];
  bool operator< (const string &rhs) const {
    return strcmp(buf, rhs.buf) < 0;
  }
  bool operator== (const string &rhs) const {
    return strcmp(buf, rhs.buf) == 0;
  }
  bool operator!= (const string &rhs) const {
    return strcmp(buf, rhs.buf) != 0;
  }
};
//key and value must have fixed size
template<class Key,class Value>
class UnrolledLinkedList{
private: 
    char* filename;
    std::fstream file;
    bool less(const std::pair<Key,Value>& a,const std::pair<Key,Value>& b){
        if(a.first<b.first){
            return 1;
        }else if(a.first==b.first){
            if(a.second<b.second){
                return 1;
            }else{return 0;}
        }else{
            return 0;
        }
    }
    int insert(std::pair<Key,Value>& tmp,std::pair<Key,Value>* vec,int& size){
        int i;
        bool flag=false;
        for(i=0;i<size+1;++i){
            if((i==size||less(tmp,vec[i]))&&(i==0||less(vec[i-1],tmp))){
                for(int j=size;j>i;--j){
                    vec[j]=vec[j-1];
                }
                flag=true;
                break;
            }
        }
        if(flag){
            size++;
            vec[i]=tmp;
            return i;
        }else{
            return -1;
        }
        
        
    }
    void read(std::pair<Key,Value>* vec,int& next,int& size){
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        file.read(reinterpret_cast<char*>(&size),sizeof(size));
        file.read(reinterpret_cast<char*>(vec), sizeof(std::pair<Key,Value>)*size);
        file.seekg(-sizeof(int)*2-size*sizeof(std::pair<Key,Value>),std::ios::cur);
    }


    void write(std::pair<Key,Value>* vec,int& next,int& size,bool isndsupplement){
        file.write(reinterpret_cast<char*>(&next),sizeof(next));
        file.write(reinterpret_cast<char*>(&size),sizeof(size));
        file.write(reinterpret_cast<char*>(vec), sizeof(std::pair<Key,Value>)*size);
        if(isndsupplement){
            file.write(reinterpret_cast<char*>(vec), sizeof(std::pair<Key,Value>)*(N-size));
        }
        
    }

public:
    UnrolledLinkedList()=default;
    UnrolledLinkedList(const char* name){
        filename=new char[strlen(name)+1];
        strcpy(filename,name);
        file.open(filename,std::ios::binary|std::ios::out|std::ios::in);
        if(!file){
            file.open(filename,std::ios::out);
            file.close();
            file.open(filename,std::ios::binary|std::ios::out|std::ios::in);
        }
        if(!file){
            std::cout<<"error!";
        }
        if(file.peek()==std::ios::traits_type::eof()){
            int next=-1;
            file.clear();
            file.seekp(0,std::ios::beg);
            file.write(reinterpret_cast<char*>(&next),sizeof(next));
        }
    }
    void insert(Key index,Value value){
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
                if(next==-1||less(tmp,vec[size-1])||tmp==vec[size-1]){
                    int i=insert(tmp,vec,size);
                    if(i!=-1){
                        file.seekp(readpos+sizeof(next),std::ios::beg);
                        file.write(reinterpret_cast<char*>(&size),sizeof(size));
                        file.seekp(i*sizeof(std::pair<Key,Value>),std::ios::cur);
                        file.write(reinterpret_cast<char*>(vec+i), sizeof(std::pair<Key,Value>)*(size-i));
                        
                    }
                    return;
                }else{
                    file.seekg(next,std::ios::beg);
                }
        }
    }
    
    void remove(Key index,Value value){
        int lastpos=0;
        int next,size;
        std::pair<Key,Value> vec[N];
        std::pair<Key,Value> tmp(index,value);
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
                if(next==-1||less(tmp,vec[size-1])||tmp==vec[size-1]){
                    for(int i=0;i<size;i++){
                        if(tmp==vec[i]){
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
    bool find(Key index){
        int next,size;
        bool flag=false;
        std::pair<Key,Value> vec[N];
        file.clear();
        file.seekg(0,std::ios::beg);
        file.read(reinterpret_cast<char*>(&next),sizeof(next));
        if(next==-1){
            std::cout<<"null\n";
            return 0;
        }
        while(1){
            // 从文件中读取向量数据
                file.seekg(next,std::ios::beg);
                read(vec,next,size);
                if(next==-1||index<vec[size-1].first||index==vec[size-1].first){
                    for(int i=0;i<size;i++){
                        if(index==vec[i].first){
                            std::cout<<vec[i].second<<' ';
                            flag=true;
                        }else{
                            if(flag){
                                break;
                            }
                        }
                        
                    }
                }
                if(next==-1){
                    break;
                }else{
                    file.seekg(next,std::ios::beg);
                }
                
        }
        if(flag==true){
            std::cout<<"\n";
            return 1;
        }else{
            std::cout<<"null\n";
            return 0;
        }    
    }
    ~UnrolledLinkedList(){file.close();}
    
};
#endif

