#pragma once
#include "Exception/JudgerException.h"
#include "FormatJudger.h"
#include "FunctionJudger.h"
#include "MemoryJudger.h"
#include "StyleJudger.h"
#include "spdlog/spdlog.h"

using namespace std;

class ExtraJudger
{
private:
    int m_CurrentRow;           //当前行           

    string m_Buff;                      //缓冲区
    string m_Result;                    //结果
    vector<pair<int,int>> m_MultiSum;   //多行注释前缀和
      

    void ProcessComment();      //处理注释
    int GetRealRow(int row);    //获得真实行数

public:
    ExtraJudger(const char* src);
    ~ExtraJudger();
    void FormatJudge(int indentSize, bool leftBigPara);
    void FuncJudge(const char* func);
    void MemoryJudge(const char* list);
    void StyleJudge(const char* global_prefix, const char* white_list,
                    int func_naming, int global_naming, int local_naming, 
                    bool single_name);
    const char* GetResult();
};

