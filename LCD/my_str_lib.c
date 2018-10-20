/*
 * my_str_lib.c
 *
 *  Created on: 29 окт. 2013
 *      Author: dmitry
 */

#include "my_str_lib.h"

//FLOAT_BUFF_TYPE Fbuff;

char* strcopy_m (char *destination, const char *source)
{
	while(*source != '\0') {
		*destination++ = *source++;
	}
	*destination = '\0';
	return destination;
}

void reverse(char *s)
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

uint8_t itoa_m(int n, char *s)
{
    int i, sign;
    memset(s, 0, strlen(s));
    if ((sign = n) < 0)  			/* record sign */
        n = -n;          			/* make n positive */
    i = 0;
    do {       						/* generate digits in reverse order */
        s[i++] = n % 10 + '0';   	/* get next digit */
    } while ((n /= 10) > 0);     	/* delete it */
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return i;
}
