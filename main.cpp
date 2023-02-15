#include <bits/stdc++.h>
#include "src/ExtraJudger.h"
#include "src/Exception/JudgerException.h"

using namespace std;

/* void ProcessComment(string& m_RowStr)
{
    vector<pair<int,int>> vet;
    for(int i=0; i<m_RowStr.length()-1; i++)
    {
        if(m_RowStr[i] == '"')
        {
            i++;
            while(i<m_RowStr.length() && m_RowStr[i] != '"')
                i++;
        }
        else if(m_RowStr[i] == '/' && (m_RowStr[i+1]=='/' || m_RowStr[i+1] == '*'))
        {
            int pos = i+2;
            if(m_RowStr[i+1] == '/')
            {
                for(; pos<m_RowStr.length(); pos++)
                {
                    if(m_RowStr[pos] == '\n')
                        break;
                }
            }
            else
            {
                for(; pos<m_RowStr.length()-1; pos++)
                {
                    if(m_RowStr[pos] == '*' && m_RowStr[pos+1] == '/')
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
        vet.push_back({m_RowStr.length(),m_RowStr.length()});
        int now = vet[0].first;
        for(int i=1; i<vet.size(); i++)
        {
            printf("%d %d\n",vet[i].first, vet[i].second);
            for(int j=vet[i-1].second+1; j<=vet[i].first-1; j++)
                m_RowStr[now++] = m_RowStr[j];
        }
        m_RowStr.resize(now);
    }
}


int check(string src, int m_IndentSize, bool m_LeftBigPara)
{
    int m_CurrentRow = 0;
    int m_CurrentLayer = 0;
    bool m_IsSingle = false;
    int las = 0;
    string m_RowStr;
    for(int pi=0; pi<src.length(); pi++)
    {
        if(src[pi] == '\n' || pi == src.length()-1)
        {
            //获取当前行
            m_RowStr = src.substr(las,pi-las);
            las = pi+1;
            m_CurrentRow++;
            cout << "Current: " << m_CurrentRow << " Layer: " << m_CurrentLayer << " Single:" << m_IsSingle << '\n';
            cout << m_RowStr << '\n';

            //空行的情况
            if(m_RowStr.length() == 0)
            {
                cout << "空行\n";
                continue;   
            }

            //去除首尾空格
            int start = 0, end=m_RowStr.length()-1;
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
            for(; end>=0 && m_RowStr[end] == ' '; end--);
            if(start>end)
            {
                cout << "全是空格\n";
                continue;
            }

            //忽略#
            if(m_RowStr[start] == '#')
            {
                cout << "#开头\n";
                continue;
            }

            //单字符的情况特判
            if(start == end)
            {
                if(m_RowStr[start] == '{')
                {
                    if(!m_LeftBigPara)
                    {
                        cout << "{位置出错\n";
                        return m_CurrentRow;
                    }
                    if(m_IndentSize*(m_CurrentLayer-1) != space)
                    {
                        cout << "{缩进出错\n";
                        return m_CurrentRow;
                    }
                    m_IsSingle = false;
                }
                else if(m_RowStr[start] == '}')
                {
                    if(m_IndentSize*(m_CurrentLayer-1) != space)
                    {
                        cout << "}缩进出错\n";
                        return m_CurrentRow;
                    }
                    m_CurrentLayer--;
                }
                else if(m_RowStr[start] == '\\')
                    continue;
                else 
                {
                    cout << "未知字符/缩进出错\n";
                    return m_CurrentRow;
                }
                continue;
            }

            //判断缩进长度是否合法
            printf("space:%d need:%d\n",space,m_IndentSize*m_CurrentLayer);
            if(space != m_IndentSize*m_CurrentLayer)
            {
                cout << "缩进长度不合法\n";
                return m_CurrentRow;
            }

            //单行的情况
            if(m_IsSingle)
            {
                m_CurrentLayer--;
                m_IsSingle = false;
            }

            //判断层数是否增加
            if(m_RowStr[end] != ';')
            {
                if(m_RowStr[end] == '{')
                {
                    //大括号样式1但{在末尾
                    if(m_LeftBigPara==1 && start != end)
                    {
                        cout << "{位置出错\n";
                        return m_CurrentRow;
                    }
                    m_CurrentLayer++;
                }
                else if(m_RowStr[end] == '}')
                {
                    cout << "}位置出错\n";
                    return m_CurrentRow;
                }
                else if(m_RowStr[end] == ')' || m_RowStr[end] == ',')
                {
                    cout << "可能的单行\n";
                    m_IsSingle = true;
                    m_CurrentLayer++;
                }
                else if(m_RowStr[end] == 'e')        //else的情况
                {
                    if(end-start==3 && m_RowStr[end-1]=='s' && m_RowStr[end-2]=='l' && m_RowStr[end-3]=='e')
                    {
                        m_CurrentLayer++;
                        m_IsSingle++;
                        cout << "else\n";
                    }
                }
            }

        }
    }
    return -1;
} */


int main(){
    //check("data/1.in", 4);
    ifstream fin("data/2.in");
    stringstream buf;
    buf << fin.rdbuf();
    string Str = buf.str();
    ExtraJudger* judger = new ExtraJudger(Str.c_str());
    judger->FormatJudge(4,true);
    cout << judger->GetResult();
    //cout << format_judge(Str.c_str(),4,true) << '\n';
    string strs = "sort lower_bound";
    judger->FuncJudge(strs.c_str());
    cout << judger->GetResult(); 
    //cout << func_judge(Str.c_str(),strs.c_str()) << '\n';
    delete judger;
    return 0;
}