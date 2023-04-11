#include "ExtraJudger.h"

using namespace spdlog;

ExtraJudger::ExtraJudger(const char* src)
:m_CurrentRow(0), m_Buff(src)
{
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

void ExtraJudger::FormatJudge(int indent_size, bool left_big_para, bool comma_space, int max_statement)
{
    try
    {
        shared_ptr<FormatJudger> judger = make_shared<FormatJudger>(m_Buff, indent_size, left_big_para, comma_space, max_statement);
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

void ExtraJudger::FuncJudge(const char* blacklist, const char* whitelist, int max_statement, bool disableIO)
{
    try
    {
        shared_ptr<FunctionJudger> judger = make_shared<FunctionJudger>(m_Buff, blacklist, whitelist, max_statement, disableIO);
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

void ExtraJudger::MemoryJudge(const char* white_list, bool check_ptr_free)
{
    try
    {
        shared_ptr<MemoryJudger> judger = make_shared<MemoryJudger>(m_Buff, white_list, check_ptr_free);
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

void ExtraJudger::StyleJudge(const char* global_prefix, const char* white_list,
    int func_naming, int global_naming, int local_naming, bool single_name)
{
    try
    {
        shared_ptr<StyleJudger> judger = make_shared<StyleJudger>(m_Buff, global_prefix, white_list,
            func_naming, global_naming, local_naming, single_name);
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

