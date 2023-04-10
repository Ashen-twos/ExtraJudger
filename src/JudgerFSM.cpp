#include "JudgerFSM.h"

using namespace spdlog;

JudgerFSM::JudgerFSM(string& code):Judger(code), 
m_NextWordPtr(0), m_CurrentRow(1), m_InDefineStruct(false), m_CurrentScope(""), m_SumOfBigPara(0), m_SumOfSmallPara(0), m_InFor(false)
{
    m_DataType["char"] = 1; m_DataType["int"] = 1; m_DataType["short"] = 1;
    m_DataType["long"] = 1; m_DataType["float"] = 1; m_DataType["double"] = 1;
    m_DataType["void"] = 1; m_DataType["enum"] = 1;
};

JudgerFSM::~JudgerFSM(){}

void JudgerFSM::NextWord()               //下一个单词
    {
        m_CurrentWord.clear();
        while(m_NextWordPtr < m_Buff.length() && util::CheckCharType(m_Buff[m_NextWordPtr]) == util::SPACE)
        {
            if(m_Buff[m_NextWordPtr]=='\n')
                m_CurrentRow++;
            m_NextWordPtr++;
        }
        m_CharType = util::CheckCharType(m_Buff[m_NextWordPtr]);
        m_CurrentWord.push_back(m_Buff[m_NextWordPtr]);
        m_NextWordPtr++;
        if(m_CharType == util::SYMBOL)
        {
            if(m_Buff[m_NextWordPtr-1] == '\'' || m_Buff[m_NextWordPtr-1] == '\"')
            {
                char target = m_Buff[m_NextWordPtr-1];
                while(m_NextWordPtr < m_Buff.length())
                {
                    if(target == m_Buff[m_NextWordPtr])
                    {
                        m_CurrentWord.push_back(m_Buff[m_NextWordPtr]);
                        m_NextWordPtr++;
                        break;
                    }
                    m_CurrentWord.push_back(m_Buff[m_NextWordPtr]);
                    m_NextWordPtr++;
                    if(m_Buff[m_NextWordPtr] == '\\')
                    {
                        m_CurrentWord.push_back(m_Buff[m_NextWordPtr]);
                        m_NextWordPtr++;
                    }
                }
            }
            return; 
        }
        while(m_NextWordPtr < m_Buff.length() && m_CharType == util::CheckCharType(m_Buff[m_NextWordPtr]))
        {
            m_CurrentWord.push_back(m_Buff[m_NextWordPtr]);
            m_NextWordPtr++;
        }
    }

util::word_type JudgerFSM::CheckWordType(string& word)
{
    if(word == "if" || word == "while" || word == "switch" || word == "case" || 
        word == "break" || word == "continue" || word == "default" || word == "do" || 
        word == "else" || word == "extern" || word == "for" || word == "goto" || 
        word == "return" || word == "union" || word == "typedef")
        return util::RESERVED_WORD;
    else if(m_DataType.count(word))
        return util::DATA_TYPE;
    else if(util::CheckCharType(word[0]) == util::SYMBOL)
        return util::SYMBOLS;
    else
        return util::VARIABLE;
}

void JudgerFSM::FSM()
{
    //变量名
    m_VariableName;
    //当前状态
    util::judge_state state = util::INIT;
    while (m_NextWordPtr < m_Buff.length())
    {
        NextWord();
        //info("当前单词: {}",m_CurrentWord);
        //info("当前作用域: {}",m_CurrentScope);
        switch (state)
        {
        case util::INIT:
            if(m_CurrentWord == "struct")
                state = util::STRUCT;
            else if(CheckWordType(m_CurrentWord) == util::DATA_TYPE)
                state = util::DEFINE_DATA_TYPE;
            else if(CheckWordType(m_CurrentWord) == util::RESERVED_WORD)
            {
                if(m_CurrentWord == "for")
                {
                    m_InFor = true;
                    m_SumOfSmallPara = 0;
                }
            }
            else if(CheckWordType(m_CurrentWord) == util::VARIABLE)
            {
                m_VariableName = m_CurrentWord;
                state = util::VARIABLE_NAME;
            }
            else if (CheckWordType(m_CurrentWord) == util::SYMBOLS)
            {
                if(m_CurrentWord == "}")
                {
                    m_SumOfBigPara--;
                    if(!m_SumOfBigPara)
                        m_CurrentScope = "";
                }
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
                WhenDefineArray();
                state = util::INIT;
            }
            else if(m_CurrentWord[0] == '(')
            {
                info("定义函数: {}", m_VariableName);
                m_CurrentScope = m_VariableName;
                state = util::FUNCTION_DEFINE_PARA;
                WhenDefineFunction();
            }
            else if(m_CurrentWord[0] == ';' || m_CurrentWord[0] == '=' || m_CurrentWord[0] == ',')
            {
                info("定义变量: {}", m_VariableName);
                WhenDefineVariable();
                if(m_CurrentWord[0] == ',')
                    state = util::DEFINE_DATA_TYPE;
                else
                    state = util::INIT;
            }
            else
                state = util::INIT;
            break;
        
        case util::FUNCTION_DEFINE_PARA:
            if(m_CurrentWord == "struct")
                break;
            if(CheckWordType(m_CurrentWord) == util::DATA_TYPE)
                state = util::FUNCTION_DEFINE_PARA_TYPE;
            else if(m_CurrentWord == ")")
                state = util::FUNCTION_DEFINE;
            break;
        
        case util::FUNCTION_DEFINE_PARA_TYPE:
            if(m_CurrentWord == "*")
                break;
            if(CheckWordType(m_CurrentWord) == util::VARIABLE)
            {
                state = util::FUNCTION_DEFINE_PARA_NAME;
                m_VariableName = m_CurrentWord;
            }
            break;
        
        case util::FUNCTION_DEFINE_PARA_NAME:
            if(m_CurrentWord == "[")
            {
                info("定义函数参数(数组): {}", m_VariableName);
                WhenDefineVariable();
            }
            else if(m_CurrentWord == ",")
            {
                info("定义函数参数(变量): {}", m_VariableName);
                state = util::FUNCTION_DEFINE_PARA;
                WhenDefineVariable();
            }
            else if(m_CurrentWord == ")")
                state = util::FUNCTION_DEFINE;
            else
                throw JudgerException(m_CurrentRow, "未知情况");
            break;

        case util::FUNCTION_DEFINE:
            if(m_CurrentWord == "{")
            {
                m_SumOfBigPara++;
                state = util::INIT;
            }
            else
            {
                m_CurrentScope = "";
                state = util::INIT;
            }
            break;

        case util::VARIABLE_NAME:
            if(m_CurrentWord == "(")
            {
                info("调用函数: {}", m_VariableName);
                WhenCallFunction();
                state = util::INIT;
            }
            else if(m_CurrentWord == ".")
                state = util::VARIABLE_NAME;
            else
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
        if(m_CurrentWord == ";")
            WhenStatementEnd();
        if(m_InFor)
        {
            if(m_CurrentWord == "(")
                m_SumOfSmallPara++;
            else if(m_CurrentWord == ")")
            {
                m_SumOfSmallPara--;
                if(!m_SumOfSmallPara)
                    m_InFor = false;
            }
        }
    }
}

