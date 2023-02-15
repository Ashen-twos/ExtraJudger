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
                throw JudgerException(GetRealRow(m_CurrentRow),"{ 不应置于行首");
            if(m_IndentSize*(m_CurrentLayer-1) != space)
                throw JudgerException(GetRealRow(m_CurrentRow),"{ 缩进出错");
            m_NextIsSingle = false;
            m_SingleSum = 0;
        }
        else if(m_RowStr[start] == '}')
        {
            if(m_IndentSize*(m_CurrentLayer-1) != space)
                throw JudgerException(GetRealRow(m_CurrentRow), "} 缩进出错");
            m_CurrentLayer--;
        }
        else if(m_RowStr[start] == '\\')
            info("Line {}: \\", m_CurrentRow);
        else 
            throw JudgerException(GetRealRow(m_CurrentRow), "不允许的单行字符");
        return true;
    }

    //判断缩进长度是否合法
    info("Line {}: space: {} need:{}", m_CurrentRow, space, m_IndentSize*m_CurrentLayer);
    if(space != m_IndentSize*m_CurrentLayer)
        throw JudgerException(GetRealRow(m_CurrentRow), "缩进出错");

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
                throw JudgerException(GetRealRow(m_CurrentRow),"{ 不应置于行末");
            m_CurrentLayer++;
        }
        else if(m_RowStr[end] == '}')
            throw JudgerException(GetRealRow(m_CurrentRow),"{ 不应置于行末");
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
                //获取当前行
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
    //分离单词
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
    //定义了结构体，加入数据类型
    if(m_PreFuncWord == "struct")
    {
        info("定义结构体: {}", m_FuncWord);
        m_DataType[m_FuncWord] = 1;
        return;
    }
    //函数的情况
    if(m_Buff[m_FuncPtr] == '(')
    {
        if(m_DataType[m_PreFuncWord] == 1)
        {
            info("定义函数: {}",m_FuncWord);
            functions[m_FuncWord] = 0;
        }
        else
        {
            info("调用函数: {}",m_FuncWord);
            if(functions[m_FuncWord]==1)
            {
                error("不应调用的函数: {}",m_FuncWord);
                throw JudgerException(GetRealRow(m_CurrentRow), m_FuncWord + "() 不应使用的函数");
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
                vet.push_back(tmp.substr(las,i-las));
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
