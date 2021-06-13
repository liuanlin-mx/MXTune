#ifndef __FFT_H__
#define __FFT_H__
#include "mayer_fft.h"

// Variables for FFT routine
typedef struct
{
    int nfft;        // size of FFT
    int numfreqs;    // number of frequencies represented (nfft/2 + 1)
    float* fft_data; // array for writing/reading to/from FFT function
} fft_vars;

fft_vars* fft_con(int nfft);
void fft_des(fft_vars* membvars);
void fft_forward(fft_vars* membvars, float* input, float* output_re, float* output_im);
void fft_inverse(fft_vars* membvars, float* input_re, float* input_im, float* output);

#endif