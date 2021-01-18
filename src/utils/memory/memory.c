#include <stdio.h>
#include <stdlib.h>

#ifdef linux
#include <malloc.h>
#endif

void *mem_alloc(unsigned n)
{
    void *p;
    if(!(p = malloc(n)))
    {
        fprintf(stderr,"Malloc(%d) failed.\n",n);
        fflush(stderr);
        abort();
    }
    return p;
}
