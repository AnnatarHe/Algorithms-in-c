#include <ctype.h>
#include <stdlib.h>
#include <string.h>

#include "chtbl.h"
#include "lex.h"
// 书里没写
#include "symbol.h"

Token lex(const char *istream, CHTbl *symtbl)
{
    Token token;
    Symbol *symbol;
    int length,
        retval,
        i;
    if ((symbol = (Symbol *)malloc(sizeof(Symbol))) == NULL) {
        return error;
    }

    // 进行下一个token
    if ((symbol->lexme = next_token(istream)) == NULL) {
        // 没有输入了就返回
        free(symbol);
        return lexit;
    }else {
        symbol->token = digit;
        length = strlen(symbol->lexme);

        for (i = 0; i < length; i++) {
            if (!isdigit(symbol->lexme[i])) {
                symbol->token = other;
            }
        }

        memcpy(&token, &symbol->token, sizeof(Token));

        if ((retval = chtbl_insert(symtbl, symbol)) < 0) {
            free(symbol);
            return error;
        }else if (retval == 1) {
            free(symbol);
        }
    }

    // 给解析器返回token
    return token;
}
