/* autotalent.c
   A pitch-correcting LADSPA plugin.

   Free software by Thomas A. Baran.
   http://web.mit.edu/tbaran/www/autotalent.html
   VERSION 0.2
   March 20, 2010

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 */

/*****************************************************************************/

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "net_log.h"
#define PI (float)3.14159265358979323846
#define L2SC (float)3.32192809488736218171

/*****************************************************************************/

#include "lv2.h"
#include "mayer_fft.h"


// Variables for FFT routine
typedef struct
{
    int nfft;        // size of FFT
    int numfreqs;    // number of frequencies represented (nfft/2 + 1)
    float* fft_data; // array for writing/reading to/from FFT function
} fft_vars;

// Constructor for FFT routine
fft_vars* fft_con(int nfft)
{
    fft_vars* membvars = (fft_vars*)malloc(sizeof(fft_vars));

    membvars->nfft = nfft;
    membvars->numfreqs = nfft / 2 + 1;

    membvars->fft_data = (float*)calloc(nfft, sizeof(float));

    return membvars;
}

// Destructor for FFT routine
void fft_des(fft_vars* membvars)
{
    free(membvars->fft_data);

    free(membvars);
}

// Perform forward FFT of real data
// Accepts:
//   membvars - pointer to struct of FFT variables
//   input - pointer to an array of (real) input values, size nfft
//   output_re - pointer to an array of the real part of the output,
//     size nfft/2 + 1
//   output_im - pointer to an array of the imaginary part of the output,
//     size nfft/2 + 1
void fft_forward(fft_vars* membvars, float* input, float* output_re, float* output_im)
{
    int ti;
    int nfft;
    int hnfft;
    int numfreqs;

    nfft = membvars->nfft;
    hnfft = nfft / 2;
    numfreqs = membvars->numfreqs;

    for(ti = 0; ti < nfft; ti++)
    {
        membvars->fft_data[ti] = input[ti];
    }

    mayer_realfft(nfft, membvars->fft_data);

    output_im[0] = 0;
    for(ti = 0; ti < hnfft; ti++)
    {
        output_re[ti] = membvars->fft_data[ti];
        output_im[ti + 1] = membvars->fft_data[nfft - 1 - ti];
    }
    output_re[hnfft] = membvars->fft_data[hnfft];
    output_im[hnfft] = 0;
}

// Perform inverse FFT, returning real data
// Accepts:
//   membvars - pointer to struct of FFT variables
//   input_re - pointer to an array of the real part of the output,
//     size nfft/2 + 1
//   input_im - pointer to an array of the imaginary part of the output,
//     size nfft/2 + 1
//   output - pointer to an array of (real) input values, size nfft
void fft_inverse(fft_vars* membvars, float* input_re, float* input_im, float* output)
{
    int ti;
    int nfft;
    int hnfft;
    int numfreqs;

    nfft = membvars->nfft;
    hnfft = nfft / 2;
    numfreqs = membvars->numfreqs;

    for(ti = 0; ti < hnfft; ti++)
    {
        membvars->fft_data[ti] = input_re[ti];
        membvars->fft_data[nfft - 1 - ti] = input_im[ti + 1];
    }
    membvars->fft_data[hnfft] = input_re[hnfft];

    mayer_realifft(nfft, membvars->fft_data);

    for(ti = 0; ti < nfft; ti++)
    {
        output[ti] = membvars->fft_data[ti];
    }
}

// DONE WITH FFT CODE

// The port numbers

#define AT_TUNE 0
#define AT_FIXED 1
#define AT_PULL 2
#define AT_A 3
#define AT_Bb 4
#define AT_B 5
#define AT_C 6
#define AT_Db 7
#define AT_D 8
#define AT_Eb 9
#define AT_E 10
#define AT_F 11
#define AT_Gb 12
#define AT_G 13
#define AT_Ab 14
#define AT_AMOUNT 15
#define AT_SMOOTH 16
#define AT_SHIFT 17
#define AT_SCWARP 18
#define AT_LFOAMP 19
#define AT_LFORATE 20
#define AT_LFOSHAPE 21
#define AT_LFOSYMM 22
#define AT_LFOQUANT 23
#define AT_FCORR 24
#define AT_FWARP 25
#define AT_MIX 26
#define AT_PITCH 27
#define AT_CONF 28
#define AT_INPUT1 29
#define AT_OUTPUT1 30
#define AT_LATENCY 31

/*************************
 *  THE MEMBER VARIABLES *
 *************************/

