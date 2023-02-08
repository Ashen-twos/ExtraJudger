#include <bits/stdc++.h>

using namespace std;

class JudgerException : public exception
{
private:
    string m_info;
public:
    JudgerException(const string errInfo);
    JudgerException(const char* errInfo);
    ~JudgerException();
};

JudgerException::JudgerException(const string errInfo):
m_info(errInfo){}

JudgerException::JudgerException(const char* errInfo):
m_info(string(errInfo)){}

JudgerException::~JudgerException()
{
}
