#ifndef WIN_H
#define WIN_H

#include <napi.h>
#include <string>
#include <windows.h>

#include "common.h"

class AquesTalk10 {
public:
    AquesTalk10(const std::string libraryName);
    ~AquesTalk10();

    unsigned char *AquesTalk_Synthe_Sjis(const AQTK_VOICE* pParam, const char* koe, int* pSize);
    unsigned char *AquesTalk_Synthe_Utf8(const AQTK_VOICE* pParam, const char* koe, int* pSize);
    unsigned char *AquesTalk_Synthe_Utf16(const AQTK_VOICE* pParam, const unsigned short* koe, int* pSize);

    void AquesTalk_FreeWave(unsigned char* wav);

    int AquesTalk_SetDevKey(const char* key);
    int AquesTalk_SetUsrKey(const char* key);

private:
    HMODULE m_handler;
};

#endif // WIN_H
