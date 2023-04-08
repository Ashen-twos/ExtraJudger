#include "FunctionJudger.h"

using namespace spdlog;

FunctionJudger::FunctionJudger(string& code, const char* func) : JudgerFSM(code)
{
    m_CurrentRow = 1;
    vector<string> vet;
    string tmp(func);
    int las = 0;
    for(int i=0; i<=tmp.length(); i++)
    {
        if(i==tmp.length() || tmp[i] == ' ')
        {
            if(i>las)
            {
                vet.push_back(tmp.substr(las,i-las));
                info("检测函数: {}",vet.back());
            }
            las = i+1;
        }
    }
    for(int i=0; i<vet.size(); i++)
        function_blacklist[vet[i]] = 1;
}

FunctionJudger::~FunctionJudger(){}

/* void FunctionJudger::judge()
{
     //左大括号数目
    int leftsum = 0;
    //变量名
    m_VariableName;
    //当前状态
    util::judge_state state = util::INIT;
    while (m_NextWordPtr < m_Buff.length())
    {
        NextWord();
        //info("当前单词: {}",m_CurrentWord);
        switch (state)
        {
        case util::INIT:
            if(CheckWordType(m_CurrentWord) == util::DATA_TYPE)
                state = util::DEFINE_DATA_TYPE;
            else if(m_CurrentWord == "struct")
                state = util::STRUCT;
            else if(CheckWordType(m_CurrentWord) == util::VARIABLE)
            {
                m_VariableName = m_CurrentWord;
                state = util::VARIABLE_NAME;
            }
            break;
        
        case util::DEFINE_DATA_TYPE:
            if(m_CurrentWord == "*")
                state = util::DEFINE_PTR;
            else if(CheckWordType(m_CurrentWord) == util::VARIABLE)
            {
                m_VariableName = m_CurrentWord;
                state = util::DEFINE_VARIABLE_NAME;
            }
            else
                state = util::INIT;
            break;
        
        case util::DEFINE_PTR:
            if(m_CurrentWord == "*")
                state = util::DEFINE_PTR;
            else if(CheckWordType(m_CurrentWord) == util::VARIABLE)
            {
                m_VariableName = m_CurrentWord;
                state = util::DEFINE_VARIABLE_NAME;
            }
            else
                state = util::INIT;
            break;
        
        case util::DEFINE_VARIABLE_NAME:
            if(m_CurrentWord[0] == '[')
            {
                info("定义数组: {}", m_VariableName);
                state = util::INIT;
            }
            else if(m_CurrentWord[0] == '(')
            {
                info("定义函数: {}", m_VariableName);
                state = util::DEFINE_FUNCTION;
                leftsum = 1;
                function_blacklist[m_VariableName] = 0;
            }
            else if(m_CurrentWord[0] == ';' || m_CurrentWord[0] == '=' || m_CurrentWord[0] == ',')
            {
                info("定义变量: {}", m_VariableName);
                if(m_CurrentWord[0] == ',')
                    state = util::DEFINE_DATA_TYPE;
                else
                    state = util::INIT;
            }
            else
                state = util::INIT;
            break;
        
        case util::DEFINE_FUNCTION:
            if(m_CurrentWord[0] == '(')
                leftsum++;
            else if(m_CurrentWord[0] == ')')
                leftsum--;
            if(!leftsum)
                state = util::INIT;
            break;

        case util::VARIABLE_NAME:
            if(m_CurrentWord[0] == '(')
            {
                info("调用函数: {}", m_VariableName);
                if(function_blacklist[m_VariableName] == 1)
                    throw JudgerException(m_CurrentRow, m_VariableName + "() 不应使用的函数");
            }
            state = util::INIT;
            break;

        case util::STRUCT:
            if(CheckWordType(m_CurrentWord) == util::VARIABLE)
            {
                m_DataType[m_CurrentWord] = 1;
                state = util::STRUCT_DEFINE_NAME;
            }

        case util::STRUCT_DEFINE_NAME:
            if(m_CurrentWord == "{")
            {
                m_InDefineStruct = true;
                state = util::STRUCT_DEFINE_MEMBER;
            }
            else
            {
                m_VariableName = m_CurrentWord;
                state = util::DEFINE_VARIABLE_NAME;
            }

        case util::STRUCT_DEFINE_MEMBER:
            if(m_CurrentWord == "}")
                m_InDefineStruct = false;
            if(!m_InDefineStruct)
            {
                if(m_CurrentWord == ";")
                    state = util::INIT;
                else if(m_CurrentWord == "*")
                    state = util::DEFINE_PTR;
                else
                {
                    m_VariableName = m_CurrentWord;
                    state = util::DEFINE_VARIABLE_NAME;
                }
            }

        default:
            break;
        }
    }
} */

void FunctionJudger::WhenDefineFunction()
{
    function_blacklist[m_VariableName] = 0;
}

void FunctionJudger::WhenCallFunction()
{
    if(function_blacklist[m_VariableName] == 1)
        throw JudgerException(m_CurrentRow, m_VariableName + "() 不应使用的函数");
}

void FunctionJudger::judge()
{
    FSM();
    pass = true;
}




