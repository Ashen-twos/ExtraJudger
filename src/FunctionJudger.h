#pragma once
#include <bits/stdc++.h>
#include "JudgerFSM.h"
#include "spdlog/spdlog.h"
#include "Exception/JudgerException.h"

using namespace std;

class FunctionJudger : public JudgerFSM
{
private:
    util::char_type m_WordType;            //单词类型
    map<string,int> function_blacklist;    //函数黑名单

protected:
    virtual void WhenDefineFunction();
    virtual void WhenCallFunction();

public:
    FunctionJudger(string& code, const char* func);
    ~FunctionJudger();
    void judge();
};
