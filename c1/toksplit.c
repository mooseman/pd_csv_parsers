/* This code is from here -  */ 
/* http://bytes.com/topic/c/answers/456555-spaces-c  */  
/* Author: C.B. Falconer.  */  

#include "toksplit.h"

/* copy over the next token from an input string, after
skipping leading blanks (or other whitespace?). The
token is terminated by the first appearance of tokchar,
or by the end of the source string.

The caller must supply sufficient space in token to
receive any token, Otherwise tokens will be truncated.

Returns: a pointer past the terminating tokchar.

This will happily return an infinity of empty tokens if
called with src pointing to the end of a string. Tokens
will never include a copy of tokchar.

A better name would be "strtkn", except that is reserved
for the system namespace. Change to that at your risk.

released to Public Domain, by C.B. Falconer.
Published 2006-02-20. Attribution appreciated.
*/

const char *toksplit(const char *src, /* Source of tokens */
char tokchar, /* token delimiting char */
char *token, /* receiver of parsed token */
size_t lgh) /* length token can receive */
/* not including final '\0' */
{
if (src) {
while (' ' == *src) *src++;

while (*src && (tokchar != *src)) {
if (lgh) {
*token++ = *src;
--lgh;
}
src++;
}
if (*src && (tokchar == *src)) src++;
}
*token = '\0';
return src;
} /* toksplit */

#ifdef TESTING
#include <stdio.h>

#define ABRsize 6 /* length of acceptable token abbreviations */

int main(void)
{
char teststring[] = "This is a test, ,, abbrev, more";

const char *t, *s = teststring;
int i;
char token[ABRsize + 1];

puts(teststring);
t = s;
for (i = 0; i < 4; i++) {
t = toksplit(t, ',', token, ABRsize);
putchar(i + '1'); putchar(':');
puts(token);
}

puts("\nHow to detect 'no more tokens'");
t = s; i = 0;
while (*t) {
t = toksplit(t, ',', token, 3);
putchar(i + '1'); putchar(':');
puts(token);
i++;
}

puts("\nUsing blanks as token delimiters");
t = s; i = 0;
while (*t) {
t = toksplit(t, ' ', token, ABRsize);
putchar(i + '1'); putchar(':');
puts(token);
i++;
}
return 0;
} /* main */

#endif


 
