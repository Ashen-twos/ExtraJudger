#pragma once
#include <bits/stdc++.h>

using namespace std;

class Judger{
protected:
    string& m_Buff;
public:
    Judger()=delete;
    Judger(string& code):m_Buff(code){};
    ~Judger(){};
    virtual void judge()=0;
    bool pass;
};
