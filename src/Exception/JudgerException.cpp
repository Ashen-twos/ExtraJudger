#include "JudgerException.h"

JudgerException::JudgerException(int line, const string errInfo):
m_info(errInfo), m_line(line), error(""){}

JudgerException::~JudgerException(){}

const char* JudgerException::what() const throw()
{
    return error.c_str();
}

int JudgerException::GetLine()
{
    return m_line;
}

void JudgerException::UpdateLine(int line)
{
    m_line = line;
    error = "JudgerException: Line:";
    error += to_string(m_line);
    error.push_back(' ');
    error += m_info;
}