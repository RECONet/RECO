#include "bstrlib.h"
#include <stdarg.h>	

#if defined (__TURBOC__) && !defined (__BORLANDC__)
# ifndef BSTRLIB_NOVSNP
#  define BSTRLIB_NOVSNP
# endif
#endif

/* Give WATCOM C/C++, MSVC some latitude for their non-support of vsnprintf */
#if defined(__WATCOMC__) || defined(_MSC_VER)
#define exvsnprintf(r,b,n,f,a) {r = _vsnprintf (b,n,f,a);}
#else
#ifdef BSTRLIB_NOVSNP
/* This is just a hack.  If you are using a system without a vsnprintf, it is
   not recommended that bformat be used at all. */
#define exvsnprintf(r,b,n,f,a) {vsprintf (b,f,a); r = -1;}
#define START_VSNBUFF (256)
#else

#if defined(__GNUC__) && !defined(__APPLE__)
/* Something is making gcc complain about this prototype not being here, so
   I've just gone ahead and put it in. */
extern int vsnprintf (char *buf, size_t count, const char *format, va_list arg);
#endif

#define exvsnprintf(r,b,n,f,a) {r = vsnprintf (b,n,f,a);}
#endif
#endif

#ifndef BSTRLIB_NOVSNP
#endif

#ifndef START_VSNBUFF
#define START_VSNBUFF (16)
#endif

/*  int bdestroy (bstring b)
 *
 *  Free up the bstring.  Note that if b is detectably invalid or not writable
 *  then no action is performed and BSTR_ERR is returned.  Like a freed memory
 *  allocation, dereferences, writes or any other action on b after it has
 *  been bdestroyed is undefined.
 */
int bdestroy (bstring b) {
	if (b == NULL || b->slen < 0 || b->mlen <= 0 || b->mlen < b->slen ||
	    b->data == NULL)
		return BSTR_ERR;

	bstr__free (b->data);

	/* In case there is any stale usage, there is one more chance to
	   notice this error. */

	b->slen = -1;
	b->mlen = -__LINE__;
	b->data = NULL;

	bstr__free (b);
	return BSTR_OK;
}

void bdestroy_wrapper(bstring *b)
{
  if ((b) && (*b)) {
    bdestroy(*b);
    *b = NULL;
  }
}

/* Compute the snapped size for a given requested size.  By snapping to powers
   of 2 like this, repeated reallocations are avoided. */
static int snapUpSize (int i) {
	if (i < 8) {
		i = 8;
	} else {
		unsigned int j;
		j = (unsigned int) i;

		j |= (j >>  1);
		j |= (j >>  2);
		j |= (j >>  4);
		j |= (j >>  8);		/* Ok, since int >= 16 bits */
#if (UINT_MAX != 0xffff)
		j |= (j >> 16);		/* For 32 bit int systems */
#if (UINT_MAX > 0xffffffffUL)
		j |= (j >> 32);		/* For 64 bit int systems */
#endif
#endif
		/* Least power of two greater than i */
		j++;
		if ((int) j >= i) i = (int) j;
	}
	return i;
}

/*  int balloc (bstring b, int len)
 *
 *  Increase the size of the memory backing the bstring b to at least len.
 */
int balloc (bstring b, int olen) {
	int len;
	if (b == NULL || b->data == NULL || b->slen < 0 || b->mlen <= 0 ||
	    b->mlen < b->slen || olen <= 0) {
		return BSTR_ERR;
	}

	if (olen >= b->mlen) {
		unsigned char * x;

		if ((len = snapUpSize (olen)) <= b->mlen) return BSTR_OK;

		/* Assume probability of a non-moving realloc is 0.125 */
		if (7 * b->mlen < 8 * b->slen) {

			/* If slen is close to mlen in size then use realloc to reduce
			   the memory defragmentation */

			reallocStrategy:;

			x = (unsigned char *) bstr__realloc (b->data, (size_t) len);
			if (x == NULL) {

				/* Since we failed, try allocating the tighest possible
				   allocation */

				len = olen;
				x = (unsigned char *) bstr__realloc (b->data, (size_t) olen);
				if (NULL == x) {
					return BSTR_ERR;
				}
			}
		} else {

			/* If slen is not close to mlen then avoid the penalty of copying
			   the extra bytes that are allocated, but not considered part of
			   the string */

			if (NULL == (x = (unsigned char *) bstr__alloc ((size_t) len))) {

				/* Perhaps there is no available memory for the two
				   allocations to be in memory at once */

				goto reallocStrategy;

			} else {
				if (b->slen) bstr__memcpy ((char *) x, (char *) b->data,
				                           (size_t) b->slen);
				bstr__free (b->data);
			}
		}
		b->data = x;
		b->mlen = len;
		b->data[b->slen] = (unsigned char) '\0';

#if defined (BSTRLIB_TEST_CANARY)
		if (len > b->slen + 1) {
			memchr (b->data + b->slen + 1, 'X', len - (b->slen + 1));
		}
#endif
	}

	return BSTR_OK;
}

