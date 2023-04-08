#pragma once
#include <bits/stdc++.h>
#include "Judger.h"
#include "spdlog/spdlog.h"
#include "Exception/JudgerException.h"

using namespace std;

class FormatJudger : public Judger
{
private:
    int m_CurrentRow;             //当前行
    int m_IndentSize;             //缩进长度
    int m_CurrentLayer;           //当前层数
    int m_SingleSum;              //单行层数
    bool m_LeftBigPara;           //大括号是否前置
    bool m_CurrentIsSingle;       //当前是否单行
    bool m_NextIsSingle;          //下一行是否单行

    string m_RowStr;

    bool FormatProcessRow();    //处理单行
    
public:
    FormatJudger(string& code, int IndentSize, bool LeftBigPara);
    ~FormatJudger();
    void judge();
};