typedef struct
{

    float* m_pfTune;
    float* m_pfFixed;
    float* m_pfPull;
    float* m_pfA;
    float* m_pfBb;
    float* m_pfB;
    float* m_pfC;
    float* m_pfDb;
    float* m_pfD;
    float* m_pfEb;
    float* m_pfE;
    float* m_pfF;
    float* m_pfGb;
    float* m_pfG;
    float* m_pfAb;
    float* m_pfAmount;
    float* m_pfSmooth;
    float* m_pfShift;
    float* m_pfScwarp;
    float* m_pfLfoamp;
    float* m_pfLforate;
    float* m_pfLfoshape;
    float* m_pfLfosymm;
    float* m_pfLfoquant;
    float* m_pfFcorr;
    float* m_pfFwarp;
    float* m_pfMix;
    float* m_pfPitch;
    float* m_pfConf;
    float* m_pfInputBuffer1;
    float* m_pfOutputBuffer1;
    float* m_pfLatency;
    fft_vars* fmembvars; // member variables for fft routine

    unsigned long fs; // Sample rate

    unsigned long cbsize;   // size of circular buffer
    unsigned long corrsize; // cbsize/2 + 1
    unsigned long cbiwr;
    unsigned long cbord;
    float* cbi; // circular input buffer
    float* cbf; // circular formant correction buffer
    float* cbo; // circular output buffer

    float* cbwindow;   // hann of length N/2, zeros for the rest
    float* acwinv;     // inverse of autocorrelation of window
    float* hannwindow; // length-N hann
    int noverlap;

    float* ffttime;
    float* fftfreqre;
    float* fftfreqim;

    // VARIABLES FOR LOW-RATE SECTION
    float aref;     // A tuning reference (Hz)
    float inpitch;  // Input pitch (semitones)
    float conf;     // Confidence of pitch period estimate (between 0 and 1)
    float outpitch; // Output pitch (semitones)
    float vthresh;  // Voiced speech threshold

    float pmax;         // Maximum allowable pitch period (seconds)
    float pmin;         // Minimum allowable pitch period (seconds)
    unsigned long nmax; // Maximum period index for pitch prd est
    unsigned long nmin; // Minimum period index for pitch prd est

    float lrshift;  // Shift prescribed by low-rate section
    int ptarget;    // Pitch target, between 0 and 11
    float sptarget; // Smoothed pitch target

    float lfophase;

    // VARIABLES FOR PITCH SHIFTER
    float phprdd;     // default (unvoiced) phase period
    double inphinc;   // input phase increment
    double outphinc;  // input phase increment
    double phincfact; // factor determining output phase increment
    double phasein;
    double phaseout;
    float* frag;            // windowed fragment of speech
    unsigned long fragsize; // size of fragment in samples

    // VARIABLES FOR FORMANT CORRECTOR
    int ford;
    float falph;
    float flamb;
    float* fk;
    float* fb;
    float* fc;
    float* frb;
    float* frc;
    float* fsig;
    float* fsmooth;
    float fhp;
    float flp;
    float flpa;
    float** fbuff;
    float* ftvec;
    float fmute;
    float fmutealph;

} Autotalent;

/********************
 *  THE CONSTRUCTOR *
 ********************/

static LV2_Handle instantiate(const LV2_Descriptor* descriptor, double rate,
                              const char* bundle_path, const LV2_Feature* const* features)
{
    unsigned long ti;

    Autotalent* membvars = malloc(sizeof(Autotalent));

    membvars->aref = 440;

    membvars->fs = rate;

    if(rate >= 88200)
    {
        membvars->cbsize = 4096;
    }
    else
    {
        membvars->cbsize = 2048;
    }
    membvars->corrsize = membvars->cbsize / 2 + 1;

    membvars->pmax = 1 / (float)70;  // max and min periods (ms)
    membvars->pmin = 1 / (float)700; // eventually may want to bring these out as sliders

    membvars->nmax = (unsigned long)(rate * membvars->pmax);
    if(membvars->nmax > membvars->corrsize)
    {
        membvars->nmax = membvars->corrsize;
    }
    membvars->nmin = (unsigned long)(rate * membvars->pmin);

    membvars->cbi = calloc(membvars->cbsize, sizeof(float));
    membvars->cbf = calloc(membvars->cbsize, sizeof(float));
    membvars->cbo = calloc(membvars->cbsize, sizeof(float));

    membvars->cbiwr = 0;
    membvars->cbord = 0;

    membvars->lfophase = 0;

    // Initialize formant corrector
    membvars->ford = 7; // should be sufficient to capture formants
    membvars->falph = pow(0.001, (float)80 / (rate));
    membvars->flamb = -(0.8517 * sqrt(atan(0.06583 * rate)) - 0.1916); // or about -0.88 @ 44.1kHz
    membvars->fk = calloc(membvars->ford, sizeof(float));
    membvars->fb = calloc(membvars->ford, sizeof(float));
    membvars->fc = calloc(membvars->ford, sizeof(float));
    membvars->frb = calloc(membvars->ford, sizeof(float));
    membvars->frc = calloc(membvars->ford, sizeof(float));
    membvars->fsig = calloc(membvars->ford, sizeof(float));
    membvars->fsmooth = calloc(membvars->ford, sizeof(float));
    membvars->fhp = 0;
    membvars->flp = 0;
    membvars->flpa = pow(0.001, (float)10 / (rate));
    membvars->fbuff = (float**)malloc((membvars->ford) * sizeof(float*));
    for(ti = 0; ti < membvars->ford; ti++)
    {
        membvars->fbuff[ti] = calloc(membvars->cbsize, sizeof(float));
    }
    membvars->ftvec = calloc(membvars->ford, sizeof(float));
    membvars->fmute = 1;
    membvars->fmutealph = pow(0.001, (float)1 / (rate));

    // Standard raised cosine window, max height at N/2
    membvars->hannwindow = calloc(membvars->cbsize, sizeof(float));
    for(ti = 0; ti < membvars->cbsize; ti++)
    {
        membvars->hannwindow[ti] = -0.5 * cos(2 * PI * ti / membvars->cbsize) + 0.5;
    }

    // Generate a window with a single raised cosine from N/4 to 3N/4
    membvars->cbwindow = calloc(membvars->cbsize, sizeof(float));
    for(ti = 0; ti < (membvars->cbsize / 2); ti++)
    {
        membvars->cbwindow[ti + membvars->cbsize / 4] = -0.5 * cos(4 * PI * ti / (membvars->cbsize - 1)) + 0.5;
    }

    membvars->noverlap = 4;

    membvars->fmembvars = fft_con(membvars->cbsize);

    membvars->ffttime = calloc(membvars->cbsize, sizeof(float));
    membvars->fftfreqre = calloc(membvars->corrsize, sizeof(float));
    membvars->fftfreqim = calloc(membvars->corrsize, sizeof(float));

    // ---- Calculate autocorrelation of window ----
    membvars->acwinv = calloc(membvars->cbsize, sizeof(float));
    for(ti = 0; ti < membvars->cbsize; ti++)
    {
        membvars->ffttime[ti] = membvars->cbwindow[ti];
    }
    fft_forward(membvars->fmembvars, membvars->cbwindow, membvars->fftfreqre, membvars->fftfreqim);
    for(ti = 0; ti < membvars->corrsize; ti++)
    {
        membvars->fftfreqre[ti] = (membvars->fftfreqre[ti]) * (membvars->fftfreqre[ti]) +
                                  (membvars->fftfreqim[ti]) * (membvars->fftfreqim[ti]);
        membvars->fftfreqim[ti] = 0;
    }
    fft_inverse(membvars->fmembvars, membvars->fftfreqre, membvars->fftfreqim, membvars->ffttime);
    for(ti = 1; ti < membvars->cbsize; ti++)
    {
        membvars->acwinv[ti] = membvars->ffttime[ti] / membvars->ffttime[0];
        if(membvars->acwinv[ti] > 0.000001)
        {
            membvars->acwinv[ti] = (float)1 / membvars->acwinv[ti];
        }
        else
        {
            membvars->acwinv[ti] = 0;
        }
    }
    membvars->acwinv[0] = 1;
    // ---- END Calculate autocorrelation of window ----

    membvars->lrshift = 0;
    membvars->ptarget = 0;
    membvars->sptarget = 0;

    membvars->vthresh = 0.7; //  The voiced confidence (unbiased peak) threshold level

    // Pitch shifter initialization
    membvars->phprdd = 0.01; // Default period
    membvars->inphinc = (float)1 / (membvars->phprdd * rate);
    membvars->phincfact = 1;
    membvars->phasein = 0;
    membvars->phaseout = 0;
    membvars->frag = calloc(membvars->cbsize, sizeof(float));
    membvars->fragsize = 0;

    return membvars;
}

