#ifndef LIBRARY_H
#define LIBRARY_H
#include<cmath>
#include"UnrolledLinkedList.h"

class Librarysystem
{
    typedef string<20> ISBN;
    typedef string<60> string60;
private:
    struct Book{
        ISBN isbn;
        string60 bookname;
        string60 author;
        string60 keyword;
        float price;
        int quantity=0;
        Book(){}
        Book(ISBN isbn):isbn(isbn){}
        Book(ISBN isbn,string60 bookname,string60 author,string60 keyword,float price,int quantity=0):isbn(isbn),bookname(bookname),author(author),keyword(keyword),price(price),quantity(quantity){}
        
    };
    int sizeofbook=sizeof(Book);
    UnrolledLinkedList<ISBN,int> ISBNindex;
    UnrolledLinkedList<string60,int> NAMEindex;
    UnrolledLinkedList<string60,int> Authorindex;
    UnrolledLinkedList<string60,int> Keywordindex;
    std::fstream file;
    const char* filename="Library";
    
    int booknum;
    int selectedpos=-1;
    Book selectedbook;
    //insert or revise book at certain pos,by default the last
    int insert(Book book,int pos=-1){
        if(pos==-1){
            pos=sizeof(booknum)+booknum*sizeofbook;
        }
        file.seekp(pos);
        file.tellp();
        file.write(reinterpret_cast<char*>(&book),sizeofbook);
        booknum++;
        return pos;
    }
    void getbook(Book& book,int pos){
        file.seekg(pos);
        file.read(reinterpret_cast<char*>(&book),sizeofbook);
    }
    void update(){
        insert(selectedbook,selectedpos);
    }
    /* data */
public:
    void Showall(){

    }
    int Show(std::string which,std::string content){
            std::vector<int> vec;
            if(which=="ISBN"){
                ISBNindex.findall(ISBN(content),vec);
            }else if(which=="name"){         
                NAMEindex.findall(string60(content),vec);
            }else if(which=="author"){
                Authorindex.findall(string60(content),vec);
            }else if(which=="keyword"){
                Keywordindex.findall(string60(content),vec);
            }else{
                return -1;
            }
    }
    int Import(int quantity,float totalcost){
        if(selectedpos==-1){
            return -1;
        }
        if(totalcost<=0){
            return -1;
        }
        int num=totalcost/quantity;
        if(fmod(totalcost,quantity)==0){
            selectedbook.quantity+=num;
            update();
            return 0;
        }else{
            return -1;
        }
    }
    void Select(std::string isbn){
        int tmp=ISBNindex.find(ISBN(isbn),selectedpos);
        if(tmp!=-1){
            getbook(selectedbook,selectedpos);
        }else{
            insert(Book(ISBN(isbn)));
        }
    }
    int Modify(const std::string* which,const std::string* content,int count){
        if(selectedpos==-1){
            return -1;
        }
        for(int i=0;i<count;i++){
            if(which[i]=="ISBN"){
                ISBN tmp(content[i]);
                if(tmp==selectedbook.isbn){
                    return -1;
                }else{
                    ISBNindex.remove(selectedbook.isbn,selectedpos);
                    selectedbook.isbn=tmp;
                    ISBNindex.insert(tmp,selectedpos);
                }
            }else if(which[i]=="name"){
                string60 tmp(content[i]);
                if(tmp!=selectedbook.bookname){
                    NAMEindex.remove(selectedbook.bookname,selectedpos);
                    selectedbook.bookname=tmp;
                    NAMEindex.insert(tmp,selectedpos);
                }        
            }else if(which[i]=="author"){
                string60 tmp(content[i]);
                if(tmp!=selectedbook.bookname){
                    Authorindex.remove(selectedbook.bookname,selectedpos);
                    selectedbook.bookname=tmp;
                    Authorindex.insert(tmp,selectedpos);
                }        
            }else if(which[i]=="keyword"){
                string60 tmp(content[i]);
                //todo  
            }else if(which[i]=="price"){
                selectedbook.price=std::stoi(content[i]);
            }
        }
        update();
        return 0;
    }
    Librarysystem(/* args */)
    {
        ISBNindex.init("ISBNindex");
        NAMEindex.init("NAMEindex");
        Authorindex.init("Authorindex");
        Keywordindex.init("keywordindex");
        file.open(filename,std::ios::out);
        file.close();
        file.open(filename,std::ios::binary|std::ios::out|std::ios::in);
        if(!file){
            std::cout<<"error!";
        }
        if(file.peek()==std::ios::traits_type::eof()){
            booknum=0;
            file.clear();
            file.seekp(0,std::ios::beg);
            file.write(reinterpret_cast<char*>(&booknum),sizeof(booknum));
        }
        file.seekg(0);
        file.read(reinterpret_cast<char*>(&booknum),sizeof(booknum));
    }


    ~Librarysystem(){
        file.close();
    }
};



#endif