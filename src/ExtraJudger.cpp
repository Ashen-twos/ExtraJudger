#include "ExtraJudger.h"

using namespace spdlog;

ExtraJudger::ExtraJudger(const char* src)
:m_IndentSize(4), m_LeftBigPara(true), m_CurrentIsSingle(false),
m_NextIsSingle(false), m_CurrentRow(0), m_CurrentLayer(0), m_SingleSum(0), m_Buff(src),
m_FuncPtr(0)
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
        else if(m_Buff[i] == '"')
        {
            i++;
            while(i<m_Buff.length() && m_Buff[i] != '"')
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

bool ExtraJudger::FormatProcessRow()
{
    m_CurrentRow++;
    m_CurrentIsSingle = m_NextIsSingle;
    m_NextIsSingle = false;
    info("Line {} Len:{} Layer:{} Single:{}", m_CurrentRow, m_RowStr.length(),m_CurrentLayer, m_CurrentIsSingle, m_NextIsSingle);
    info(m_RowStr);

    //???????????????
    if(m_RowStr.length() == 0)
    {
        info("Line {}: ??????", m_CurrentRow);   
        return true;
    }

    //??????????????????
    size_t start = 0, end=m_RowStr.length()-1;
    int space = 0;
    for(; start<=end; start++)
    {
        if(m_RowStr[start] == ' ')
            space++;
        else if(m_RowStr[start] == '\t')         //???????????????????????????
            space = space + m_IndentSize - (space%m_IndentSize);
        else
            break;
    }
    for(; end>=start && (m_RowStr[end]==' ' || m_RowStr[end]=='\t'); end--);
    if(start>end)
    {
        info("Line {}: ??????", m_CurrentRow);
        return true;
    }

    //????????????????????????
    if(start == end)
    {
        if(m_RowStr[start] == '{')
        {
            if(!m_LeftBigPara)
                throw JudgerException(GetRealRow(m_CurrentRow),"{ ??????????????????");
            if(m_IndentSize*(m_CurrentLayer-1) != space)
                throw JudgerException(GetRealRow(m_CurrentRow),"{ ????????????");
            m_NextIsSingle = false;
            m_SingleSum = 0;
        }
        else if(m_RowStr[start] == '}')
        {
            if(m_IndentSize*(m_CurrentLayer-1) != space)
                throw JudgerException(GetRealRow(m_CurrentRow), "} ????????????");
            m_CurrentLayer--;
        }
        else if(m_RowStr[start] == '\\')
            info("Line {}: \\", m_CurrentRow);
        else 
            throw JudgerException(GetRealRow(m_CurrentRow), "????????????????????????");
        return true;
    }

    //??????????????????????????????
    info("Line {}: space: {} need:{}", m_CurrentRow, space, m_IndentSize*m_CurrentLayer);
    if(space != m_IndentSize*m_CurrentLayer)
        throw JudgerException(GetRealRow(m_CurrentRow), "????????????");

    //??????#
    if(m_RowStr[start] == '#')
    {
        info("Line {}: #??????", m_CurrentRow);
        return true;
    }

    //????????????????????????
    if(m_RowStr[end] != ';')
    {
        if(m_RowStr[end] == '{')
        {
            //???????????????1???{?????????
            if(m_LeftBigPara==1 && start != end)
                throw JudgerException(GetRealRow(m_CurrentRow),"{ ??????????????????");
            m_CurrentLayer++;
        }
        else if(m_RowStr[end] == '}')
            throw JudgerException(GetRealRow(m_CurrentRow),"{ ??????????????????");
        else if(m_RowStr[end] == ')' || m_RowStr[end] == ',')
        {
            m_NextIsSingle = true;
            m_SingleSum++;
            m_CurrentLayer++;
        }
        else if(m_RowStr[end] == 'e')        //else?????????
        {
            if(end-start==3 && m_RowStr[end-1]=='s' && m_RowStr[end-2]=='l' && m_RowStr[end-3]=='e')
            {
                m_CurrentLayer++;
                m_NextIsSingle = true;
                m_SingleSum++;
                info("Line {}: else", m_CurrentRow);
            }
        }
    }

    //????????????
    if(!m_NextIsSingle && m_CurrentIsSingle)
    {
        m_CurrentLayer -= m_SingleSum;
        m_SingleSum = 0;
    }
    return true;
}

void ExtraJudger::FormatJudge(int indentSize, bool leftBigPara)
{
    try
    {
        m_IndentSize = indentSize;
        m_LeftBigPara = leftBigPara;
        m_CurrentRow = 0;
        size_t las = 0;
        string ret;
        for(size_t pi=0; pi<m_Buff.length(); pi++)
        {
            if(m_Buff[pi] == '\n' || pi == m_Buff.length()-1)
            {
                //???????????????
                m_RowStr = m_Buff.substr(las,pi-las);
                las = pi+1;
                FormatProcessRow();
            }
        }
        info("Judge Finish");
        m_Result = "success";
    }
    catch(const JudgerException& e)
    {
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

void ExtraJudger::FuncProcessWord()
{
    m_PreFuncWord = m_FuncWord;
    m_FuncWord.clear();
    //????????????
    while(m_FuncPtr < m_Buff.length() && !((m_Buff[m_FuncPtr]>='a' && m_Buff[m_FuncPtr]<='z') || m_Buff[m_FuncPtr]=='_')||
            (m_Buff[m_FuncPtr]>='A' && m_Buff[m_FuncPtr]<='Z') || (m_Buff[m_FuncPtr]>='0' && m_Buff[m_FuncPtr]<='9'))
    {
        if(m_Buff[m_FuncPtr]=='\n')
            m_CurrentRow++;
        m_FuncPtr++;
    }
    while(m_FuncPtr < m_Buff.length() && ((m_Buff[m_FuncPtr]>='a' && m_Buff[m_FuncPtr]<='z') || m_Buff[m_FuncPtr]=='_' ||
            (m_Buff[m_FuncPtr]>='A' && m_Buff[m_FuncPtr]<='Z') || (m_Buff[m_FuncPtr]>='0' && m_Buff[m_FuncPtr]<='9')))
    {
        m_FuncWord.push_back(m_Buff[m_FuncPtr]);
        m_FuncPtr++;
    }
    while(m_FuncPtr < m_Buff.length() && (m_Buff[m_FuncPtr]==' ' || m_Buff[m_FuncPtr]=='\n' || m_Buff[m_FuncPtr]=='\t'))
    {
        if(m_Buff[m_FuncPtr]=='\n')
            m_CurrentRow++;
        m_FuncPtr++;
    }
    //???????????????????????????????????????
    if(m_PreFuncWord == "struct")
    {
        info("???????????????: {}", m_FuncWord);
        m_DataType[m_FuncWord] = 1;
        return;
    }
    //???????????????
    if(m_Buff[m_FuncPtr] == '(')
    {
        if(m_DataType[m_PreFuncWord] == 1)
        {
            info("????????????: {}",m_FuncWord);
            functions[m_FuncWord] = 0;
        }
        else
        {
            info("????????????: {}",m_FuncWord);
            if(functions[m_FuncWord]==1)
            {
                error("?????????????????????: {}",m_FuncWord);
                throw JudgerException(GetRealRow(m_CurrentRow), m_FuncWord + "() ?????????????????????");
            }
        } 
    }
}

void ExtraJudger::FuncJudge(const char* func)
{
    try
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
                    info("????????????: {}",vet.back());
                }
                las = i+1;
            }
        }
        for(int i=0; i<vet.size(); i++)
            functions[vet[i]] = 1;
        while (m_FuncPtr < m_Buff.length())
            FuncProcessWord();
        m_Result = "success";
    }
    catch(const JudgerException& e)
    {
        m_Result = e.what();
    }
}
