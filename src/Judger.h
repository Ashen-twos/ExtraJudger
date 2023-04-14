#pragma once
#include <bits/stdc++.h>

using namespace std;

class Judger{
protected:
    string& m_Buff;
    void SetMessage(int line, string msg)
    {
        result += string("") + "Line " + to_string(line) + ": " + msg + "\n";
    }
public:
    Judger()=delete;
    Judger(string& code):m_Buff(code),result(""){};
    ~Judger(){};
    virtual void judge()=0;
    int pass;
    string result;
};
