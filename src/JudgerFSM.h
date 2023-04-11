#pragma once
#include <bits/stdc++.h>
#include "Judger.h"
#include "util.h"
#include "spdlog/spdlog.h"
#include "Exception/JudgerException.h"

using namespace std;

class JudgerFSM : public Judger
{
protected:
    string m_CurrentWord;
    string m_VariableName;
    string m_CurrentScope;
    string m_EqualLeft;
    int m_NextWordPtr;
    int m_CurrentRow;
    int m_SumOfBigPara;
    int m_SumOfSmallPara;
    bool m_InDefineStruct;
    bool m_InFor;
    util::char_type m_CharType;    //单词类型
    util::judge_state m_PreState;  //上一个状态
    map<string,int> m_DataType;    //数据类型

    void NextWord();              //下一个单词
    util::word_type CheckWordType(string& word);
    void FSM();

    virtual void WhenDefineFunction(){};
    virtual void WhenDefineVariable(){};
    virtual void WhenDefineArray(){};
    virtual void WhenCallFunction(){};
    virtual void WhenStatementEnd(){};
public:
    JudgerFSM()=delete;
    JudgerFSM(string& code);
    ~JudgerFSM();
    virtual void judge()=0;
};
