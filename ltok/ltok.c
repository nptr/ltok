#include "ltok.h"

#include <string.h>


int ltok_find_tokens(ltok_tokenizer_t* tokenizer, const char* input, int input_len, ltok_token_t* token_store, int token_store_len)
{
    int result_index = 0;

    for (int i = 0; i < tokenizer->num_tokens; ++i)
    {
        ltok_token_t* current_token = &tokenizer->tokens[i];
        if (input_len < current_token->length)
        {
            continue;
        }

        if (memcmp(input, current_token->text, current_token->length) == 0)
        {
            if (result_index < token_store_len)
            {
                token_store[result_index].id = current_token->id;
                token_store[result_index].text = input;
                token_store[result_index].length = current_token->length;
                result_index++;

                if (result_index >= token_store_len)
                {
                    break;
                }
            }
        }
    }

    return result_index;
}


int ltok_next_tokens(ltok_tokenizer_t* tokenizer, const char* input, int input_len, ltok_token_t* token_store, int token_store_len)
{
    const char* start = input;
    int remaining = input_len;

    int tokens_found = 0;
    while (remaining && (tokens_found = ltok_find_tokens(
        tokenizer,
        start,
        remaining,
        token_store,
        token_store_len)) == 0)
    {
        start++;
        remaining--;
    }

    int end_text = remaining == 0 && input_len > 0;
    int next_token = input != start && tokens_found;
    if (next_token || end_text)
    {
        /*
        * We have found the next token or end of text, but have to return the
        * preceeding text first. I chose to discard the result,
        * return the text and find the token again in the next invocation.
        * I'm trading speed for reduced complexity / memory.
        */
        token_store[0].id = LTOK_TOKEN_TEXT;
        token_store[0].text = input;
        token_store[0].length = input_len - remaining;
        return 1;
    }
    else return tokens_found;
}


void ltok_tokenize_amb(ltok_tokenizer_t* tokenizer, const char* input, int input_len, ltok_token_callback callback, void* userdata)
{
    ltok_token_t token[2];

    const char* begin = input;
    int remaining = input_len;
    int num_found = 0;
    while ((num_found = ltok_next_tokens(tokenizer
        , begin
        , remaining
        , (ltok_token_t*)&token
        , 2)) > 0)
    {
        int sel_index = callback(tokenizer, &token[0], num_found, userdata);
        if (sel_index < 0 || sel_index > num_found - 1)
        {
            return;
        }

        begin += token[sel_index].length;
        remaining -= token[sel_index].length;
    }
}


void ltok_tokenize(ltok_tokenizer_t* tokenizer, const char* input, int input_len, ltok_token_callback callback, void* userdata)
{
    ltok_token_t token;

    const char* begin = input;
    int remaining = input_len;
    int num_found = 0;
    while ((num_found = ltok_next_tokens(tokenizer
        , begin
        , remaining
        , &token
        , 1)) > 0)
    {
        if(callback(tokenizer, &token, num_found, userdata) != 0)
        {
            return;
        }

        begin += token.length;
        remaining -= token.length;
    }
}