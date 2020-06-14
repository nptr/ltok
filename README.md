# ltok [![License: MIT](https://img.shields.io/badge/License-MIT-brightgreen.svg)](https://opensource.org/licenses/MIT)


"ltok" is a lightweight tokenizer written in C. It doesn't allocate dynamic memory and tokenizes the input<br>
string in one pass. Ambiguous grammer is supported in that the user can select from the matching tokens and with it control the tokenizing process.


## Usage
For such a small utility, no library and build system is provided.
Just add `ltok.c`and `ltok.h` to your project<br>
and compile as usual. Any C++ or C99(-ish) compiler for any platform should be fine with it.

Work in-function with `ltok_next_tokens()` or use the callback interface as shown below:
```C
int main(int argc, char** argv)
{
    ltok_token_t bb_tokens[] =
    {
        { tokenOpenBold             , "[b"          , 2 },
        { tokenOpenItalic           , "[i"          , 2 },
        { tokenOpenUnderline        , "[u"          , 2 },
        { tokenOpenStrikethrough    , "[s"          , 2 },
        { tokenOpenColor            , "[color"      , 6 },
        { tokenClose                , "]"           , 1 },
        { tokenEndBold              , "[/b]"        , 4 },
        { tokenEndItalic            , "[/i]"        , 4 },
        { tokenEndUnderline         , "[/u]"        , 4 },
        { tokenEndStrikethrough     , "[/s]"        , 4 },
        { tokenEndColor             , "[/color]"    , 8 }
    };

    ltok_tokenizer_t bb_tokenizer =
    {
        bb_tokens,
        sizeof(bb_tokens) / sizeof(bb_tokens[0])
    };
    
    const char* text = "[b]Hello[/b][i] [color=red]World[/color][/i]!";

    ltok_tokenize(&bb_tokenizer, text, strlen(text), 
        &tokenizer_callback, NULL);

    return 0;
}
```