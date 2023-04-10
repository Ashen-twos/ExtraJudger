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
    int m_MaxStatement;
    bool m_LeftBigPara;           //大括号是否前置
    bool m_CurrentIsSingle;       //当前是否单行
    bool m_NextIsSingle;          //下一行是否单行
    bool m_CommaSpace;


    string m_RowStr;

    void CheckSpace();
    void CheckStatement();
    bool FormatProcessRow();    //处理单行
    
public:
    FormatJudger(string& code, int indent_size, bool left_big_para, bool comma_space, int max_statement);
    ~FormatJudger();
    void judge();
};
