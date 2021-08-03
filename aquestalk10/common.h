#ifndef COMMON_H
#define COMMON_H

typedef struct _AQTK_VOICE_ {
    int bas;
    int spd;
    int vol;
    int pit;
    int acc;
    int lmd;
    int fsc;
} AQTK_VOICE;

typedef unsigned char* (*SYNTHE)(const AQTK_VOICE* pParam, const char* koe, int* pSize);
typedef unsigned char* (*SYNTHE_UTF16)(const AQTK_VOICE* pParam, const unsigned short* koe, int* pSize);

typedef void (*FREEWAVE)(unsigned char* wav);
typedef int (*SETKEY)(const char* key);

#endif // COMMON_H
