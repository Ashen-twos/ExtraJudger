#include "StyleJudger.h"

using namespace spdlog;

StyleJudger::StyleJudger(string& code, const char* global_prefix, const char* white_list, int func_naming, 
    int global_naming, int local_naming, bool single_name): JudgerFSM(code),
    m_GlobalPrefix(global_prefix), m_FunctionNaming(func_naming), m_GlobalNaming(global_naming),
    m_LocalNaming(local_naming), m_SingleName(single_name),m_GNamePass(true), m_FNamePass(true), m_LNamePass(true), m_PrefixPass(true), m_SinglePass(true)
{
    util::ParseString(white_list,m_WhiteList);
    pass = 5;
    if(!single_name)
        pass--;
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

void StyleJudger::CheckNaming(string& word, int style, int type)
{
    if(style == CAMEL_CASE)
    {
        if(!CheckCamelCase(word))
        {
            if(type == 1)
                m_GNamePass = false;
            else if(type == 2)
                m_LNamePass = false;
            else
                m_FNamePass = false;
            SetMessage(m_CurrentRow, word + ": 不合法的命名");
            return;
        }
    }
    else if(style == UPPER_CASE)
    {
        for(int i=0; i<word.length(); i++)
        {
            if(word[i] >= 'a' && word[i] <= 'z')
            {
                if(type == 1)
                    m_GNamePass = false;
                else if(type == 2)
                    m_LNamePass = false;
                else
                    m_FNamePass = false;
                SetMessage(m_CurrentRow, word + ": 不合法的命名");
                return;
            }
        }

    }
    else if(style == LOWER_CASE)
    {
        for(int i=0; i<word.length(); i++)
        {
            if(word[i] >= 'A' && word[i] <= 'Z')
            {
                if(type == 1)
                    m_GNamePass = false;
                else if(type == 2)
                    m_LNamePass = false;
                else
                    m_FNamePass = false;
                SetMessage(m_CurrentRow, word + ": 不合法的命名");
                return;
            }
        }
    }
}

void StyleJudger::WhenDefineFunction()
{
    if(m_VariableName == "main")
        return;
    //白名单
    if(m_WhiteList.count(m_VariableName))
        return;
    CheckNaming(m_VariableName, m_FunctionNaming, 3);
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
    {
        m_SinglePass = false;
        SetMessage(m_CurrentRow, m_VariableName + ": 过短的命名");
    }
    //全局变量
    if(m_CurrentScope == "")
    {
        if(m_GlobalPrefix.length() )
        {
            if((m_GlobalPrefix.length() >= m_VariableName.length()) || (m_VariableName.substr(0,m_GlobalPrefix.length()) != m_GlobalPrefix))
            {
                m_PrefixPass = false;
                SetMessage(m_CurrentRow, m_VariableName + ": 缺少前缀" + m_GlobalPrefix);
            }
            if(m_VariableName.length() > m_GlobalPrefix.length())
                m_VariableName = m_VariableName.substr(m_GlobalPrefix.length());
        }
        CheckNaming(m_VariableName, m_GlobalNaming, 1);
        m_VariableName = m_GlobalPrefix + m_VariableName;
    }
    //局部变量
    else
    {
        CheckNaming(m_VariableName, m_LocalNaming, 2);
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
    {
        m_SingleName = false;
        SetMessage(m_CurrentRow, m_VariableName + ": 过短的命名");
    }
    //全局变量
    if(m_CurrentScope == "")
    {
        if(m_GlobalPrefix.length() )
        {
            if((m_GlobalPrefix.length() >= m_VariableName.length()) || (m_VariableName.substr(0,m_GlobalPrefix.length()) != m_GlobalPrefix))
            {
                m_PrefixPass = false;
                SetMessage(m_CurrentRow, m_VariableName + ": 缺少前缀" + m_GlobalPrefix);
            }
            if(m_VariableName.length() > m_GlobalPrefix.length())
                m_VariableName = m_VariableName.substr(m_GlobalPrefix.length());
        }
        CheckNaming(m_VariableName, m_GlobalNaming, 1);
        m_VariableName = m_GlobalPrefix + m_VariableName;
    }
    //局部变量
    else
    {
        CheckNaming(m_VariableName, m_LocalNaming, 2);
    }
}

void StyleJudger::judge()
{
    FSM();
    if(!m_GNamePass)
        pass--;
    if(!m_FNamePass)
        pass--;
    if(!m_LNamePass)
        pass--;
    if(!m_SinglePass)
        pass--;
    if(!m_PrefixPass)
        pass--;
    if(m_GNamePass && m_FNamePass && m_LNamePass && m_SinglePass && m_PrefixPass)
        result = "success";
}






