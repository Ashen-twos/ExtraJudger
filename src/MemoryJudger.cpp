#include "MemoryJudger.h"

using namespace spdlog;

MemoryJudger::MemoryJudger(string& code, const char* white_list, bool check_ptr_free) : JudgerFSM(code),
m_CheckPtrFree(check_ptr_free)
{
    util::ParseString(white_list, m_EnableArray);
}

MemoryJudger::~MemoryJudger(){}

void MemoryJudger::WhenDefineArray()
{
    if(!m_EnableArray.count(m_VariableName))
        throw JudgerException(m_CurrentRow, m_VariableName + ": 不允许的数组");
}

void MemoryJudger::WhenCallFunction()
{
    if(m_CheckPtrFree)
    {
        if(m_EqualLeft != "" && m_VariableName == "malloc")
            m_MallocPtr[m_EqualLeft] = m_CurrentRow;
        else if(m_VariableName == "free")
        {
            NextWord();
            m_MallocPtr[m_CurrentWord] = 0;
        }
    }
}

void MemoryJudger::WhenStatementEnd()
{
    if(m_EqualLeft != "")
        m_EqualLeft = "";
}

void MemoryJudger::judge()
{
    FSM();
    if(m_CheckPtrFree)
    {
        for(auto iter=m_MallocPtr.begin(); iter != m_MallocPtr.end(); iter++)
            if(iter->second != 0)
                throw JudgerException(iter->second, iter->first + ": 未释放的指针");
    }
    pass = true;
}
