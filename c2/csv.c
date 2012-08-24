

/* csv.c  */    
/* Written by Phil Bewig */  

/* ( He originally posted the code on the */  
/* comp.lang.awk list, in reply to a question asking */ 
/* how to parse csv files. )  */ 
/* Please see the accompanying README for more background */ 
/* and usage notes. */  
/* As Phil says in the accompanying doc - this code is */ 
/* released to the public domain.  */  
/* "Share and enjoy...... "  ;)    */        
  

#include <stdio.h>

/* dofile -- convert one file from comma-separated to delimited */
void dofile(char ofs, char fs, FILE *f) {
    int c; /* current input character */

    START:
        c = fgetc(f);
        if (c == EOF)  {                     return; }
        if (c == '\r') {                     goto CARRIAGE_RETURN; }
        if (c == '\n') {                     goto LINE_FEED; }
        if (c == '\"') {                     goto QUOTED_FIELD; }
        if (c == fs)   { putchar(ofs);       goto NOT_FIELD; }
        /* default */  { putchar(c);         goto UNQUOTED_FIELD; }

    NOT_FIELD:
        c = fgetc(f);
        if (c == EOF)  { putchar('\n');      return; }
        if (c == '\r') {                     goto CARRIAGE_RETURN; }
        if (c == '\n') {                     goto LINE_FEED; }
        if (c == '\"') {                     goto QUOTED_FIELD; }
        if (c == fs)   { putchar(ofs);       goto NOT_FIELD; }
        /* default */  { putchar(c);         goto UNQUOTED_FIELD; }

    QUOTED_FIELD:
        c = fgetc(f);
        if (c == EOF)  { putchar('\n');      return; }
        if (c == '\"') {                     goto MAY_BE_DOUBLED_QUOTES; }
        /* default */  { putchar(c);         goto QUOTED_FIELD; }

    MAY_BE_DOUBLED_QUOTES:
        c = fgetc(f);
        if (c == EOF)  { putchar('\n');      return; }
        if (c == '\r') {                     goto CARRIAGE_RETURN; }
        if (c == '\n') {                     goto LINE_FEED; }
        if (c == '\"') { putchar('\"');      goto QUOTED_FIELD; }
        if (c == fs)   { putchar(ofs);       goto NOT_FIELD; }
        /* default */  { putchar(c);         goto UNQUOTED_FIELD; }

    UNQUOTED_FIELD:
        c = fgetc(f);
        if (c == EOF)  { putchar('\n');      return; }
        if (c == '\r') {                     goto CARRIAGE_RETURN; }
        if (c == '\n') {                     goto LINE_FEED; }
        if (c == fs)   { putchar(ofs);       goto NOT_FIELD; }
        /* default */  { putchar(c);         goto UNQUOTED_FIELD; }

    CARRIAGE_RETURN:
        c = fgetc(f);
        if (c == EOF)  { putchar('\n');      return; }
        if (c == '\r') { putchar('\n');      goto CARRIAGE_RETURN; }
        if (c == '\n') { putchar('\n');      goto START; }
        if (c == '\"') { putchar('\n');      goto QUOTED_FIELD; }
        if (c == fs)   { printf("\n%c",ofs); goto NOT_FIELD; }
        /* default */  { printf("\n%c",c);   goto UNQUOTED_FIELD; }

    LINE_FEED:
        c = fgetc(f);
        if (c == EOF)  { putchar('\n');      return; }
        if (c == '\r') { putchar('\n');      goto START; }
        if (c == '\n') { putchar('\n');      goto LINE_FEED; }
        if (c == '\"') { putchar('\n');      goto QUOTED_FIELD; }
        if (c == fs)   { printf("\n%c",ofs); goto NOT_FIELD; }
        /* default */  { printf("\n%c",c);   goto UNQUOTED_FIELD; }
}



/* ***************************************************** */ 
/* main -- process command line, call appropriate conversion */
/* ***************************************************** */ 

int main(int argc, char *argv[]) {
    char ofs = '\034'; /* output field separator */
    char fs = ',';     /* input field separator */
    int  status = 0;   /* error status for return to operating system */
    char *progname;    /* name of program for error messages */

    FILE *f;
    int i;

    progname = (char *) malloc(strlen(argv[0])+1);
    strcpy(progname, argv[0]);

    while (argc > 1 && argv[1][0] == '-') {
        switch (argv[1][1]) {
            case 'c':
            case 'C':
                fs = argv[1][2];
                break;
            case 'f':
            case 'F':
                ofs = argv[1][2];
                break;
            default:
                fprintf(stderr, "%s: unknown argument %s\n",
                    progname, argv[1]);
                fprintf(stderr,
                   "usage: %s [-Cc] [-Fc] [filename ...]\n",
                    progname);
                exit(1);
        }
        argc--;
        argv++;
    }

    if (argc == 1)
        dofile(ofs, fs, stdin);
    else
        for (i = 1; i < argc; i++)
            if ((f = fopen(argv[i], "r")) == NULL) {
                fprintf(stderr, "%s: can't open %s\n",
                    progname, argv[i]);
                status = 2;
            } else {
                dofile(ofs, fs, f);
                fclose(f);
            }

    exit(status);
}