//  Connect port
static void connect_port(LV2_Handle instance, uint32_t port, void* data)
{
    Autotalent* psAutotalent;
    net_log_info("port:%d data:%p\n", port, data);
    psAutotalent = (Autotalent*)instance;
    switch(port)
    {
    case AT_TUNE:
        psAutotalent->m_pfTune = data;
        break;
    case AT_FIXED:
        psAutotalent->m_pfFixed = data;
        break;
    case AT_PULL:
        psAutotalent->m_pfPull = data;
        break;
    case AT_A:
        psAutotalent->m_pfA = data;
        break;
    case AT_Bb:
        psAutotalent->m_pfBb = data;
        break;
    case AT_B:
        psAutotalent->m_pfB = data;
        break;
    case AT_C:
        psAutotalent->m_pfC = data;
        break;
    case AT_Db:
        psAutotalent->m_pfDb = data;
        break;
    case AT_D:
        psAutotalent->m_pfD = data;
        break;
    case AT_Eb:
        psAutotalent->m_pfEb = data;
        break;
    case AT_E:
        psAutotalent->m_pfE = data;
        break;
    case AT_F:
        psAutotalent->m_pfF = data;
        break;
    case AT_Gb:
        psAutotalent->m_pfGb = data;
        break;
    case AT_G:
        psAutotalent->m_pfG = data;
        break;
    case AT_Ab:
        psAutotalent->m_pfAb = data;
        break;
    case AT_AMOUNT:
        psAutotalent->m_pfAmount = data;
        break;
    case AT_SMOOTH:
        psAutotalent->m_pfSmooth = data;
        break;
    case AT_SHIFT:
        psAutotalent->m_pfShift = data;
        break;
    case AT_SCWARP:
        psAutotalent->m_pfScwarp = data;
        break;
    case AT_LFOAMP:
        psAutotalent->m_pfLfoamp = data;
        break;
    case AT_LFORATE:
        psAutotalent->m_pfLforate = data;
        break;
    case AT_LFOSHAPE:
        psAutotalent->m_pfLfoshape = data;
        break;
    case AT_LFOSYMM:
        psAutotalent->m_pfLfosymm = data;
        break;
    case AT_LFOQUANT:
        psAutotalent->m_pfLfoquant = data;
        break;
    case AT_FCORR:
        psAutotalent->m_pfFcorr = data;
        break;
    case AT_FWARP:
        psAutotalent->m_pfFwarp = data;
        break;
    case AT_MIX:
        psAutotalent->m_pfMix = data;
        break;
    case AT_PITCH:
        psAutotalent->m_pfPitch = data;
        break;
    case AT_CONF:
        psAutotalent->m_pfConf = data;
        break;
    case AT_INPUT1:
        psAutotalent->m_pfInputBuffer1 = data;
        break;
    case AT_OUTPUT1:
        psAutotalent->m_pfOutputBuffer1 = data;
        break;
    case AT_LATENCY:
        psAutotalent->m_pfLatency = data;
        if (psAutotalent->m_pfLatency)
        {
            *(psAutotalent->m_pfLatency) = psAutotalent->cbsize - 1;
        }
        break;
    }
}