bstring bstrcpy (const_bstring b) {
bstring b0;
int i,j;

	/* Attempted to copy an invalid string? */
	if (b == NULL || b->slen < 0 || b->data == NULL) return NULL;

	b0 = (bstring) bstr__alloc (sizeof (struct tagbstring));
	if (b0 == NULL) {
		/* Unable to allocate memory for string header */
		return NULL;
	}

	i = b->slen;
	j = snapUpSize (i + 1);

	b0->data = (unsigned char *) bstr__alloc (j);
	if (b0->data == NULL) {
		j = i + 1;
		b0->data = (unsigned char *) bstr__alloc (j);
		if (b0->data == NULL) {
			/* Unable to allocate memory for string data */
			bstr__free (b0);
			return NULL;
		}
	}

	b0->mlen = j;
	b0->slen = i;

	if (i) bstr__memcpy ((char *) b0->data, (char *) b->data, i);
	b0->data[b0->slen] = (unsigned char) '\0';

	return b0;
}

/*  int bcatblk (bstring b, const void * s, int len)
 *
 *  Concatenate a fixed length buffer to a bstring.
 */
int bcatblk (bstring b, const void * s, int len) {
int nl;

	if (b == NULL || b->data == NULL || b->slen < 0 || b->mlen < b->slen
	 || b->mlen <= 0 || s == NULL || len < 0) return BSTR_ERR;

	if (0 > (nl = b->slen + len)) return BSTR_ERR; /* Overflow? */
	if (b->mlen <= nl && 0 > balloc (b, nl + 1)) return BSTR_ERR;

	bBlockCopy (&b->data[b->slen], s, (size_t) len);
	b->slen = nl;
	b->data[nl] = (unsigned char) '\0';
	return BSTR_OK;
}

/*  int bcatcstr (bstring b, const char * s)
 *
 *  Concatenate a char * string to a bstring.
 */
int bcatcstr (bstring b, const char * s) {
char * d;
int i, l;

	if (b == NULL || b->data == NULL || b->slen < 0 || b->mlen < b->slen
	 || b->mlen <= 0 || s == NULL) return BSTR_ERR;

	/* Optimistically concatenate directly */
	l = b->mlen - b->slen;
	d = (char *) &b->data[b->slen];
	for (i=0; i < l; i++) {
		if ((*d++ = *s++) == '\0') {
			b->slen += i;
			return BSTR_OK;
		}
	}
	b->slen += i;

	/* Need to explicitely resize and concatenate tail */
	return bcatblk (b, (const void *) s, (int) strlen (s));
}

/*  int bconchar (bstring b, char c)
 *
 *  Concatenate the single character c to the bstring b.
 */
int bconchar (bstring b, char c) {
int d;

	if (b == NULL) return BSTR_ERR;
	d = b->slen;
	if ((d | (b->mlen - d)) < 0 || balloc (b, d + 2) != BSTR_OK)
		return BSTR_ERR;
	b->data[d] = (unsigned char) c;
	b->data[d + 1] = (unsigned char) '\0';
	b->slen++;
	return BSTR_OK;
}

/*  bstring bfromcstr (const char * str)
 *
 *  Create a bstring which contains the contents of the '\0' terminated char *
 *  buffer str.
 */
bstring bfromcstr (const char * str) {
bstring b;
int i;
size_t j;

	if (str == NULL) return NULL;
	j = (strlen) (str);
	i = snapUpSize ((int) (j + (2 - (j != 0))));
	if (i <= (int) j) return NULL;

	b = (bstring) bstr__alloc (sizeof (struct tagbstring));
	if (NULL == b) return NULL;
	b->slen = (int) j;
	if (NULL == (b->data = (unsigned char *) bstr__alloc (b->mlen = i))) {
		bstr__free (b);
		return NULL;
	}

	bstr__memcpy (b->data, str, j+1);
	return b;
}

