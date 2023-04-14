#include <bits/stdc++.h>
#include "src/ExtraJudger.h"
#include "src/Exception/JudgerException.h"

using namespace std;

ExtraJudger* judger;

void Format()
{
    cout << judger->FormatJudge(4,true,true,3) << '\n';
    cout << judger->GetResult() << '\n';
}

void Func()
{
    cout << judger->FuncJudge("sort lower_bound", "init", 5, true) << '\n';
    cout << judger->GetResult() << '\n';
}

void Memory()
{
    cout << judger->MemoryJudge("nums", true) << '\n';
    cout << judger->GetResult() << '\n';
}

void Style()
{
    cout << judger->StyleJudge("global_", "ans", 3, 3, 3, true) << '\n';
    cout << judger->GetResult() << '\n';
}

int main(){
    //check("data/1.in", 4);
    ifstream fin("data/2.in");
    stringstream buf;
    buf << fin.rdbuf();
    string Str = buf.str();
    judger = new ExtraJudger(Str.c_str());
    
    //Format();
    //Func();
    Memory();
    //Style();
    delete judger;
    return 0;
}