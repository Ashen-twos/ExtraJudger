#pragma once
#include <bits/stdc++.h>
#include "Exception/JudgerException.h"
#include "spdlog/spdlog.h"

using namespace std;

class ExtraJudger
{
private:
    int m_IndentSize;             //缩进长度
    bool m_LeftBigPara;           //大括号是否前置
    bool m_CurrentIsSingle;       //当前是否单行
    bool m_NextIsSingle;          //下一行是否单行

    int m_CurrentRow;           //当前行
    int m_CurrentLayer;         //当前层数
    int m_SingleSum;            //迭代次数

    int m_FuncPtr;              //函数测试指针              

    string m_RowStr;                    //当前行
    string m_Buff;                      //缓冲区
    string m_Result;                    //结果
    vector<pair<int,int>> m_MultiSum;   //多行注释前缀和

    string m_FuncWord;          //单词
    string m_PreFuncWord;       //上一个单词
    map<string,int> m_DataType; //数据类型
    map<string,int> functions;  //函数    

    void ProcessComment();      //处理注释
    bool FormatProcessRow();    //处理单行
    void FuncProcessWord();     //单词分离
    int GetRealRow(int row);    //获得真实行数

public:
    ExtraJudger(const char* src);
    ~ExtraJudger();
    void FormatJudge(int indentSize, bool leftBigPara);
    void FuncJudge(const char* func);
    const char* GetResult();
};

