#ifndef AQUESTALK10_H
#define AQUESTALK10_H

#include <napi.h>

#include "aquestalk10/common.h"
#ifdef _WIN32 || _WIN64
#include "aquestalk10/win.h"
#else
#include "aquestalk10/linux.h"
#endif

class AquesTalk10Wrapper : public Napi::ObjectWrap<AquesTalk10Wrapper> {
public:
    static Napi::Object Init(Napi::Env env, Napi::Object exports);
    static Napi::Object NewInstance(Napi::Env env, const Napi::CallbackInfo& info);

    AquesTalk10Wrapper(const Napi::CallbackInfo& info);
    ~AquesTalk10Wrapper();

    Napi::Value AquesTalkSyntheSjis(const Napi::CallbackInfo& info);
    Napi::Value AquesTalkSyntheUtf8(const Napi::CallbackInfo& info);
    Napi::Value AquesTalkSyntheUtf16(const Napi::CallbackInfo& info);

    Napi::Value AquesTalkSetDevKey(const Napi::CallbackInfo& info);
    Napi::Value AquesTalkSetUsrKey(const Napi::CallbackInfo& info);

private:
    AQTK_VOICE* CreateVoiceParamsStructAndValidateArgs(const Napi::CallbackInfo& info);
    bool CheckError(const Napi::Env env, unsigned char* wav, int size);

    AquesTalk10* m_aquestalk10;
};

#endif // AQUESTALK10_H