// Called every time we get a new chunk of audio
static void run(LV2_Handle instance, uint32_t n_samples)
{
    float* pfInput;
    float* pfOutput;
    float fAmount;
    float fSmooth;
    int iNotes[12];
    int iPitch2Note[12];
    int iNote2Pitch[12];
    int numNotes;
    float fTune;
    float fFixed;
    float fPull;
    float fShift;
    int iScwarp;
    float fLfoamp;
    float fLforate;
    float fLfoshape;
    float fLfosymm;
    int iLfoquant;
    int iFcorr;
    float fFwarp;
    float fMix;
    Autotalent* psAutotalent;
    unsigned long lSampleIndex;

    long int N;
    long int Nf;
    long int fs;
    float pmin;
    float pmax;
    unsigned long nmin;
    unsigned long nmax;

    long int ti;
    long int ti2;
    long int ti3;
    long int ti4;
    float tf;
    float tf2;

    // Variables for cubic spline interpolator
    float indd;
    int ind0;
    int ind1;
    int ind2;
    int ind3;
    float vald;
    float val0;
    float val1;
    float val2;
    float val3;

    int lowersnap;
    int uppersnap;
    float lfoval;

    float pperiod;
    float inpitch;
    float conf;
    float outpitch;
    float aref;
    float fa;
    float fb;
    float fc;
    float fk;
    float flamb;
    float frlamb;
    float falph;
    float foma;
    float f1resp;
    float f0resp;
    float flpa;
    int ford;
    psAutotalent = (Autotalent*)instance;

    pfInput = psAutotalent->m_pfInputBuffer1;
    pfOutput = psAutotalent->m_pfOutputBuffer1;
    fAmount = (float)*(psAutotalent->m_pfAmount);
    fSmooth = (float)*(psAutotalent->m_pfSmooth) * 0.8; // Scales max to a more reasonable value
    fTune = (float)*(psAutotalent->m_pfTune);
    iNotes[0] = (int)*(psAutotalent->m_pfA);
    iNotes[1] = (int)*(psAutotalent->m_pfBb);
    iNotes[2] = (int)*(psAutotalent->m_pfB);
    iNotes[3] = (int)*(psAutotalent->m_pfC);
    iNotes[4] = (int)*(psAutotalent->m_pfDb);
    iNotes[5] = (int)*(psAutotalent->m_pfD);
    iNotes[6] = (int)*(psAutotalent->m_pfEb);
    iNotes[7] = (int)*(psAutotalent->m_pfE);
    iNotes[8] = (int)*(psAutotalent->m_pfF);
    iNotes[9] = (int)*(psAutotalent->m_pfGb);
    iNotes[10] = (int)*(psAutotalent->m_pfG);
    iNotes[11] = (int)*(psAutotalent->m_pfAb);
    fFixed = (float)*(psAutotalent->m_pfFixed);
    fPull = (float)*(psAutotalent->m_pfPull);
    fShift = (float)*(psAutotalent->m_pfShift);
    iScwarp = (int)*(psAutotalent->m_pfScwarp);
    fLfoamp = (float)*(psAutotalent->m_pfLfoamp);
    fLforate = (float)*(psAutotalent->m_pfLforate);
    fLfoshape = (float)*(psAutotalent->m_pfLfoshape);
    fLfosymm = (float)*(psAutotalent->m_pfLfosymm);
    iLfoquant = (int)*(psAutotalent->m_pfLfoquant);
    iFcorr = (int)*(psAutotalent->m_pfFcorr);
    fFwarp = (float)*(psAutotalent->m_pfFwarp);
    fMix = (float)*(psAutotalent->m_pfMix);

    // Some logic for the semitone->scale and scale->semitone conversion
    // If no notes are selected as being in the scale, instead snap to all notes
    ti2 = 0;
    for(ti = 0; ti < 12; ti++)
    {
        if(iNotes[ti] >= 0)
        {
            iPitch2Note[ti] = ti2;
            iNote2Pitch[ti2] = ti;
            ti2 = ti2 + 1;
        }
        else
        {
            iPitch2Note[ti] = -1;
        }
    }
    numNotes = ti2;
    while(ti2 < 12)
    {
        iNote2Pitch[ti2] = -1;
        ti2 = ti2 + 1;
    }
    if(numNotes == 0)
    {
        for(ti = 0; ti < 12; ti++)
        {
            iNotes[ti] = 1;
            iPitch2Note[ti] = ti;
            iNote2Pitch[ti] = ti;
        }
        numNotes = 12;
    }
    iScwarp = (iScwarp + numNotes * 5) % numNotes;

    ford = psAutotalent->ford;
    falph = psAutotalent->falph;
    foma = (float)1 - falph;
    flpa = psAutotalent->flpa;
    flamb = psAutotalent->flamb;
    tf = pow((float)2, fFwarp / 2) * (1 + flamb) / (1 - flamb);
    frlamb = (tf - 1) / (tf + 1);

    psAutotalent->aref = (float)fTune;

    N = psAutotalent->cbsize;
    Nf = psAutotalent->corrsize;
    fs = psAutotalent->fs;

    pmax = psAutotalent->pmax;
    pmin = psAutotalent->pmin;
    nmax = psAutotalent->nmax;
    nmin = psAutotalent->nmin;

    aref = psAutotalent->aref;
    pperiod = psAutotalent->pmax;
    inpitch = psAutotalent->inpitch;
    conf = psAutotalent->conf;
    outpitch = psAutotalent->outpitch;

    /*******************
     *  MAIN DSP LOOP  *
     *******************/
    for(lSampleIndex = 0; lSampleIndex < n_samples; lSampleIndex++)
    {

        // load data into circular buffer
        tf = (float)*(pfInput++);
        ti4 = psAutotalent->cbiwr;
        psAutotalent->cbi[ti4] = tf;

        if(iFcorr >= 1)
        {
            // Somewhat experimental formant corrector
            //  formants are removed using an adaptive pre-filter and
            //  re-introduced after pitch manipulation using post-filter
            // tf is signal input
            fa = tf - psAutotalent->fhp; // highpass pre-emphasis filter
            psAutotalent->fhp = tf;
            fb = fa;
            for(ti = 0; ti < ford; ti++)
            {
                psAutotalent->fsig[ti] = fa * fa * foma + psAutotalent->fsig[ti] * falph;
                fc = (fb - psAutotalent->fc[ti]) * flamb + psAutotalent->fb[ti];
                psAutotalent->fc[ti] = fc;
                psAutotalent->fb[ti] = fb;
                fk = fa * fc * foma + psAutotalent->fk[ti] * falph;
                psAutotalent->fk[ti] = fk;
                tf = fk / (psAutotalent->fsig[ti] + 0.000001);
                tf = tf * foma + psAutotalent->fsmooth[ti] * falph;
                psAutotalent->fsmooth[ti] = tf;
                psAutotalent->fbuff[ti][ti4] = tf;
                fb = fc - tf * fa;
                fa = fa - tf * fc;
            }
            psAutotalent->cbf[ti4] = fa;
            // Now hopefully the formants are reduced
            // More formant correction code at the end of the DSP loop
        }
        else
        {
            psAutotalent->cbf[ti4] = tf;
        }

        // Input write pointer logic
        psAutotalent->cbiwr++;
        if(psAutotalent->cbiwr >= N)
        {
            psAutotalent->cbiwr = 0;
        }

        // ********************
        // * Low-rate section *
        // ********************

        // Every N/noverlap samples, run pitch estimation / manipulation code
        if((psAutotalent->cbiwr) % (N / psAutotalent->noverlap) == 0)
        {

            // ---- Obtain autocovariance ----

            // Window and fill FFT buffer
            ti2 = psAutotalent->cbiwr;
            for(ti = 0; ti < N; ti++)
            {
                psAutotalent->ffttime[ti] = (float)(psAutotalent->cbi[(ti2 - ti + N) % N] * psAutotalent->cbwindow[ti]);
            }

            // Calculate FFT
            fft_forward(
                psAutotalent->fmembvars, psAutotalent->ffttime, psAutotalent->fftfreqre, psAutotalent->fftfreqim);

            // Remove DC
            psAutotalent->fftfreqre[0] = 0;
            psAutotalent->fftfreqim[0] = 0;

            // Take magnitude squared
            for(ti = 1; ti < Nf; ti++)
            {
                psAutotalent->fftfreqre[ti] = (psAutotalent->fftfreqre[ti]) * (psAutotalent->fftfreqre[ti]) +
                                              (psAutotalent->fftfreqim[ti]) * (psAutotalent->fftfreqim[ti]);
                psAutotalent->fftfreqim[ti] = 0;
            }

            // Calculate IFFT
            fft_inverse(
                psAutotalent->fmembvars, psAutotalent->fftfreqre, psAutotalent->fftfreqim, psAutotalent->ffttime);

            // Normalize
            tf = (float)1 / psAutotalent->ffttime[0];
            for(ti = 1; ti < N; ti++)
            {
                psAutotalent->ffttime[ti] = psAutotalent->ffttime[ti] * tf;
            }
            psAutotalent->ffttime[0] = 1;

            //  ---- END Obtain autocovariance ----

            //  ---- Calculate pitch and confidence ----

            // Calculate pitch period
            //   Pitch period is determined by the location of the max (biased)
            //     peak within a given range
            //   Confidence is determined by the corresponding unbiased height
            tf2 = 0;
            pperiod = pmin;
            for(ti = nmin; ti < nmax; ti++)
            {
                ti2 = ti - 1;
                ti3 = ti + 1;
                if(ti2 < 0)
                {
                    ti2 = 0;
                }
                if(ti3 > Nf)
                {
                    ti3 = Nf;
                }
                tf = psAutotalent->ffttime[ti];

                if(tf > psAutotalent->ffttime[ti2] && tf >= psAutotalent->ffttime[ti3] && tf > tf2)
                {
                    tf2 = tf;
                    ti4 = ti;
                }
            }
            if(tf2 > 0)
            {
                conf = tf2 * psAutotalent->acwinv[ti4];
                if(ti4 > 0 && ti4 < Nf)
                {
                    // Find the center of mass in the vicinity of the detected peak
                    tf = psAutotalent->ffttime[ti4 - 1] * (ti4 - 1);
                    tf = tf + psAutotalent->ffttime[ti4] * (ti4);
                    tf = tf + psAutotalent->ffttime[ti4 + 1] * (ti4 + 1);
                    tf = tf /
                         (psAutotalent->ffttime[ti4 - 1] + psAutotalent->ffttime[ti4] + psAutotalent->ffttime[ti4 + 1]);
                    pperiod = tf / fs;
                }
                else
                {
                    pperiod = (float)ti4 / fs;
                }
            }

            // Convert to semitones
            tf = (float)-12 * log10((float)aref * pperiod) * L2SC;
            if(conf >= psAutotalent->vthresh)
            {
                inpitch = tf;
                psAutotalent->inpitch = tf; // update pitch only if voiced
            }
            psAutotalent->conf = conf;

            *(psAutotalent->m_pfPitch) = (float)inpitch;
            *(psAutotalent->m_pfConf) = (float)conf;

            //  ---- END Calculate pitch and confidence ----

            //  ---- Modify pitch in all kinds of ways! ----

            outpitch = inpitch;

            // Pull to fixed pitch
            outpitch = (1 - fPull) * outpitch + fPull * fFixed;

            // -- Convert from semitones to scale notes --
            ti = (int)(outpitch / 12 + 32) - 32; // octave
            tf = outpitch - ti * 12;             // semitone in octave
            ti2 = (int)tf;
            ti3 = ti2 + 1;
            // a little bit of pitch correction logic, since it's a convenient place for it
            if(iNotes[ti2 % 12] < 0 || iNotes[ti3 % 12] < 0)   // if between 2 notes that are more than a semitone apart
            {
                lowersnap = 1;
                uppersnap = 1;
            }
            else
            {
                lowersnap = 0;
                uppersnap = 0;
                if(iNotes[ti2 % 12] == 1)   // if specified by user
                {
                    lowersnap = 1;
                }
                if(iNotes[ti3 % 12] == 1)   // if specified by user
                {
                    uppersnap = 1;
                }
            }
            // (back to the semitone->scale conversion)
            // finding next lower pitch in scale
            while(iNotes[(ti2 + 12) % 12] < 0)
            {
                ti2 = ti2 - 1;
            }
            // finding next higher pitch in scale
            while(iNotes[ti3 % 12] < 0)
            {
                ti3 = ti3 + 1;
            }
            tf = (tf - ti2) / (ti3 - ti2) + iPitch2Note[(ti2 + 12) % 12];
            if(ti2 < 0)
            {
                tf = tf - numNotes;
            }
            outpitch = tf + numNotes * ti;
            // -- Done converting to scale notes --

            // The actual pitch correction
            ti = (int)(outpitch + 128) - 128;
            tf = outpitch - ti - 0.5;
            ti2 = ti3 - ti2;
            if(ti2 > 2)   // if more than 2 semitones apart, put a 2-semitone-like transition halfway between
            {
                tf2 = (float)ti2 / 2;
            }
            else
            {
                tf2 = (float)1;
            }
            if(fSmooth < 0.001)
            {
                tf2 = tf * tf2 / 0.001;
            }
            else
            {
                tf2 = tf * tf2 / fSmooth;
            }
            if(tf2 < -0.5)
                tf2 = -0.5;
            if(tf2 > 0.5)
                tf2 = 0.5;
            tf2 = 0.5 * sin(PI * tf2) + 0.5; // jumping between notes using horizontally-scaled sine segment
            tf2 = tf2 + ti;
            if((tf < 0.5 && lowersnap) || (tf >= 0.5 && uppersnap))
            {
                outpitch = fAmount * tf2 + ((float)1 - fAmount) * outpitch;
            }

            // Add in pitch shift
            outpitch = outpitch + fShift;

            // LFO logic
            tf = fLforate * N / (psAutotalent->noverlap * fs);
            if(tf > 1)
                tf = 1;
            psAutotalent->lfophase = psAutotalent->lfophase + tf;
            if(psAutotalent->lfophase > 1)
                psAutotalent->lfophase = psAutotalent->lfophase - 1;
            lfoval = psAutotalent->lfophase;
            tf = (fLfosymm + 1) / 2;
            if(tf <= 0 || tf >= 1)
            {
                if(tf <= 0)
                    lfoval = 1 - lfoval;
            }
            else
            {
                if(lfoval <= tf)
                {
                    lfoval = lfoval / tf;
                }
                else
                {
                    lfoval = 1 - (lfoval - tf) / (1 - tf);
                }
            }
            if(fLfoshape >= 0)
            {
                // linear combination of cos and line
                lfoval = (0.5 - 0.5 * cos(lfoval * PI)) * fLfoshape + lfoval * (1 - fLfoshape);
                lfoval = fLfoamp * (lfoval * 2 - 1);
            }
            else
            {
                // smoosh the sine horizontally until it's squarish
                tf = 1 + fLfoshape;
                if(tf < 0.001)
                {
                    lfoval = (lfoval - 0.5) * 2 / 0.001;
                }
                else
                {
                    lfoval = (lfoval - 0.5) * 2 / tf;
                }
                if(lfoval > 1)
                    lfoval = 1;
                if(lfoval < -1)
                    lfoval = -1;
                lfoval = fLfoamp * sin(lfoval * PI * 0.5);
            }
            // add in quantized LFO
            if(iLfoquant >= 1)
            {
                outpitch = outpitch + (int)(numNotes * lfoval + numNotes + 0.5) - numNotes;
            }

            // Convert back from scale notes to semitones
            outpitch = outpitch + iScwarp; // output scale rotate implemented here
            ti = (int)(outpitch / numNotes + 32) - 32;
            tf = outpitch - ti * numNotes;
            ti2 = (int)tf;
            ti3 = ti2 + 1;
            outpitch = iNote2Pitch[ti3 % numNotes] - iNote2Pitch[ti2];
            if(ti3 >= numNotes)
            {
                outpitch = outpitch + 12;
            }
            outpitch = outpitch * (tf - ti2) + iNote2Pitch[ti2];
            outpitch = outpitch + 12 * ti;
            outpitch = outpitch - (iNote2Pitch[iScwarp] - iNote2Pitch[0]); // more scale rotation here

            // add in unquantized LFO
            if(iLfoquant <= 0)
            {
                outpitch = outpitch + lfoval * 2;
            }

            if(outpitch < -36)
                outpitch = -48;
            if(outpitch > 24)
                outpitch = 24;

            psAutotalent->outpitch = outpitch;

            //  ---- END Modify pitch in all kinds of ways! ----

            // Compute variables for pitch shifter that depend on pitch
            psAutotalent->inphinc = aref * pow(2, inpitch / 12) / fs;
            psAutotalent->outphinc = aref * pow(2, outpitch / 12) / fs;
            psAutotalent->phincfact = psAutotalent->outphinc / psAutotalent->inphinc;
        }
        // ************************
        // * END Low-Rate Section *
        // ************************

        // *****************
        // * Pitch Shifter *
        // *****************

        // Pitch shifter (kind of like a pitch-synchronous version of Fairbanks' technique)
        //   Note: pitch estimate is naturally N/2 samples old
        psAutotalent->phasein = psAutotalent->phasein + psAutotalent->inphinc;
        psAutotalent->phaseout = psAutotalent->phaseout + psAutotalent->outphinc;

        //   When input phase resets, take a snippet from N/2 samples in the past
        if(psAutotalent->phasein >= 1)
        {
            psAutotalent->phasein = psAutotalent->phasein - 1;
            ti2 = psAutotalent->cbiwr - N / 2;
            for(ti = -N / 2; ti < N / 2; ti++)
            {
                psAutotalent->frag[(ti + N) % N] = psAutotalent->cbf[(ti + ti2 + N) % N];
            }
        }

        //   When output phase resets, put a snippet N/2 samples in the future
        if(psAutotalent->phaseout >= 1)
        {
            psAutotalent->fragsize = psAutotalent->fragsize * 2;
            if(psAutotalent->fragsize > N)
            {
                psAutotalent->fragsize = N;
            }
            psAutotalent->phaseout = psAutotalent->phaseout - 1;
            ti2 = psAutotalent->cbord + N / 2;
            ti3 = (long int)(((float)psAutotalent->fragsize) / psAutotalent->phincfact);
            if(ti3 >= N / 2)
            {
                ti3 = N / 2 - 1;
            }
            for(ti = -ti3 / 2; ti < (ti3 / 2); ti++)
            {
                tf = psAutotalent->hannwindow[(long int)N / 2 + ti * (long int)N / ti3];
                // 3rd degree polynomial interpolator - based on eqns from Hal Chamberlin's book
                indd = psAutotalent->phincfact * ti;
                ind1 = (int)indd;
                ind2 = ind1 + 1;
                ind3 = ind1 + 2;
                ind0 = ind1 - 1;
                val0 = psAutotalent->frag[(ind0 + N) % N];
                val1 = psAutotalent->frag[(ind1 + N) % N];
                val2 = psAutotalent->frag[(ind2 + N) % N];
                val3 = psAutotalent->frag[(ind3 + N) % N];
                vald = 0;
                vald = vald - (float)0.166666666667 * val0 * (indd - ind1) * (indd - ind2) * (indd - ind3);
                vald = vald + (float)0.5 * val1 * (indd - ind0) * (indd - ind2) * (indd - ind3);
                vald = vald - (float)0.5 * val2 * (indd - ind0) * (indd - ind1) * (indd - ind3);
                vald = vald + (float)0.166666666667 * val3 * (indd - ind0) * (indd - ind1) * (indd - ind2);
                psAutotalent->cbo[(ti + ti2 + N) % N] = psAutotalent->cbo[(ti + ti2 + N) % N] + vald * tf;
            }
            psAutotalent->fragsize = 0;
        }
        psAutotalent->fragsize++;

        //   Get output signal from buffer
        tf = psAutotalent->cbo[psAutotalent->cbord]; // read buffer

        psAutotalent->cbo[psAutotalent->cbord] = 0; // erase for next cycle
        psAutotalent->cbord++;                      // increment read pointer
        if(psAutotalent->cbord >= N)
        {
            psAutotalent->cbord = 0;
        }

        // *********************
        // * END Pitch Shifter *
        // *********************

        ti4 = (psAutotalent->cbiwr + 2) % N;
        if(iFcorr >= 1)
        {
            // The second part of the formant corrector
            // This is a post-filter that re-applies the formants, designed
            //   to result in the exact original signal when no pitch
            //   manipulation is performed.
            // tf is signal input
            // gotta run it 3 times because of a pesky delay free loop
            //  first time: compute 0-response
            tf2 = tf;
            fa = 0;
            fb = fa;
            for(ti = 0; ti < ford; ti++)
            {
                fc = (fb - psAutotalent->frc[ti]) * frlamb + psAutotalent->frb[ti];
                tf = psAutotalent->fbuff[ti][ti4];
                fb = fc - tf * fa;
                psAutotalent->ftvec[ti] = tf * fc;
                fa = fa - psAutotalent->ftvec[ti];
            }
            tf = -fa;
            for(ti = ford - 1; ti >= 0; ti--)
            {
                tf = tf + psAutotalent->ftvec[ti];
            }
            f0resp = tf;
            //  second time: compute 1-response
            fa = 1;
            fb = fa;
            for(ti = 0; ti < ford; ti++)
            {
                fc = (fb - psAutotalent->frc[ti]) * frlamb + psAutotalent->frb[ti];
                tf = psAutotalent->fbuff[ti][ti4];
                fb = fc - tf * fa;
                psAutotalent->ftvec[ti] = tf * fc;
                fa = fa - psAutotalent->ftvec[ti];
            }
            tf = -fa;
            for(ti = ford - 1; ti >= 0; ti--)
            {
                tf = tf + psAutotalent->ftvec[ti];
            }
            f1resp = tf;
            //  now solve equations for output, based on 0-response and 1-response
            tf = (float)2 * tf2;
            tf2 = tf;
            tf = ((float)1 - f1resp + f0resp);
            if(tf != 0)
            {
                tf2 = (tf2 + f0resp) / tf;
            }
            else
            {
                tf2 = 0;
            }
            //  third time: update delay registers
            fa = tf2;
            fb = fa;
            for(ti = 0; ti < ford; ti++)
            {
                fc = (fb - psAutotalent->frc[ti]) * frlamb + psAutotalent->frb[ti];
                psAutotalent->frc[ti] = fc;
                psAutotalent->frb[ti] = fb;
                tf = psAutotalent->fbuff[ti][ti4];
                fb = fc - tf * fa;
                fa = fa - tf * fc;
            }
            tf = tf2;
            tf = tf + flpa * psAutotalent->flp; // lowpass post-emphasis filter
            psAutotalent->flp = tf;
            // Bring up the gain slowly when formant correction goes from disabled
            // to enabled, while things stabilize.
            if(psAutotalent->fmute > 0.5)
            {
                tf = tf * (psAutotalent->fmute - 0.5) * 2;
            }
            else
            {
                tf = 0;
            }
            tf2 = psAutotalent->fmutealph;
            psAutotalent->fmute = (1 - tf2) + tf2 * psAutotalent->fmute;
            // now tf is signal output
            // ...and we're done messing with formants
        }
        else
        {
            psAutotalent->fmute = 0;
        }

        // Write audio to output of plugin
        // Mix (blend between original (delayed) =0 and processed =1)
        *(pfOutput++) = (float)fMix * tf + (1 - fMix) * psAutotalent->cbi[ti4];
    }

    // Tell the host the algorithm latency
    *(psAutotalent->m_pfLatency) = (float)(N - 1);
}

