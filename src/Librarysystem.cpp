#include"Librarysystem.h"
Librarysystem library;
std::ostream& operator<<(std::ostream& out,Book& book){
    out<<book.isbn<<"\t"<<book.bookname<<"\t"<<book.author<<"\t"<<book.keyword<<"\t"<<std::fixed<<std::setprecision(2)<<book.price<<"\t"<<book.quantity<<"\n";
    return out;
}

int Librarysystem::insert(Book& book,int pos){
        if(pos==-1){
           file.seekp(0,std::ios::end); 
        }else{
            file.seekp(pos,std::ios::beg);
        }
        pos=file.tellp();
        file.write(reinterpret_cast<char*>(&book),sizeofbook);
        booknum++;
        return pos;
    }
    void Librarysystem::getbook(Book& book,int pos){
        file.seekg(pos);
        file.read(reinterpret_cast<char*>(&book),sizeofbook);
    }
    void Librarysystem::update(){
        
        file.seekp(bookstack.top().second,std::ios::beg);
        file.write(reinterpret_cast<char*>(&bookstack.top().first),sizeofbook);
    }
    /* data */

    int Librarysystem::buy(ISBN isbn,int quantity){
        if(quantity<1){
            return -1;
        }
        Book book;
        int pos;
        if(ISBNindex.find(isbn,pos)==-1){
            return -1;
        }else{
            getbook(book,pos);
            if(book.quantity<quantity){
                return -1;
            }
            book.quantity-=quantity;
            logsys.Add(quantity*book.price,0);
            std::cout<<std::fixed<<std::setprecision(2)<<quantity*book.price<<'\n';
            file.seekp(pos);
            file.write(reinterpret_cast<char*>(&book),sizeofbook);
        }
        return 0;
    }
    void Librarysystem::Show(std::string which,std::string content){
            std::vector<int> vec;
            std::vector<Book> vecbook;
            if(which=="all"){
                ISBNindex.findall(vec);
            }else if(which=="ISBN"){
                ISBNindex.findall(ISBN(content),vec);
            }else if(which=="name"){         
                NAMEindex.findall(string60(content),vec);
            }else if(which=="author"){
                Authorindex.findall(string60(content),vec);
            }else if(which=="keyword"){
                Keywordindex.findall(string60(content),vec);
            }
            Book book;
            if(vec.empty()){
                std::cout<<'\n';
            }else{
                for(auto it=vec.begin();it!=vec.end();++it){
                getbook(book,*it);
                vecbook.push_back(book);
                } 
                std::sort(vecbook.begin(),vecbook.end(),Book_cmp());
                for(auto it=vecbook.begin();it!=vecbook.end();it++){
                    std::cout<<*it;
                }
            }  
    }
    int Librarysystem::Import(int quantity,double totalcost){
        if(bookstack.top().second==-1){
            return -1;
        }
        if(totalcost<=0){
            return -1;
        }
        bookstack.top().first.quantity+=quantity;
        logsys.Add(0,totalcost);
        update();
        return 0;
        
    }
    void Librarysystem::Select(std::string isbn){
        int tmp=ISBNindex.find(ISBN(isbn),bookstack.top().second);
        if(tmp!=-1){
            getbook(bookstack.top().first,bookstack.top().second);
        }else{
            bookstack.top().first=Book(ISBN(isbn));
            bookstack.top().second=insert(bookstack.top().first);
            ISBNindex.insertunique(ISBN(isbn),bookstack.top().second);
        }
    }
    int Librarysystem::Modify(const std::string* which,const std::string* content,int count){
        if(bookstack.top().second==-1){
            return -1;
        }
        for(int i=0;i<count;i++){
            if(which[i]=="ISBN"){
                ISBN tmp(content[i]);
                if(tmp==bookstack.top().first.isbn){
                    return -1;
                }else{
                    if(ISBNindex.insertunique(tmp,bookstack.top().second).second==false){
                        return -1;
                    }
                    ISBNindex.remove(bookstack.top().first.isbn,bookstack.top().second);
                    bookstack.top().first.isbn=tmp;
                    
                }
            }else if(which[i]=="name"){
                string60 tmp(content[i]);
                if(tmp!=bookstack.top().first.bookname){
                    NAMEindex.remove(bookstack.top().first.bookname,bookstack.top().second);
                    bookstack.top().first.bookname=tmp;
                    NAMEindex.insert(tmp,bookstack.top().second);
                }        
            }else if(which[i]=="author"){
                string60 tmp(content[i]);
                if(tmp!=bookstack.top().first.author){
                    Authorindex.remove(bookstack.top().first.author,bookstack.top().second);
                    bookstack.top().first.author=tmp;
                    Authorindex.insert(tmp,bookstack.top().second);
                }        
            }else if(which[i]=="keyword"){

                string60 tmp(content[i]);
                if(tmp!=bookstack.top().first.keyword){
                    //remove existed key
                    std::string str(bookstack.top().first.keyword.buf);
                    std::string newstr=content[i];
                    char separator = '|';  // 使用逗号作为分隔符
                    if(newstr[0]==separator||newstr[newstr.size()-1]==separator){
                        return -1;
                    }
                    int start = 0, end = 0;
                    if(!str.empty()){
                        while ((end = str.find(separator, start)) != std::string::npos) {
                            std::string token = str.substr(start, end - start);
                            start = end + 1;
                            Keywordindex.remove(string60(token),bookstack.top().second);
                        }
                        Keywordindex.remove(string60(str.substr(start)),bookstack.top().second);
                    }
                    
                    //input new key
                    start=0;
                    end=0;
                    std::set<std::string> setstring;
                    while ((end = newstr.find(separator, start)) != std::string::npos) {
                        std::string token = newstr.substr(start, end - start);
                        start = end + 1;
                        if(token.size()==0||setstring.find(token)!=setstring.end()){
                            return -1;
                        }
                        setstring.insert(token);
                        Keywordindex.insert(string60(token),bookstack.top().second);
                    }
                    Keywordindex.insert(string60(newstr.substr(start)),bookstack.top().second);
                    bookstack.top().first.keyword=tmp;
                }  
            }else if(which[i]=="price"){
                bookstack.top().first.price=std::stod(content[i]);
            }else{
                throw std::exception();
            }
            
        }
        update();
        return 0;
    }
    Librarysystem::Librarysystem(/* args */)
    {
        ISBNindex.init("ISBNindex");
        NAMEindex.init("NAMEindex");
        Authorindex.init("Authorindex");
        Keywordindex.init("keywordindex");
        
        
        file.open(filename,std::ios::binary|std::ios::out|std::ios::in);
        if(!file){
            file.open(filename,std::ios::out);
            file.close();
            file.open(filename,std::ios::binary|std::ios::out|std::ios::in);
            booknum=0;
            file.clear();
            file.seekp(0,std::ios::beg);
            file.write(reinterpret_cast<char*>(&booknum),sizeof(booknum));
        }else{
            file.seekg(0);
            file.read(reinterpret_cast<char*>(&booknum),sizeof(booknum));
        }
        if(!file){
            std::cout<<"error!";
        }
        
    }
    Librarysystem::~Librarysystem(){
        file.seekp(0);
        file.write(reinterpret_cast<char*>(&booknum),sizeof(booknum));
        file.close();
    }