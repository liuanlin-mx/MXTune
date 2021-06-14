#include <stdio.h>
#include <assert.h>
#include "lv2.h"
#include "mat_helper.h"
#include <thread>

LV2_SYMBOL_EXPORT
const LV2_Descriptor* lv2_descriptor(uint32_t index);

int main(int argc, char **argv)
{
    //main2(argc, argv);
#ifdef MAT_STORAGE_OS_WINDOWS
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
#endif
    const LV2_Descriptor *desc = lv2_descriptor(0);
    LV2_Handle instance = desc->instantiate(desc, 44100, NULL, NULL);
    
    float cfg[32] = {
        440,    //AT_TUNE
        0,      //AT_FIXED
        0,      //AT_PULL
        1,      //AT_A
        -1,      //AT_Bb
        1,      //AT_B:
        1,      //AT_C:
        -1,      //AT_Db:
        1,      //AT_D:
        -1,      //AT_Eb:
        1,      //AT_E:
        1,      //AT_F:
        -1,      //AT_Gb:
        1,      //AT_G:
        -1,      //AT_Ab:
        1,      //AT_AMOUNT:
        1,      //AT_SMOOTH:
        0,      //AT_SHIFT:
        0,      //AT_SCWARP:
        0,      //AT_LFOAMP:
        5,      //AT_LFORATE:
        0,      //AT_LFOSHAPE:
        0,      //AT_LFOSYMM:
        0,      //AT_LFOQUANT:
        0,      //AT_FCORR:
        0,      //AT_FWARP:
        1,      //AT_MIX:
        0,      //AT_PITCH:
        0,      //AT_CONF:
        0,      //AT_INPUT1:29
        0,      //AT_OUTPUT1:30
        0,      //AT_LATENCY:
    };
    
    
    for (int i = 0; i < sizeof(cfg)/ sizeof(cfg[0]); ++i)
    {
        desc->connect_port(instance, i, (void *)&cfg[i]);
    }
    
#define N (4096)

    static float input[N];
    static float output[N];
    
    FILE *ifp = fopen("fy.raw", "rb");
    assert(ifp);
    FILE *ofp = fopen("fy.out", "wb");
    assert(ofp);
    
    for (int i = 0; i < 100000; i++)
    {
        int rlen = fread(input, 1, sizeof(input), ifp);
        if (rlen < sizeof(input))
        {
            break;
        }
        
        mat_helper_write_mat2_4("input", 1, N, "float", input);
        desc->connect_port(instance, 29, input);
        desc->connect_port(instance, 30, output);
        desc->run(instance, N);
        mat_helper_write_mat2_4("output", 1, N, "float", output);
        fwrite(output, 1, rlen, ofp);
        //break;
    }
    
    fclose(ifp);
    fclose(ofp);
    printf("hello xxx\n");
    getchar();
    return 0;
}