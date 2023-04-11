#include "StyleJudger.h"

using namespace spdlog;

StyleJudger::StyleJudger(string& code, const char* global_prefix, const char* white_list, int func_naming, 
    int global_naming, int local_naming, bool single_name): JudgerFSM(code),
    m_GlobalPrefix(global_prefix), m_FunctionNaming(func_naming), m_GlobalNaming(global_naming),
    m_LocalNaming(local_naming), m_SingleName(single_name)
{
    util::ParseString(white_list,m_WhiteList);
}

StyleJudger::~StyleJudger(){}

bool StyleJudger::CheckCamelCase(string& word)
{
    for(int i=0; i<word.length(); i++)
        if(util::CheckCharType(word[i]) ==  util::SYMBOL)
            return false;
    regex camelCase("[a-z]+([A-Z][a-z]+)+");
    return regex_search(word, camelCase);
}

void StyleJudger::CheckNaming(string& word, int style, bool global)
{
    if(style == CAMEL_CASE)
    {
        if(!CheckCamelCase(word))
            throw JudgerException(m_CurrentRow, (global ? m_GlobalPrefix : string("")) + word + ": 不合法的命名");
    }
    else if(style == UPPER_CASE)
    {
        for(int i=0; i<word.length(); i++)
            if(word[i] >= 'a' && word[i] <= 'z')
                throw JudgerException(m_CurrentRow, (global ? m_GlobalPrefix : string("")) + word + ": 不合法的命名");
    }
    else if(style == LOWER_CASE)
    {
        for(int i=0; i<word.length(); i++)
            if(word[i] >= 'A' && word[i] <= 'Z')
                throw JudgerException(m_CurrentRow, (global ? m_GlobalPrefix : string("")) + word + ": 不合法的命名");
    }
}

void StyleJudger::WhenDefineFunction()
{
    if(m_VariableName == "main")
        return;
    CheckNaming(m_VariableName, m_FunctionNaming, false);
}

void StyleJudger::WhenDefineVariable()
{
    //常用变量名
    if(m_VariableName=="i" || m_VariableName=="j" || m_VariableName=="k" ||
            m_VariableName=="x" || m_VariableName=="y" || m_VariableName=="z")
        return;
    //白名单
    if(m_WhiteList.count(m_VariableName))
        return;
    if(m_SingleName && m_VariableName.length() == 1)
        throw JudgerException(m_CurrentRow, m_VariableName + ": 过短的命名");
    //全局变量
    if(m_CurrentScope == "")
    {
        if(m_GlobalPrefix.length())
        {
            if(m_VariableName.substr(0,m_GlobalPrefix.length()) != m_GlobalPrefix)
                throw JudgerException(m_CurrentRow, m_VariableName + ": 缺少前缀" + m_GlobalPrefix);
            m_VariableName = m_VariableName.substr(m_GlobalPrefix.length(),m_VariableName.length()-m_GlobalPrefix.length());
        }
        CheckNaming(m_VariableName, m_GlobalNaming, true);
        m_VariableName = m_GlobalPrefix + m_VariableName;
    }
    //局部变量
    else
    {
        CheckNaming(m_VariableName, m_LocalNaming, false);
    }
}

void StyleJudger::WhenDefineArray()
{
    //常用变量名
    if(m_VariableName=="i" || m_VariableName=="j" || m_VariableName=="k" ||
            m_VariableName=="x" || m_VariableName=="y" || m_VariableName=="z")
        return;
    //白名单
    if(m_WhiteList.count(m_VariableName))
        return;
    if(m_SingleName && m_VariableName.length() == 1)
        throw JudgerException(m_CurrentRow, m_VariableName + ": 过短的命名");
    //全局变量
    if(m_CurrentScope == "")
    {
        if(m_GlobalPrefix.length())
        {
            if(m_VariableName.substr(0,m_GlobalPrefix.length()) != m_GlobalPrefix)
                throw JudgerException(m_CurrentRow, m_VariableName + ": 缺少前缀" + m_GlobalPrefix);
            m_VariableName = m_VariableName.substr(m_GlobalPrefix.length(),m_VariableName.length()-m_GlobalPrefix.length());
        }
        CheckNaming(m_VariableName, m_GlobalNaming, true);
        m_VariableName = m_GlobalPrefix + m_VariableName;
    }
    //局部变量
    else
    {
        CheckNaming(m_VariableName, m_LocalNaming, false);
    }
}

void StyleJudger::judge()
{
    FSM();
    pass = true;
}






