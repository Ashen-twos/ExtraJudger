#pragma once



class util{
public:
    enum judge_state{
        INIT,
        DEFINE_DATA_TYPE,
        DEFINE_PTR,
        DEFINE_VARIABLE_NAME,

        FUNCTION_DEFINE,
        FUNCTION_DEFINE_PARA,
        FUNCTION_DEFINE_PARA_TYPE,
        FUNCTION_DEFINE_PARA_NAME,

        STRUCT,
        STRUCT_DEFINE_NAME,
        STRUCT_DEFINE_MEMBER,

        VARIABLE_NAME,

        TYPE_CONVERSION,
        EXPRESSION
    };

    enum char_type{
        LETTER_OR_NUMBER,
        SYMBOL,
        SPACE
    };

    enum word_type{
        DATA_TYPE,
        SYMBOLS,
        VARIABLE,
        RESERVED_WORD
    };

    static char_type CheckCharType(char word)   //获取字符类型
    {
        if((word>='a' && word<='z') || (word>='A' && word<='Z') || word=='_')
            return LETTER_OR_NUMBER;
        else if(word>='0' && word<='9')
            return LETTER_OR_NUMBER;
        else if(word==' ' || word=='\n' || word=='\t')
            return SPACE;
        else
            return SYMBOL;
    }

    static void ParseString(const char* str, map<string,int>& mp)
    {
        vector<string> vet;
        string tmp(str);
        int las = 0;
        for(int i=0; i<=tmp.length(); i++)
        {
            if(i==tmp.length() || tmp[i] == ' ')
            {
                if(i>las)
                    vet.push_back(tmp.substr(las,i-las));
                las = i+1;
            }
        }
        for(int i=0; i<vet.size(); i++)
            mp[vet[i]] = 1;
    }
};
