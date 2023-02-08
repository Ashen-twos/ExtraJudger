#include <bits/stdc++.h>

using namespace std;
void ProcessComment(string& str)
{
    vector<pair<int,int>> vet;
    for(int i=0; i<str.length()-1; i++)
    {
        if(str[i] == '"')
        {
            i++;
            while(i<str.length() && str[i] != '"')
                i++;
        }
        else if(str[i] == '/' && (str[i+1]=='/' || str[i+1] == '*'))
        {
            int pos = i+2;
            if(str[i+1] == '/')
            {
                for(; pos<str.length(); pos++)
                {
                    if(str[pos] == '\n')
                        break;
                }
            }
            else
            {
                for(; pos<str.length()-1; pos++)
                {
                    if(str[pos] == '*' && str[pos+1] == '/')
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
        vet.push_back({str.length(),str.length()});
        int now = vet[0].first;
        for(int i=1; i<vet.size(); i++)
        {
            printf("%d %d\n",vet[i].first, vet[i].second);
            for(int j=vet[i-1].second+1; j<=vet[i].first-1; j++)
                str[now++] = str[j];
        }
        str.resize(now);
    }
}


int check(string src, int indentSize, bool leftBigParantheses, bool elseUp)
{
    int currentRow = 0;
    int currentLayer = 0;
    bool isComment = false;
    int las = 0;
    string str;
    for(int pi=0; pi<src.length(); pi++)
    {
        if(src[pi] == '\n' || pi == src.length()-1)
        {
            //获取当前行
            str = src.substr(las,pi-las);
            las = pi+1;
            currentRow++;
            cout << "Current: " << currentRow << " Size: " << str.length() << '\n';
            cout << str << '\n';

            //空行的情况
            if(str.length() == 0)
            {
                cout << "空行\n";
                continue;   
            }

            //去除首尾空格
            int start = 0, end=str.length()-1;
            int space = 0;
            for(; start<=end && str[start] == ' '; start++,space++);
            for(; end>=0 &&str[end] == ' '; end--);
            if(start>end)
            {
                cout << "全是空格\n";
                continue;
            }

            //忽略#
            if(str[start] == '#')
            {
                cout << "#开头\n";
                continue;
            }

            //单字符的情况特判
            if(start == end)
            {
                if(str[start] == '{')
                {
                    if(!leftBigParantheses)
                    {
                        cout << "{位置出错\n";
                        return currentRow;
                    }
                    if(indentSize*(currentLayer-1) != space)
                    {
                        cout << "缩进出错\n";
                        return currentRow;
                    }
                }
                else if(str[start] == '}')
                {
                    if(indentSize*(currentLayer-1) != space)
                    {
                        cout << "缩进出错\n";
                        return currentRow;
                    }
                    currentLayer--;
                }
                else if(str[start] == '\\')
                    continue;
                else 
                {
                    cout << "未知字符/缩进出错\n";
                    return currentRow;
                }
                continue;
            }

            //判断缩进长度是否合法
            printf("space:%d need:%d\n",space,indentSize*currentLayer);
            if(space != indentSize*currentLayer)
            {
                cout << "缩进长度不合法\n";
                return currentRow;
            }

            //判断层数是否增加
            if(str[end] != ';')
            {
                //大括号样式1但{在末尾
                if(str[end] == '{')
                {
                    if(leftBigParantheses==1 && start != end)
                    {
                        cout << "{位置出错\n";
                        return currentRow;
                    }
                    currentLayer++;
                }
                else if(str[end] == '}')
                {
                    currentLayer--;
                }
                else if(str[end] == ')')
                {
                    currentLayer++;
                }
            }


        }
    }
}


int main(){
    //check("data/1.in", 4);
    ifstream fin("data/2.in");
    stringstream buf;
    buf << fin.rdbuf();
    string str = buf.str();
    ProcessComment(str);
    //check(str,4);
    return 0;
}