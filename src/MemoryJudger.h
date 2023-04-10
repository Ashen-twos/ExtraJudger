#pragma once
#include <bits/stdc++.h>
#include "JudgerFSM.h"
#include "spdlog/spdlog.h"

using namespace std;

class MemoryJudger : public JudgerFSM
{
private:
    map<string,int> m_EnableArray;//允许的数组  

protected:
    virtual void WhenDefineArray();

public:
    MemoryJudger(string& code, const char* list);
    ~MemoryJudger();
    void judge();
};
