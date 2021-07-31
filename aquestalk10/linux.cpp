#include <dlfcn.h>

#include "linux.h"

AquesTalk10::AquesTalk10(const std::string libraryPath)
{
    void *handler = dlopen(libraryPath.c_str(), RTLD_LAZY);
    if (handler == NULL) {
        throw "failed load library";
    }
	void *AquesTalk_Synthe = dlsym(handler, "AquesTalk_Synthe");
	void *AquesTalk_Synthe_Utf8 = dlsym(handler, "AquesTalk_Synthe_Utf8");
	void *AquesTalk_Synthe_Utf16 = dlsym(handler, "AquesTalk_Synthe_Utf16");
	void *AquesTalk_FreeWave = dlsym(handler, "AquesTalk_FreeWave");
	void *AquesTalk_SetDevKey = dlsym(handler, "AquesTalk_SetDevKey");
	void *AquesTalk_SetUsrKey = dlsym(handler, "AquesTalk_SetUsrKey");
	if (
		AquesTalk_Synthe == NULL ||
		AquesTalk_Synthe_Utf8 == NULL ||
		AquesTalk_Synthe_Utf16 == NULL ||
		AquesTalk_FreeWave == NULL ||
		AquesTalk_SetDevKey == NULL ||
		AquesTalk_SetUsrKey == NULL
	) {
		throw "loading library is succeeded, but can't found needed functions";
	}
	m_handler = handler;
}

AquesTalk10::~AquesTalk10()
{
	dlclose(m_handler);
}

unsigned char *AquesTalk10::AquesTalk_Synthe_Sjis(const AQTK_VOICE* pParam, const char* koe, int* pSize)
{
	SYNTHE Synthe = (SYNTHE)dlsym(m_handler, "AquesTalk_Synthe");
	return Synthe(pParam, koe, pSize);
}

unsigned char *AquesTalk10::AquesTalk_Synthe_Utf8(const AQTK_VOICE* pParam, const char* koe, int* pSize)
{
	SYNTHE Synthe = (SYNTHE)dlsym(m_handler, "AquesTalk_Synthe_Utf8");
	return Synthe(pParam, koe, pSize);
}

unsigned char *AquesTalk10::AquesTalk_Synthe_Utf16(const AQTK_VOICE* pParam, const unsigned short* koe, int* pSize)
{
	SYNTHE_UTF16 Synthe = (SYNTHE_UTF16)dlsym(m_handler, "AquesTalk_Synthe_Utf16");
	return Synthe(pParam, koe, pSize);
}

void AquesTalk10::AquesTalk_FreeWave(unsigned char* wav)
{
	FREEWAVE Freewave = (FREEWAVE)dlsym(m_handler, "AquesTalk_FreeWave");
	return Freewave(wav);
}

int AquesTalk10::AquesTalk_SetDevKey(const char* key)
{
	SETKEY SetKey = (SETKEY)dlsym(m_handler, "AquesTalk_SetDevKey");
	return SetKey(key);
}

int AquesTalk10::AquesTalk_SetUsrKey(const char* key)
{
	SETKEY SetKey = (SETKEY)dlsym(m_handler, "AquesTalk_SetUsrKey");
	return SetKey(key);
}
