#pragma once
#include <string>

using namespace std;

class ExtraJudger
{
private:
    string filePath;
    int indentSize;
    
    bool isComment;         //是否处于多行注释
    string rowStr;          //当前行

public:
    ExtraJudger(string filePath, int indentSize);
    ~ExtraJudger();
};

ExtraJudger::ExtraJudger(string filePath, int indentSize=4)
:filePath(filePath), indentSize(indentSize){}

ExtraJudger::~ExtraJudger(){}