static void activate(LV2_Handle instance)
{
}

static void deactivate(LV2_Handle instance)
{
}

/********************
 *  THE DESTRUCTOR! *
 ********************/
static void cleanup(LV2_Handle instance)
{
    int ti;
    fft_des(((Autotalent*)instance)->fmembvars);
    free(((Autotalent*)instance)->cbi);
    free(((Autotalent*)instance)->cbf);
    free(((Autotalent*)instance)->cbo);
    free(((Autotalent*)instance)->cbwindow);
    free(((Autotalent*)instance)->hannwindow);
    free(((Autotalent*)instance)->acwinv);
    free(((Autotalent*)instance)->frag);
    free(((Autotalent*)instance)->ffttime);
    free(((Autotalent*)instance)->fftfreqre);
    free(((Autotalent*)instance)->fftfreqim);
    free(((Autotalent*)instance)->fk);
    free(((Autotalent*)instance)->fb);
    free(((Autotalent*)instance)->fc);
    free(((Autotalent*)instance)->frb);
    free(((Autotalent*)instance)->frc);
    free(((Autotalent*)instance)->fsmooth);
    free(((Autotalent*)instance)->fsig);
    for(ti = 0; ti < ((Autotalent*)instance)->ford; ti++)
    {
        free(((Autotalent*)instance)->fbuff[ti]);
    }
    free(((Autotalent*)instance)->fbuff);
    free(((Autotalent*)instance)->ftvec);
    free((Autotalent*)instance);
}

static const void* extension_data(const char* uri)
{
    return NULL;
}

static const LV2_Descriptor descriptor = {"urn:jeremy.salwen:plugins:autotalent",
                                          instantiate,
                                          connect_port,
                                          activate,
                                          run,
                                          deactivate,
                                          cleanup,
                                          extension_data
                                         };



// Return the plugin descriptor (there's only one in this file)
LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    return index == 0 ? &descriptor : NULL;
}


// All done
