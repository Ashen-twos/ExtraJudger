#include "FunctionJudger.h"

using namespace spdlog;

FunctionJudger::FunctionJudger(string& code, const char* blacklist, const char* whitelist,
    int max_statement, bool disableIO) : JudgerFSM(code), m_MaxOfStatement(max_statement), m_DisableIO(disableIO)

{
    util::ParseString(blacklist, m_FunctionBlacklist);
    util::ParseString(whitelist, m_FunctionWhitelist);
}

FunctionJudger::~FunctionJudger(){}

void FunctionJudger::WhenDefineFunction()
{
    m_FunctionBlacklist[m_VariableName] = 0;
}

void FunctionJudger::WhenCallFunction()
{
    // 黑名单
    if(m_FunctionBlacklist[m_VariableName] == 1)
        throw JudgerException(m_CurrentRow, m_VariableName + "() 不应使用的函数");
    
    // 白名单
    if(m_FunctionWhitelist.count(m_VariableName))
        m_FunctionWhitelist[m_VariableName] = 0;
    
    // IO
    if(m_DisableIO && m_CurrentScope != "" && m_CurrentScope != "main")
    {
        if(m_VariableName == "printf" || m_VariableName == "scanf" || m_VariableName == "read" || m_VariableName == "write" ||
           m_VariableName == "getchar" || m_VariableName == "putchar" || m_VariableName == "gets" || m_VariableName == "puts" ||
           m_VariableName == "sgets")
            throw JudgerException(m_CurrentRow, m_VariableName + "() 函数内禁止IO");
    }
}

void FunctionJudger::WhenStatementEnd()
{
    // 函数内
    if(m_CurrentScope != "" && !m_InFor)
    {
        m_SumOfStatement[m_CurrentScope]++;
        if(m_SumOfStatement[m_CurrentScope] > m_MaxOfStatement)
            throw JudgerException(m_CurrentRow, m_CurrentScope + "函数内语句过多");
    }
    
}

void FunctionJudger::judge()
{
    FSM();
    for(auto iter=m_FunctionWhitelist.begin(); iter!=m_FunctionWhitelist.end(); iter++)
        if(iter->second > 0)
            throw JudgerException(m_CurrentRow, iter->first + "未使用的函数");
    pass = true;
}




