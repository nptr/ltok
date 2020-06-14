#include <stdio.h>
#include <string.h>

#include "ltok.h"

#define tokenOpenBold               LTOK_TOKEN_USER + 0
#define tokenOpenItalic             LTOK_TOKEN_USER + 1
#define tokenOpenUnderline          LTOK_TOKEN_USER + 2
#define tokenOpenStrikethrough      LTOK_TOKEN_USER + 3
#define tokenOpenColor              LTOK_TOKEN_USER + 4
#define tokenClose                  LTOK_TOKEN_USER + 5
#define tokenEndBold                LTOK_TOKEN_USER + 6
#define tokenEndItalic              LTOK_TOKEN_USER + 7
#define tokenEndUnderline           LTOK_TOKEN_USER + 8
#define tokenEndStrikethrough       LTOK_TOKEN_USER + 9
#define tokenEndColor               LTOK_TOKEN_USER + 10


int tokenizer_callback(ltok_tokenizer_t* tokenizer, ltok_token_t* token, int num_tokens, void* userdata)
{
    if (token[0].id != LTOK_TOKEN_TEXT)
    {
        printf("found token (id: %d):\t%.*s\n", token[0].id, token[0].length, token[0].text);
    }

    return 0;
}


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
    
    const char* text = "aa[b]Hello[/b][i] [color=red]World[/color][/i]!";
    int text_len = strlen(text);

    ltok_tokenize(&bb_tokenizer, text, text_len, &tokenizer_callback, NULL);

    return 0;
}
