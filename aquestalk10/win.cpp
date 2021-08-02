#include "win.h"

AquesTalk10::AquesTalk10(const std::string libraryPath)
{
    HMODULE handler = LoadLibrary(libraryPath.c_str());
    if (handler == NULL) {
        throw std::exception("failed load library");
    }
	FARPROC AquesTalk_Synthe = GetProcAddress(handler, "AquesTalk_Synthe");
	FARPROC AquesTalk_Synthe_Utf8 = GetProcAddress(handler, "AquesTalk_Synthe_Utf8");
	FARPROC AquesTalk_Synthe_Utf16 = GetProcAddress(handler, "AquesTalk_Synthe_Utf16");
	FARPROC AquesTalk_FreeWave = GetProcAddress(handler, "AquesTalk_FreeWave");
	FARPROC AquesTalk_SetDevKey = GetProcAddress(handler, "AquesTalk_SetDevKey");
	FARPROC AquesTalk_SetUsrKey = GetProcAddress(handler, "AquesTalk_SetUsrKey");
	if (
		AquesTalk_Synthe == NULL ||
		AquesTalk_Synthe_Utf8 == NULL ||
		AquesTalk_Synthe_Utf16 == NULL ||
		AquesTalk_FreeWave == NULL ||
		AquesTalk_SetDevKey == NULL ||
		AquesTalk_SetUsrKey == NULL
	) {
		throw std::exception("loading library is succeeded, but can't found needed functions");
	}
	m_handler = handler;
}

AquesTalk10::~AquesTalk10()
{
	FreeLibrary(m_handler);
}

unsigned char *AquesTalk10::AquesTalk_Synthe_Sjis(const AQTK_VOICE* pParam, const char* koe, int* pSize)
{
	SYNTHE Synthe = (SYNTHE)GetProcAddress(m_handler, "AquesTalk_Synthe");
	return Synthe(pParam, koe, pSize);
}

unsigned char *AquesTalk10::AquesTalk_Synthe_Utf8(const AQTK_VOICE* pParam, const char* koe, int* pSize)
{
	SYNTHE Synthe = (SYNTHE)GetProcAddress(m_handler, "AquesTalk_Synthe_Utf8");
	return Synthe(pParam, koe, pSize);
}

unsigned char *AquesTalk10::AquesTalk_Synthe_Utf16(const AQTK_VOICE* pParam, const unsigned short* koe, int* pSize)
{
	SYNTHE_UTF16 Synthe = (SYNTHE_UTF16)GetProcAddress(m_handler, "AquesTalk_Synthe_Utf16");
	return Synthe(pParam, koe, pSize);
}

void AquesTalk10::AquesTalk_FreeWave(unsigned char* wav)
{
	FREEWAVE Freewave = (FREEWAVE)GetProcAddress(m_handler, "AquesTalk_FreeWave");
	return Freewave(wav);
}

int AquesTalk10::AquesTalk_SetDevKey(const char* key)
{
	SETKEY SetKey = (SETKEY)GetProcAddress(m_handler, "AquesTalk_SetDevKey");
	return SetKey(key);
}

int AquesTalk10::AquesTalk_SetUsrKey(const char* key)
{
	SETKEY SetKey = (SETKEY)GetProcAddress(m_handler, "AquesTalk_SetUsrKey");
	return SetKey(key);
}
