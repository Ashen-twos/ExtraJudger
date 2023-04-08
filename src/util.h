#pragma once
class util{
public:
    enum judge_state{
        INIT,
        DEFINE_DATA_TYPE,
        DEFINE_PTR,
        DEFINE_VARIABLE_NAME,
        DEFINE_FUNCTION,

        STRUCT,
        STRUCT_DEFINE_NAME,
        STRUCT_DEFINE_MEMBER,

        VARIABLE_NAME,
        FUNCTION
    };

    enum char_type{
        LETTER,
        NUMBER,
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
        if((word>='a' && word<='z') || (word>='A' && word<='Z'))
            return LETTER;
        else if(word>='0' && word<='9')
            return NUMBER;
        else if(word==' ' || word=='\n' || word=='\t')
            return SPACE;
        else
            return SYMBOL;
    }
};
