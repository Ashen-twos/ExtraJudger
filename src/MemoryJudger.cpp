#include "MemoryJudger.h"

using namespace spdlog;

MemoryJudger::MemoryJudger(string& code, const char* list) : JudgerFSM(code)
{
    //处理参数
    vector<string> vet;
    string tmp(list);
    int las = 0;
    for(int i=0; i<=tmp.length(); i++)
    {
        if(i==tmp.length() || tmp[i] == ' ')
        {
            if(i>las)
            {
                vet.push_back(tmp.substr(las,i-las));
                info("允许数组: {}",vet.back());
            }
            las = i+1;
        }
    }
    for(int i=0; i<vet.size(); i++)
        m_EnableArray[vet[i]] = 1;
}

MemoryJudger::~MemoryJudger(){}

void MemoryJudger::WhenDefineArray()
{
    if(!m_EnableArray.count(m_VariableName))
        throw JudgerException(m_CurrentRow, m_VariableName + ": 不允许的数组");
}

void MemoryJudger::judge()
{
    FSM();
    pass = true;
}
