#ifndef LOG_H
#define LOG_H
#include"Accountsystem.h"
struct opt{
    double inc;
    double outc;
    opt()=default;
    opt(double p, double q) { inc = p, outc = q;}
};
class Logsystem
{
private:
    int totalcount;
    std::fstream file;
    const char* filename="Log";
public:
    void Add(double inc,double outc);
    void show();
    int show(int count);
    Logsystem(/* args */);
    ~Logsystem();
};

extern Logsystem logsys;

#endif