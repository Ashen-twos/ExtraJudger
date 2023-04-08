#include "ExtraJudger.h"

using namespace spdlog;

ExtraJudger::ExtraJudger(const char* src)
:m_CurrentRow(0), m_Buff(src)
{
    m_DataType["char"] = 1; m_DataType["int"] = 1; m_DataType["short"] = 1;
    m_DataType["long"] = 1; m_DataType["float"] = 1; m_DataType["double"] = 1;
    m_DataType["void"] = 1;
    ProcessComment();
}

ExtraJudger::~ExtraJudger(){}

void ExtraJudger::ProcessComment()
{
    m_CurrentRow = 1;
    vector<pair<size_t,size_t>> vet;
    for(size_t i=0; i<m_Buff.length()-1; i++)
    {
        if(m_Buff[i] == '\n')
            m_CurrentRow++;
        else if(m_Buff[i] == '\"')
        {
            i++;
            while(i<m_Buff.length() && m_Buff[i] != '\"')
                i++;
        }
        else if(m_Buff[i] == '/' && (m_Buff[i+1]=='/' || m_Buff[i+1] == '*'))
        {
            size_t pos = i+2;
            if(m_Buff[i+1] == '/')
            {
                for(; pos<m_Buff.length(); pos++)
                {
                    if(m_Buff[pos] == '\n')
                        break;
                }
            }
            else
            {
                int line = 0;
                for(; pos<m_Buff.length()-1; pos++)
                {
                    if(m_Buff[pos] == '\n')
                        line++;
                    else if(m_Buff[pos] == '*' && m_Buff[pos+1] == '/')
                    {
                        pos+=2;
                        break;
                    }
                }
                m_MultiSum.push_back({m_CurrentRow, line});
            }
            vet.push_back({i,pos-1});
            i=pos-1;
        }
    }
    if(vet.size())
    {
        vet.push_back({m_Buff.length(),m_Buff.length()});
        size_t now = vet[0].first;
        for(size_t i=1; i<vet.size(); i++)
            for(size_t j=vet[i-1].second+1; j<=vet[i].first-1; j++)
                m_Buff[now++] = m_Buff[j];
        m_Buff.resize(now);
    }
}

void ExtraJudger::FormatJudge(int indentSize, bool leftBigPara)
{
    try
    {
        shared_ptr<FormatJudger> judger = make_shared<FormatJudger>(m_Buff, indentSize, leftBigPara);
        judger->judge();
        if(judger->pass)
            m_Result = "success";
    }
    catch(JudgerException& e)
    {
        e.UpdateLine(GetRealRow(e.GetLine()));
        m_Result = e.what();
    }
}

const char* ExtraJudger::GetResult()
{
    return m_Result.c_str();
}

int ExtraJudger::GetRealRow(int row)
{
    int add = 0;
    for(int i=0; i<m_MultiSum.size(); i++)
    {
        if(row < m_MultiSum[i].first)
            break;
        add += m_MultiSum[i].second;
    }
    return row + add;
}

void ExtraJudger::FuncJudge(const char* func)
{
    try
    {
        shared_ptr<FunctionJudger> judger = make_shared<FunctionJudger>(m_Buff, func);
        judger->judge();
        if(judger->pass)
            m_Result = "success";
    }
    catch(JudgerException& e)
    {
        e.UpdateLine(GetRealRow(e.GetLine()));
        m_Result = e.what();
    }
}

void ExtraJudger::MemoryJudge(const char* list)
{
    try
    {
        shared_ptr<MemoryJudger> judger = make_shared<MemoryJudger>(m_Buff, list);
        judger->judge();
        if(judger->pass)
            m_Result = "success";
    }
    catch(JudgerException& e)
    {
        e.UpdateLine(GetRealRow(e.GetLine()));
        m_Result = e.what();
    }
}
