#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#ifndef bstr__alloc
#define bstr__alloc(x) malloc (x)
#endif

#ifndef bstr__free
#define bstr__free(p) free (p)
#endif

#ifndef bstr__memcpy
#define bstr__memcpy(d,s,l) memcpy ((d), (s), (l))
#endif

#ifndef bstr__realloc
#define bstr__realloc(p,x) realloc ((p), (x))
#endif

#ifndef bstr__memmove
#define bstr__memmove(d,s,l) memmove ((d), (s), (l))
#endif

#ifndef BSTRLIB_H
#define BSTRLIB_H

#define bdataofse(b, o, e)  (((b) == (void *)0 || (b)->data == (void*)0) ? (char *)(e) : ((char *)(b)->data) + (o))
#define bdataofs(b, o)      (bdataofse ((b), (o), (void *)0))
#define bdata(b)            (bdataofs (b, 0))
#define blengthe(b, e)      (((b) == (void *)0 || (b)->slen < 0) ? (int)(e) : ((b)->slen))
#define blength(b)          (blengthe ((b), 0))

#define bBlockCopy(D,S,L) { if ((L) > 0) bstr__memmove ((D),(S),(L)); }

#define BSTR_ERR (-1)
#define BSTR_OK (0)

struct tagbstring {
	int mlen;
	int slen;
	unsigned char * data;
};

typedef struct tagbstring * bstring;
typedef const struct tagbstring * const_bstring;

int bdestroy (bstring b);
void bdestroy_wrapper(bstring *b);
static int snapUpSize (int i);
int balloc (bstring b, int olen);
bstring bstrcpy (const_bstring b);
int bcatblk (bstring b, const void * s, int len);
int bcatcstr (bstring b, const char * s);
int bconchar (bstring b, char c);
bstring bfromcstr (const char * str);
int bassigncstr (bstring a, const char * str);
bstring bformat (const char * fmt, ...);
bstring bfromcstralloc (int mlen, const char * str);
bstring bfromcstrrangealloc (int minl, int maxl, const char* str);

#endif
