#ifndef LTOK_H
#define LTOK_H

#define LTOK_TOKEN_TEXT 1
#define LTOK_TOKEN_USER 16

#ifdef __cplusplus
extern "C"
{
#endif


typedef struct _ltok_token_t
{
    unsigned int id;
    const char* text;
    int length;
} ltok_token_t;


typedef struct _ltok_tokenizer_t
{
    ltok_token_t* tokens;
    int num_tokens;
} ltok_tokenizer_t;


/**
* Callback function called by the tokenizer.
*
* @param token Pointer to an array where the matching tokens are stored.
* @param num_tokens Number of tokens found.
* @param userdata The user data supplied to ltok_tokenize().
* @return a value in the range 0 and num_tokens-1. Selects the token to proceed with.
* A number < 0 (or outside the range) to indicate an error and signal the tokenizer to stop.
*/
typedef int(*ltok_token_callback)(ltok_tokenizer_t* tokenizer, ltok_token_t* token, int num_tokens, void* userdata);


/**
* Searches for the next token in a string.
*
* Can handle ambiguous grammer. All matching tokens are returned. Call this function
* in a loop and increment "input" to tokenize the whole string.
*
* @param tokenizer A pointer to the tokenizer instance.
* @param input The sequence to search in.
* @param input_len The length of the sequence.
* @param token_store Pointer to an array where matching tokens shall be stored.
* @param token_store_len Number of tokens that can be stored in the array. If
* having an ambiguous grammar, use a value > 1 or all other matches will be ignored.
* @return Number of tokens written to "token_store".
*/
int ltok_next_tokens(ltok_tokenizer_t* tokenizer, const char* input, int input_len, 
    ltok_token_t* token_store, int token_store_len);


/**
* Tokenizes the input string. For each match, the callback function is called.
*
* Can handle ambiguous grammer as far as there are no more than two possible matches
* at a position in the string. The matching tokens are passed in the callback which
* then can select how to proceed.
*
* @param tokenizer A pointer to the tokenizer instance.
* @param input The sequence to be tokenized.
* @param input_len The length of the sequence.
* @param userdata A pointer to user data.
* @param callback A callback function to be called for each found token.
*/
void ltok_tokenize_amb(ltok_tokenizer_t* tokenizer, const char* input, int input_len,
    ltok_token_callback callback, void* userdata);


/**
* Tokenizes the input string. For each matching token, the callback function is called.
*
* In case of multiple matching tokens, only the first one is returned. This means that
* this function is not suited for ambiguous grammar.
*
* @param tokenizer A pointer to the tokenizer instance.
* @param input The sequence to be tokenized.
* @param input_len The length of the sequence.
* @param userdata A pointer to user data.
* @param callback A callback function to be called for each found token.
*/
void ltok_tokenize(ltok_tokenizer_t* tokenizer, const char* input, int input_len,
    ltok_token_callback callback, void* userdata);


#ifdef __cplusplus
}
#endif


#endif