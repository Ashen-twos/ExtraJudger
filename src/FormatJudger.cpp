#include "FormatJudger.h"

using namespace spdlog;

FormatJudger::FormatJudger(string& code, int indent_size, bool left_big_para, bool comma_space, int max_statement):
Judger(code), m_IndentSize(indent_size), m_LeftBigPara(left_big_para), m_CommaSpace(comma_space), m_MaxStatement(max_statement),
m_CurrentRow(0), m_CurrentLayer(0), m_SingleSum(0), m_CurrentIsSingle(false), m_NextIsSingle(false),
m_IndentPass(true),m_ParaPass(true),m_StatementPass(true),m_CommaPass(true)
{
    pass = 4;
    if(!comma_space)
        pass--;
}

FormatJudger::~FormatJudger(){}

void FormatJudger::CheckSpace()
{
    for(int i=0; i<m_RowStr.length()-1; i++)
    {
        if((m_RowStr[i] == ',' || m_RowStr[i] == ';') && i!= m_RowStr.length()-2)
        {
            if(m_RowStr[i+1] != ' ' && m_RowStr[i+1] != '\n' && m_RowStr[i+1] != '\t')
            {
                m_CommaPass = false;
                SetMessage(m_CurrentRow,",或;后缺少空格");
                return;
            }
        }
    }
}

void FormatJudger::CheckStatement()
{
    int sum = 0;
    for(int i=0; i<m_RowStr.length(); i++)
    {
        if(m_RowStr[i] == ';')
        {
            sum++;
            if(sum > m_MaxStatement)
            {
                m_StatementPass = false;
                SetMessage(m_CurrentRow,"同一行内出现过多语句");
                return;
            }
        }
    }
}

bool FormatJudger::FormatProcessRow()
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

    if(m_CommaSpace)
        CheckSpace();
    if(m_MaxStatement)
        CheckStatement();

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
            {
                m_ParaPass = false;
                SetMessage(m_CurrentRow,"{ 不应置于行首");
            }
            if(m_IndentSize*(m_CurrentLayer-1) != space)
            {
                m_ParaPass = false;
                SetMessage(m_CurrentRow,"{ 缩进出错");
            }
            m_NextIsSingle = false;
            m_SingleSum = 0;
        }
        else if(m_RowStr[start] == '}')
        {
            if(m_IndentSize*(m_CurrentLayer-1) != space)
            {
                m_ParaPass = false;
                SetMessage(m_CurrentRow, "} 缩进出错");
            }
            m_CurrentLayer--;
        }
        return true;
    }

    //判断缩进长度是否合法
    info("Line {}: space: {} need:{}", m_CurrentRow, space, m_IndentSize*m_CurrentLayer);
    if(space != m_IndentSize*m_CurrentLayer)
    {
        m_IndentPass = false;
        SetMessage(m_CurrentRow, "缩进出错");
    }

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
            if(m_LeftBigPara && start != end)
            {
                m_ParaPass = false;
                SetMessage(m_CurrentRow,"{ 不应置于行末");
            }
            m_CurrentLayer++;
        }
        else if(m_RowStr[end] == '}')
        {
            m_ParaPass = false;
            SetMessage(m_CurrentRow,"{ 不应置于行末");
        }
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

void FormatJudger::judge()
{
    try
    {
        size_t las = 0;
        string ret;
        for(size_t pi=0; pi<m_Buff.length(); pi++)
        {
            if(m_Buff[pi] == '\n' || pi == m_Buff.length()-1)
            {
                //获取当前行
                m_RowStr = m_Buff.substr(las,pi-las);
                las = pi+1;
                FormatProcessRow();
            }
        }
        if(!m_IndentPass)
            pass--;
        if(!m_ParaPass)
            pass--;
        if(!m_StatementPass)
            pass--;
        if(!m_CommaPass)
            pass--;
        if(m_IndentPass && m_ParaPass && m_StatementPass && m_CommaPass)
            result = "success";
        
    }
    catch(const JudgerException& e)
    {
        throw e;
    }
}