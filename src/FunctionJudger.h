#pragma once
#include <bits/stdc++.h>
#include "JudgerFSM.h"
#include "spdlog/spdlog.h"

using namespace std;

class FunctionJudger : public JudgerFSM
{
private:
    int m_MaxOfStatement;
    bool m_DisableIO;
    map<string,int> m_FunctionBlacklist;    //函数黑名单
    map<string,int> m_FunctionWhitelist;    //函数黑名单
    map<string,int> m_SumOfStatement;       //函数语句数

    bool m_WhitePass;
    bool m_BlackPass;
    bool m_StatementPass;
    bool m_IOPass;

protected:
    virtual void WhenDefineFunction();
    virtual void WhenCallFunction();
    virtual void WhenStatementEnd();

public:
    FunctionJudger(string& code, const char* black_list, const char* white_list, 
                    int max_statement, bool disableIO);
    ~FunctionJudger();
    void judge();
};
