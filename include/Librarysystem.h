#ifndef LIBRARY_H
#define LIBRARY_H
#include<set>
#include<algorithm>
#include<stack>
#include"UnrolledLinkedList.h"
#include"Logsystem.h"
    typedef string<20> ISBN;
    typedef string<60> string60;
struct Book{
    ISBN isbn;
    string60 bookname;
    string60 author;
    string60 keyword;
    double price=0;
    int quantity=0;
    Book(){}
    Book(ISBN isbn):isbn(isbn){}
    Book(ISBN isbn,string60 bookname,string60 author,string60 keyword,double price,int quantity=0):isbn(isbn),bookname(bookname),author(author),keyword(keyword),price(price),quantity(quantity){}
    
};
class Book_cmp{
public:
    bool operator()(const Book &p, const Book &q) {
        return p.isbn < q.isbn;
    }
};
class Librarysystem
{
private:
    int sizeofbook=sizeof(Book);
    UnrolledLinkedList<ISBN,int> ISBNindex;
    UnrolledLinkedList<string60,int> NAMEindex;
    UnrolledLinkedList<string60,int> Authorindex;
    UnrolledLinkedList<string60,int> Keywordindex;
    std::fstream file;
    const char* filename="Library";
    
    int booknum;
    
    //insert or revise book at certain pos,by default the last
    int insert(Book& book,int pos=-1);
    void getbook(Book& book,int pos);
    void update();
    /* data */
public:
    bool isuserselected=0;
    std::stack<std::pair<Book,int>> bookstack;
    int buy(ISBN isbn,int quantity);
    void Show(std::string which,std::string content=std::string());
    int Import(int quantity,double totalcost);
    void Select(std::string isbn);
    int Modify(const std::string* which,const std::string* content,int count);
    Librarysystem(/* args */);
    ~Librarysystem();
};
extern Librarysystem library;
#endif