#include "JudgerException.h"

JudgerException::JudgerException(int line, const string errInfo):
m_info(errInfo), m_line(line)
{
    m_info = "JudgerException: Line:";
    m_info += to_string(line);
    m_info.push_back(' ');
    m_info += errInfo;
}

JudgerException::~JudgerException()
{
}

const char* JudgerException::what() const throw()
{
    return m_info.c_str();
}