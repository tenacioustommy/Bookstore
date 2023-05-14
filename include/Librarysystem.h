#ifndef LIBRARY_H
#define LIBRARY_H
#include<cmath>
#include"UnrolledLinkedList.h"
struct Book{
    string<60> bookname;
    string<60> author;
    string<60> keyword;
    float price;
    int quantity=0;
    Book(){}
    Book(string<60> bookname,string<60> author,string<60> keyword,float price,int quantity=0):bookname(bookname),author(author),keyword(keyword),price(price),quantity(quantity){}
};
class Librarysystem
{
    typedef string<20> ISBN;
    typedef string<60> string60;
private:
    UnrolledLinkedList<ISBN,Book> Library;
    int selectedpos=-1;
    Book selectedbook;
    ISBN selectedisbn;
    /* data */
public:
    void Showall(){

    }
    int Show(std::string which,std::string content){
            if(which=="ISBN"){
                
                
            }else if(which=="name"){         
            }else if(which=="author"){
            }else if(which=="keyword"){
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
            return 0;
        }else{
            return -1;
        }
    }
    void Select(const char* isbn){
        Book book;
        std::pair<int,bool> tmp=Library.insert(ISBN(isbn),book);
        selectedpos=tmp.first;
        if(tmp.second){
            selectedisbn=ISBN(isbn);
            selectedbook=book;
        }else{
            Library.readone(selectedpos,selectedisbn,selectedbook);
        }
    }
    int Modify(const std::string* which,const std::string* content,int count){
        if(selectedpos==-1){
            return -1;
        }
        for(int i=0;i<count;i++){
            if(which[i]=="ISBN"){
                ISBN tmp(content[i]);
                if(tmp==selectedisbn){
                    return -1;
                }else{
                    selectedisbn=tmp;
                }
            }else if(which[i]=="name"){
                selectedbook.bookname=string60(content[i]);            
            }else if(which[i]=="author"){
                selectedbook.author=string60(content[i]);
            }else if(which[i]=="keyword"){
                selectedbook.keyword=string60(content[i]);
            }else if(which[i]=="price"){
                selectedbook.price=std::stoi(content[i]);
            }else{
                return -1;
            }   
        }
        Library.revise(selectedpos,selectedisbn,selectedbook);
        return 0;
    }
    Librarysystem(/* args */);
    ~Librarysystem();
};



#endif