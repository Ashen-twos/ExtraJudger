#include "FunctionJudger.h"

using namespace spdlog;

FunctionJudger::FunctionJudger(string& code, const char* black_list, const char* white_list,
    int max_statement, bool disableIO) : JudgerFSM(code), m_MaxOfStatement(max_statement), m_DisableIO(disableIO),
    m_WhitePass(true), m_BlackPass(true), m_StatementPass(true), m_IOPass(true)

{
    util::ParseString(black_list, m_FunctionBlacklist);
    util::ParseString(white_list, m_FunctionWhitelist);
    pass = 4;
    if(!disableIO)
        pass--;
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
    {
        m_BlackPass = false;
        SetMessage(m_CurrentRow, m_VariableName + "() 不应使用的函数");
        return;
    }
    
    // 白名单
    if(m_FunctionWhitelist.count(m_VariableName))
        m_FunctionWhitelist[m_VariableName] = 0;
    
    // IO
    if(m_DisableIO && m_CurrentScope != "" && m_CurrentScope != "main")
    {
        if(m_VariableName == "printf" || m_VariableName == "scanf" || m_VariableName == "read" || m_VariableName == "write" ||
           m_VariableName == "getchar" || m_VariableName == "putchar" || m_VariableName == "gets" || m_VariableName == "puts" ||
           m_VariableName == "sgets")
        {
            m_IOPass = false;
            SetMessage(m_CurrentRow, m_VariableName + "() 函数内禁止IO");
        }
    }
}

void FunctionJudger::WhenStatementEnd()
{
    // 函数内
    if(m_CurrentScope != "" && !m_InFor && m_CurrentScope != "main")
    {
        m_SumOfStatement[m_CurrentScope]++;
        if(m_SumOfStatement[m_CurrentScope] > m_MaxOfStatement)
        {
            if(m_StatementPass)
                SetMessage(m_CurrentRow, m_CurrentScope + "函数内语句过多");
            m_StatementPass = false;
        }
    }
}

void FunctionJudger::judge()
{
    FSM();
    for(auto iter=m_FunctionWhitelist.begin(); iter!=m_FunctionWhitelist.end(); iter++)
        if(iter->second > 0)
        {
            m_WhitePass = false;
            result += "未使用的函数: " + iter->first + "()\n";
        }
    if(!m_WhitePass)
        pass--;
    if(!m_BlackPass)
        pass--;
    if(!m_StatementPass)
        pass--;
    if(!m_IOPass)
        pass--;
    if(m_WhitePass && m_BlackPass && m_StatementPass && m_IOPass)
        result = "success";
}




