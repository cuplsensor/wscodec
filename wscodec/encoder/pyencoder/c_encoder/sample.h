#ifndef _SAMPLE_H_
#define _SAMPLE_H_

#include <stdbool.h>


void sample_init(unsigned int status, bool err);
int cbuf_pushsample(int rd0, int rd1);
void cbuf_setelapsed(unsigned int minutes);

#endif //_SAMPLE_H_