/*  int bassigncstr (bstring a, const char * str)
 *
 *  Overwrite the string a with the contents of char * string str.  Note that
 *  the bstring a must be a well defined and writable bstring.  If an error
 *  occurs BSTR_ERR is returned however a may be partially overwritten.
 */
int bassigncstr (bstring a, const char * str) {
int i;
size_t len;
	if (a == NULL || a->data == NULL || a->mlen < a->slen ||
	    a->slen < 0 || a->mlen == 0 || NULL == str)
		return BSTR_ERR;

	for (i=0; i < a->mlen; i++) {
		if ('\0' == (a->data[i] = str[i])) {
			a->slen = i;
			return BSTR_OK;
		}
	}

	a->slen = i;
	len = strlen (str + i);
	if (len + 1 > INT_MAX - i ||
	    0 > balloc (a, (int) (i + len + 1))) return BSTR_ERR;
	bBlockCopy (a->data + i, str + i, (size_t) len + 1);
	a->slen += (int) len;
	return BSTR_OK;
}

/*  bstring bformat (const char * fmt, ...)
 *
 *  Takes the same parameters as printf (), but rather than outputting results
 *  to stdio, it forms a bstring which contains what would have been output.
 *  Note that if there is an early generation of a '\0' character, the
 *  bstring will be truncated to this end point.
 */
bstring bformat (const char * fmt, ...) {
va_list arglist;
bstring buff;
int n, r;

	if (fmt == NULL) return NULL;

	/* Since the length is not determinable beforehand, a search is
	   performed using the truncating "vsnprintf" call (to avoid buffer
	   overflows) on increasing potential sizes for the output result. */

	if ((n = (int) (2*strlen (fmt))) < START_VSNBUFF) n = START_VSNBUFF;
	if (NULL == (buff = bfromcstralloc (n + 2, ""))) {
		n = 1;
		if (NULL == (buff = bfromcstralloc (n + 2, ""))) return NULL;
	}

	for (;;) {
		va_start (arglist, fmt);
		exvsnprintf (r, (char *) buff->data, n + 1, fmt, arglist);
		va_end (arglist);

		buff->data[n] = (unsigned char) '\0';
		buff->slen = (int) (strlen) ((char *) buff->data);

		if (buff->slen < n) break;

		if (r > n) n = r; else n += n;

		if (BSTR_OK != balloc (buff, n + 2)) {
			bdestroy (buff);
			return NULL;
		}
	}

	return buff;
}

/*  bstring bfromcstralloc (int mlen, const char * str)
 *
 *  Create a bstring which contains the contents of the '\0' terminated
 *  char* buffer str.  The memory buffer backing the string is at least
 *  mlen characters in length.
 */
bstring bfromcstralloc (int mlen, const char * str) {
	return bfromcstrrangealloc (mlen, mlen, str);
}

/*  bstring bfromcstrrangealloc (int minl, int maxl, const char* str)
 *
 *  Create a bstring which contains the contents of the '\0' terminated
 *  char* buffer str.  The memory buffer backing the string is at least
 *  minl characters in length, but an attempt is made to allocate up to
 *  maxl characters.
 */
bstring bfromcstrrangealloc (int minl, int maxl, const char* str) {
bstring b;
int i;
size_t j;

	/* Bad parameters? */
	if (str == NULL) return NULL;
	if (maxl < minl || minl < 0) return NULL;

	/* Adjust lengths */
	j = (strlen) (str);
	if ((size_t) minl < (j+1)) minl = (int) (j+1);
	if (maxl < minl) maxl = minl;
	i = maxl;

	b = (bstring) bstr__alloc (sizeof (struct tagbstring));
	if (b == NULL) return NULL;
	b->slen = (int) j;

	while (NULL == (b->data = (unsigned char *) bstr__alloc (b->mlen = i))) {
		int k = (i >> 1) + (minl >> 1);
		if (i == k || i < minl) {
			bstr__free (b);
			return NULL;
		}
		i = k;
	}

	bstr__memcpy (b->data, str, j+1);
	return b;
}

