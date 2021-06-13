#ifndef MAYER_H
#define MAYER_H

#ifdef __cplusplus  
extern "C" {  
#endif

#define REAL float

void mayer_realfft(int n, REAL *real);
void mayer_realifft(int n, REAL *real);

#ifdef __cplusplus  
}
#endif

#endif
