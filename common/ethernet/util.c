/******************************************************************************
*
* Filename: utilities.c
*
* Purpose:  Generic utilities library
*
* History:
* 25FEB2009 Josh Eliser   -Created.
*
*******************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
	
/* system includes */

/* user includes */

/* define */

/* constants */

/* enumerations */

/* structures */



/******************************************************************************/
int EndianSwapArrayInPlace(unsigned char *buff, int numElems, int elemSize) {
/******************************************************************************/

    /* In-place endian conversion */
    register unsigned char tmp = 0;
    register unsigned char i = 0;
    unsigned char *ptr = (unsigned char *)buff;

    /* Sanity Check */
    if(buff == NULL) {
        return(-1);
    }
    /* This check could probably be condensed better */
    else if((elemSize != 2) && (elemSize != 4) && (elemSize != 8)) {
        return(-1);
    }

    /* Takes in an arbitrary sized array and performs and endian swap. */
    for( ; numElems > 0; numElems--, ptr += elemSize) {
        for(i = 0; i < (elemSize / 2); i++) {
            tmp    = ptr[i];
            ptr[i] = ptr[(elemSize-1)-i];
            ptr[(elemSize-1)-i] = tmp;
        }
    }

    return(i);
}


#ifdef __cplusplus
}
#endif
