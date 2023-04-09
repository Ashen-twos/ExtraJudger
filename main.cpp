#include <bits/stdc++.h>
#include "src/ExtraJudger.h"
#include "src/Exception/JudgerException.h"

using namespace std;

ExtraJudger* judger;

void Format()
{
    judger->FormatJudge(4,true);
    cout << judger->GetResult() << '\n';
}

void Func()
{
    judger->FuncJudge("sort lower_bound sad asdasdasd safasdfad");
    cout << judger->GetResult() << '\n';
}

void Memory()
{
    judger->MemoryJudge("nums");
    cout << judger->GetResult() << '\n';
}

void Style()
{
    judger->StyleJudge("global_", "", 3, 3, 3, true);
    cout << judger->GetResult() << '\n';
}

int main(){
    //check("data/1.in", 4);
    ifstream fin("data/2.in");
    stringstream buf;
    buf << fin.rdbuf();
    string Str = buf.str();
    judger = new ExtraJudger(Str.c_str());
    
    //Func();
    //Memory();
    Style();
    delete judger;
    return 0;
}