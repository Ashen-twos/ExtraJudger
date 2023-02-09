#pragma once
#include <bits/stdc++.h>

using namespace std;

class JudgerException
{
private:
    string m_info;
    int m_line;
public:
    JudgerException(int line, const string errInfo);
    ~JudgerException();
    const char * what() const throw();
};

