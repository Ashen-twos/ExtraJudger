#pragma once
#include <bits/stdc++.h>

using namespace std;

class JudgerException
{
private:
    string m_info;
    int m_line;
public:
    string error;
    JudgerException(int line, const string errInfo);
    ~JudgerException();
    const char * what() const throw();
    int GetLine();
    void UpdateLine(int line);
};

