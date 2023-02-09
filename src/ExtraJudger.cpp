#include "ExtraJudger.h"

using namespace spdlog;

ExtraJudger::ExtraJudger(string src, int indentSize, bool leftBigParantheses)
:m_IndentSize(indentSize), m_LeftBigPara(leftBigParantheses), m_CurrentIsSingle(false),
m_NextIsSingle(false), m_CurrentRow(0), m_CurrentLayer(0), m_SingleSum(0), m_Buff(src)
{}

ExtraJudger::~ExtraJudger(){}

void ExtraJudger::ProcessComment()
{
    vector<pair<size_t,size_t>> vet;
    for(size_t i=0; i<m_Buff.length()-1; i++)
    {
        if(m_Buff[i] == '"')
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
                for(; pos<m_Buff.length()-1; pos++)
                {
                    if(m_Buff[pos] == '*' && m_Buff[pos+1] == '/')
                    {
                        pos+=2;
                        break;
                    }
                }
            }
            vet.push_back({i,pos-1});
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

bool ExtraJudger::ProcessRow()
{
    m_CurrentRow++;
    m_CurrentIsSingle = m_NextIsSingle;
    m_NextIsSingle = false;
    info("Line {} Len:{} Layer:{} Single:{}", m_CurrentRow, m_RowStr.length(),m_CurrentLayer, m_CurrentIsSingle, m_NextIsSingle);
    info(m_RowStr);

    //空行的情况
    if(m_RowStr.length() == 0)
    {
        info("Line {}: 空行", m_CurrentRow);   
        return true;
    }

    //去除首尾空格
    size_t start = 0, end=m_RowStr.length()-1;
    int space = 0;
    for(; start<=end; start++)
    {
        if(m_RowStr[start] == ' ')
            space++;
        else if(m_RowStr[start] == '\t')         //制表符加到对应层数
            space = space + m_IndentSize - (space%m_IndentSize);
        else
            break;
    }
    for(; end>=start && (m_RowStr[end]==' ' || m_RowStr[end]=='\t'); end--);
    if(start>end)
    {
        info("Line {}: 空行", m_CurrentRow);
        return true;
    }

    //单字符的情况特判
    if(start == end)
    {
        if(m_RowStr[start] == '{')
        {
            if(!m_LeftBigPara)
                throw JudgerException(m_CurrentRow,"{ 不应置于行首");
            if(m_IndentSize*(m_CurrentLayer-1) != space)
                throw JudgerException(m_CurrentRow,"{ 缩进出错");
            m_NextIsSingle = false;
            m_SingleSum = 0;
        }
        else if(m_RowStr[start] == '}')
        {
            if(m_IndentSize*(m_CurrentLayer-1) != space)
                throw JudgerException(m_CurrentRow, "} 缩进出错");
            m_CurrentLayer--;
        }
        else if(m_RowStr[start] == '\\')
            info("Line {}: \\", m_CurrentRow);
        else 
            throw JudgerException(m_CurrentRow, "不允许的单行字符");
        return true;
    }

    //判断缩进长度是否合法
    info("Line {}: space: {} need:{}", m_CurrentRow, space, m_IndentSize*m_CurrentLayer);
    if(space != m_IndentSize*m_CurrentLayer)
        throw JudgerException(m_CurrentRow, "缩进出错");

    //忽略#
    if(m_RowStr[start] == '#')
    {
        info("Line {}: #开头", m_CurrentRow);
        return true;
    }

    //判断层数是否增加
    if(m_RowStr[end] != ';')
    {
        if(m_RowStr[end] == '{')
        {
            //大括号样式1但{在末尾
            if(m_LeftBigPara==1 && start != end)
                throw JudgerException(m_CurrentRow,"{ 不应置于行末");
            m_CurrentLayer++;
        }
        else if(m_RowStr[end] == '}')
            throw JudgerException(m_CurrentRow,"{ 不应置于行末");
        else if(m_RowStr[end] == ')' || m_RowStr[end] == ',')
        {
            m_NextIsSingle = true;
            m_SingleSum++;
            m_CurrentLayer++;
        }
        else if(m_RowStr[end] == 'e')        //else的情况
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

    //单行结束
    if(!m_NextIsSingle && m_CurrentIsSingle)
    {
        m_CurrentLayer -= m_SingleSum;
        m_SingleSum = 0;
    }
    return true;
}

void ExtraJudger::Judge()
{
    try
    {
        ProcessComment();
        size_t las = 0;
        string ret;
        for(size_t pi=0; pi<m_Buff.length(); pi++)
        {
            if(m_Buff[pi] == '\n' || pi == m_Buff.length()-1)
            {
                //获取当前行
                m_RowStr = m_Buff.substr(las,pi-las);
                las = pi+1;
                ProcessRow();
            }
        }
        info("Judge Finish");
        m_result = "success";
    }
    catch(const JudgerException& e)
    {
        m_result = e.what();
    }
}

const char* ExtraJudger::GetResult()
{
    return m_result.c_str();
}

const char* judge(const char* src, int indentSize, bool leftBigPara)
{
    ExtraJudger* judger = new ExtraJudger(src,indentSize,leftBigPara);
    judger->Judge();
    return judger->GetResult();
}

