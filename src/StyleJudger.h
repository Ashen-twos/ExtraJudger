#pragma once
#include <bits/stdc++.h>
#include "JudgerFSM.h"
#include "spdlog/spdlog.h"

#define CAMEL_CASE 1
#define UPPER_CASE 2
#define LOWER_CASE 3 

using namespace std;

class StyleJudger : public JudgerFSM
{
private:
    int m_FunctionNaming;
    int m_GlobalNaming;
    int m_LocalNaming;
    bool m_SingleName; 
    string m_GlobalPrefix;
    map<string,int> m_WhiteList;


    bool m_GNamePass;
    bool m_FNamePass;
    bool m_LNamePass;
    bool m_PrefixPass;
    bool m_SinglePass;

    bool CheckCamelCase(string& word);
    void CheckNaming(string& word, int style, int type);


protected:
    virtual void WhenDefineVariable();
    virtual void WhenDefineFunction();
    virtual void WhenDefineArray();

public:
    StyleJudger(string& code, const char* global_prefix, const char* white_list,
                int func_naming, int global_naming, int local_naming, 
                bool single_name);
    ~StyleJudger();
    void judge();
};
