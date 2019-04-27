#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>
#include <graphics.h>

using namespace std;

struct Punct
{
    double x, y;
    Punct(double a=0, double b=0): x{a}, y{b} {}
    Punct operator+(const Punct &p) const
    {
        return Punct{x + p.x, y + p.y};
    }
    Punct operator-(const Punct &p) const
    {
        return Punct{x - p.x, y - p.y};
    }
    double produsVectorial(const Punct &p) const
    {
        return x * p.y - y * p.x;
    }
    double produsScalar(const Punct &p) const
    {
        return x * p.x + y * p.y;
    }
    double produsVectorial(const Punct &a, const Punct &b) const
    {
        return (a - *this).produsVectorial(b - *this);
    }
    double produsScalar(const Punct &a, const Punct &b) const
    {
        return (a - *this).produsScalar(b - *this);
    }
    double lungimePatrat() const
    {
        return this->produsScalar(*this);
    }
    bool operator<(const Punct &p) const
    {
        return x < p.x || (x == p.x && y < p.y);
    }
    friend istream& operator>>(istream &in, Punct &p)
    {
        return in >> p.x >> p.y;
    }
    friend ostream& operator<<(ostream &out, const Punct &p)
    {
        return out << p.x << ' ' << p.y;
    }
};

const int zoom = 100;
const Punct origine{0, 0};
void lineto(const Punct &p)
{
    lineto(zoom*p.x, zoom*p.y);
}
void line(const Punct &p1, const Punct &p2)
{
    line(zoom*p1.x, zoom*p1.y, zoom*p2.x, zoom*p2.y);
}

int semn(double v)
{
    return v>0? 1:(v==0? 0:-1);
}

bool doubleEquals(double a, double b, double epsilon = 0.001)
{
    return abs(a - b) < epsilon;
}

void punctInTriunghi(const Punct &a, const Punct &b, const Punct &c, const Punct &p)
{
    double arie1 = abs(a.produsVectorial(b, c));
    double arie2 = abs(p.produsVectorial(a, b)) + abs(p.produsVectorial(b, c)) + abs(p.produsVectorial(c, a));
    if (doubleEquals(arie1, arie2))
        outtextxy(50,100,"Punctul este in interior.");
    else
        outtextxy(50,100,"Punctul este in exterior.");
}

void pregateste(vector<Punct> &poligon, vector<Punct> &vectori)
{
    if(poligon[0].produsVectorial(poligon[1], poligon[2]) < 0)
        reverse(poligon.begin(), poligon.end());
    int n = poligon.size();
    int pozitie = 0;
    for(int i=1; i<n; ++i)
        if(poligon[i] < poligon[pozitie])
            pozitie = i;
    rotate(poligon.begin(), poligon.begin()+pozitie, poligon.end());
    for(int i = 0; i<n-1; i++)
    {
        vectori[i] = poligon[i+1]-poligon[0];
        lineto(vectori[i]);
        delay(500);
    }
    lineto(origine);
}

void punctInPoligonConvex(const vector<Punct> &vectori, const Punct &p)
{
    int n = vectori.size();
    double vOP = vectori[0].produsVectorial(p);
    double vNP = vectori[n-1].produsVectorial(p);
    if(vOP!=0 && semn(vOP)!=semn(vectori[0].produsVectorial(vectori[n-1])))
        outtextxy(50,100,"Punctul este in exterior.");
    else if(vNP!=0 && semn(vNP)!=semn(vectori[n-1].produsVectorial(vectori[0])))
        outtextxy(50,100,"Punctul este in exterior.");
    else if(vOP == 0)
    {
        if(vectori[0].lungimePatrat() >= p.lungimePatrat())
            outtextxy(50,100,"Punctul este pe laturi.");
        else
            outtextxy(50,100,"Punctul este in exterior.");
    }
    else if(vNP == 0)
    {
        if(vectori[n-1].lungimePatrat() >= p.lungimePatrat())
            outtextxy(50,100,"Punctul este pe laturi.");
        else
            outtextxy(50,100,"Punctul este in exterior.");
    }
    else
    {
        int s=0, d = n-1;
        while(d-s > 1)
        {
            int mijloc = (s+d)/2;
            if(vectori[mijloc].produsVectorial(p) >= 0)
            {
                s = mijloc;
                line(origine, vectori[s]);
                delay(1000);
            }
            else
            {
                d = mijloc;
                line(origine, vectori[d]);
                delay(1000);
            }
        }
        if(p.produsVectorial(vectori[s], vectori[d]) == 0)
            outtextxy(50,100,"Punctul este pe laturi.");
        else
            punctInTriunghi(vectori[s], vectori[d], Punct(0, 0), p);
    }
}

int main()
{
    initwindow(300, 300, "Reprezentare grafica");
    ifstream fin("puncte.in");
    int n;
    fin >> n;
    vector<Punct> poligon(n), vectori(n-1);
    for(int i=0; i<n; ++i)
        fin >> poligon[i];
    Punct p;
    fin >> p;
    circle(zoom*p.x, zoom*p.y, 1);
    pregateste(poligon, vectori);
    punctInPoligonConvex(vectori, p-poligon[0]);
    while(!kbhit())
        delay(1);
    return 0;
}
