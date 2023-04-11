#pragma once
#include <bits/stdc++.h>
#include "JudgerFSM.h"
#include "spdlog/spdlog.h"

using namespace std;

class MemoryJudger : public JudgerFSM
{
private:
    bool m_CheckPtrFree;
    map<string,int> m_EnableArray;  //允许的数组  
    map<string,int> m_MallocPtr;

protected:
    virtual void WhenDefineArray();
    virtual void WhenStatementEnd();
    virtual void WhenCallFunction();

public:
    MemoryJudger(string& code, const char* white_list, bool check_ptr_free);
    ~MemoryJudger();
    void judge();
};
