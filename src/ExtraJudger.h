#pragma once
#include <bits/stdc++.h>
#include "Exception/JudgerException.h"
#include "spdlog/spdlog.h"

using namespace std;

class ExtraJudger
{
private:
    int m_IndentSize;             //缩进长度
    bool m_LeftBigPara;           //大括号是否前置
    bool m_CurrentIsSingle;       //当前是否单行
    bool m_NextIsSingle;          //下一行是否单行

    int m_CurrentRow;           //当前行
    int m_CurrentLayer;         //当前层数
    int m_SingleSum;            //迭代次数              

    string m_RowStr;            //当前行
    string m_Buff;              //缓冲区

    void ProcessComment();      //处理注释
    bool ProcessRow();          //处理单行

public:
    ExtraJudger(string src, int indentSize, bool leftBigPara);
    ~ExtraJudger();
    string Judge();
};

const char* judge(const char* src, int indentSize, bool leftBigPara);

