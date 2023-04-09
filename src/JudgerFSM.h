#pragma once
#include <bits/stdc++.h>
#include "Judger.h"
#include "util.h"
#include "spdlog/spdlog.h"

using namespace std;

class JudgerFSM : public Judger
{
protected:
    string m_CurrentWord;
    string m_VariableName;
    int m_NextWordPtr;
    int m_CurrentRow;
    bool m_InDefineStruct;
    bool m_InMain;
    util::char_type m_CharType;   //单词类型
    map<string,int> m_DataType;   //数据类型

    void NextWord();              //下一个单词
    util::word_type CheckWordType(string& word);
    void FSM();

    virtual void WhenDefineFunction(){};
    virtual void WhenDefineVariable(){};
    virtual void WhenDefineArray(){};
    virtual void WhenCallFunction(){};
public:
    JudgerFSM()=delete;
    JudgerFSM(string& code);
    ~JudgerFSM();
    virtual void judge()=0;
};
