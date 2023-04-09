#include <bits/stdc++.h>
using namespace std;
int main()
{
    string str;
    while(true){
        cin >> str;
        regex camelCase("[a-z]+([A-Z][a-z]+)+");
        bool found = regex_search(str, camelCase);
        if(found)
            cout << "Found\n";
        else 
            cout << "Not Found\n";
    }
    return 0;
}