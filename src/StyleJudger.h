#pragma once
#include <bits/stdc++.h>
#include "JudgerFSM.h"
#include "spdlog/spdlog.h"
#include "Exception/JudgerException.h"

using namespace std;

class StyleJudger : public JudgerFSM
{
private:
    map<string,int> m_EnableArray;//允许的数组  

protected:
    virtual void WhenDefineArray();

public:
    StyleJudger(string& code, const char* list);
    ~StyleJudger();
    void judge();
};
