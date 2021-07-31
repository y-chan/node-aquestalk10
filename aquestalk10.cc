#include <napi.h>
#include <string>

#include "aquestalk10.h"

using namespace Napi;

Napi::Object AquesTalk10Wrapper::NewInstance(Napi::Env env, const Napi::CallbackInfo& info)
{
    Napi::EscapableHandleScope scope(env);
    if (info.Length() < 1) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return Napi::Object::New(env);
    }

    const std::initializer_list<napi_value> initArgList = { info[0] };
    Napi::Object obj = env.GetInstanceData<Napi::FunctionReference>()->New(initArgList);
    return scope.Escape(napi_value(obj)).ToObject();
}

Napi::Object AquesTalk10Wrapper::Init(Napi::Env env, Napi::Object exports)
{
    Napi::Function func = DefineClass(
        env, "AquesTalk10Wrapper", {
            InstanceMethod("AquesTalkSyntheSjis", &AquesTalk10Wrapper::AquesTalkSyntheSjis),
            InstanceMethod("AquesTalkSyntheUtf8", &AquesTalk10Wrapper::AquesTalkSyntheUtf8),
            InstanceMethod("AquesTalkSyntheUtf16", &AquesTalk10Wrapper::AquesTalkSyntheUtf16),
            InstanceMethod("AquesTalkSetDevKey", &AquesTalk10Wrapper::AquesTalkSetDevKey),
            InstanceMethod("AquesTalkSetUsrKey", &AquesTalk10Wrapper::AquesTalkSetUsrKey),
        });

    Napi::FunctionReference* constructor = new Napi::FunctionReference();
    *constructor = Napi::Persistent(func);
    env.SetInstanceData(constructor);

    exports.Set("Wrapper", func);
    return exports;
}


AquesTalk10Wrapper::AquesTalk10Wrapper(const Napi::CallbackInfo& info)
    : Napi::ObjectWrap<AquesTalk10Wrapper>(info)
{
    std::string libraryName = info[0].As<Napi::String>().Utf8Value();
    try {
        m_aquestalk10 = new AquesTalk10(libraryName);
    } catch (char *err) {
        Napi::Error::New(info.Env(), err).ThrowAsJavaScriptException();
    }
};

AquesTalk10Wrapper::~AquesTalk10Wrapper()
{
    delete m_aquestalk10;
    m_aquestalk10 = nullptr;
};

AQTK_VOICE* AquesTalk10Wrapper::CreateVoiceParamsStructAndValidateArgs(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    if (info.Length() < 2) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return nullptr;
    }

    if (!info[0].IsObject() || !info[1].IsString()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return nullptr;
    }

    Napi::Object voiceParamsBase = info[0].As<Napi::Object>();
    if (
        !voiceParamsBase.Has("base") ||
        !voiceParamsBase.Has("speed") ||
        !voiceParamsBase.Has("volume") ||
        !voiceParamsBase.Has("pitch") ||
        !voiceParamsBase.Has("accent") ||
        !voiceParamsBase.Has("lmd") ||
        !voiceParamsBase.Has("fsc")
        ) {
        Napi::TypeError::New(env, "wrong object").ThrowAsJavaScriptException();
        return nullptr;
    }

    Napi::Value baseValue = voiceParamsBase.Get("base");
    Napi::Value speedValue = voiceParamsBase.Get("speed");
    Napi::Value volumeValue = voiceParamsBase.Get("volume");
    Napi::Value pitchValue = voiceParamsBase.Get("pitch");
    Napi::Value accentValue = voiceParamsBase.Get("accent");
    Napi::Value lmdValue = voiceParamsBase.Get("lmd");
    Napi::Value fscValue = voiceParamsBase.Get("fsc");

    if (
        !baseValue.IsNumber() ||
        !speedValue.IsNumber() ||
        !volumeValue.IsNumber() ||
        !pitchValue.IsNumber() ||
        !accentValue.IsNumber() ||
        !lmdValue.IsNumber() ||
        !fscValue.IsNumber()
        ) {
        Napi::TypeError::New(env, "wrong object params").ThrowAsJavaScriptException();
        return nullptr;
    }

    int base = (int)baseValue.As<Napi::Number>().Int32Value();
    int speed = (int)speedValue.As<Napi::Number>().Int32Value();
    int volume = (int)volumeValue.As<Napi::Number>().Int32Value();
    int pitch = (int)pitchValue.As<Napi::Number>().Int32Value();
    int accent = (int)accentValue.As<Napi::Number>().Int32Value();
    int lmd = (int)lmdValue.As<Napi::Number>().Int32Value();
    int fsc = (int)fscValue.As<Napi::Number>().Int32Value();

    if (
        base < 0 || 2 < base ||
        speed < 50 || 300 < speed ||
        volume < 0 || 300 < volume ||
        pitch < 20 || 200 < pitch ||
        accent < 0 || 200 < accent ||
        lmd < 0 || 200 < lmd ||
        fsc < 50 || 200 < fsc
        ) {
        Napi::RangeError::New(env, "object params out of range").ThrowAsJavaScriptException();
        return nullptr;
    }

    AQTK_VOICE* voiceParams;
    voiceParams = (AQTK_VOICE*)malloc(sizeof(AQTK_VOICE));
    voiceParams->bas = base;
    voiceParams->spd = speed;
    voiceParams->vol = volume;
    voiceParams->pit = pitch;
    voiceParams->acc = accent;
    voiceParams->lmd = lmd;
    voiceParams->fsc = fsc;
    return voiceParams;
}

bool AquesTalk10Wrapper::CheckError(const Napi::Env env, unsigned char* wav, int size)
{
    if (wav == 0) {
        char errorMsg[20];
        sprintf(errorMsg, "ERR: %d", size);
        Napi::Error::New(env, errorMsg).ThrowAsJavaScriptException();
        return true;
    }
    return false;
}

Napi::Value AquesTalk10Wrapper::AquesTalkSyntheSjis(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const AQTK_VOICE *voiceParams = CreateVoiceParamsStructAndValidateArgs(info);
    if (voiceParams == nullptr) {
        return env.Null();
    }
    int size;
    std::string koe = info[1].As<Napi::String>().Utf8Value();
    unsigned char *wav = m_aquestalk10->AquesTalk_Synthe_Sjis(voiceParams, koe.c_str(), &size);
    if (CheckError(env, wav, size)) {
        return env.Null();
    }

    Napi::Buffer<uint8_t> wavBuffer = Napi::Buffer<uint8_t>::Copy(env, wav, (size_t)size);
    m_aquestalk10->AquesTalk_FreeWave(wav);

    return wavBuffer;
}

Napi::Value AquesTalk10Wrapper::AquesTalkSyntheUtf8(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const AQTK_VOICE *voiceParams = CreateVoiceParamsStructAndValidateArgs(info);
    if (voiceParams == nullptr) {
        return env.Null();
    }
    int size;
    std::string koe = info[1].As<Napi::String>().Utf8Value();
    unsigned char *wav = m_aquestalk10->AquesTalk_Synthe_Utf8(voiceParams, koe.c_str(), &size);
    if (CheckError(env, wav, size)) {
        return env.Null();
    }
    Napi::Buffer<uint8_t> wavBuffer = Napi::Buffer<uint8_t>::Copy(env, wav, (size_t)size);
    m_aquestalk10->AquesTalk_FreeWave(wav);

    return wavBuffer;
}

Napi::Value AquesTalk10Wrapper::AquesTalkSyntheUtf16(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();
    const AQTK_VOICE *voiceParams = CreateVoiceParamsStructAndValidateArgs(info);
    if (voiceParams == nullptr) {
        return env.Null();
    }
    int size;
    std::u16string koe = info[1].As<Napi::String>().Utf16Value();
    unsigned char *wav = m_aquestalk10->AquesTalk_Synthe_Utf16(voiceParams, (uint16_t *)koe.c_str(), &size);
    if (CheckError(env, wav, size)) {
        return env.Null();
    }

    Napi::Buffer<uint8_t> wavBuffer = Napi::Buffer<uint8_t>::Copy(env, wav, (size_t)size);
    m_aquestalk10->AquesTalk_FreeWave(wav);

    return wavBuffer;
}

Napi::Value AquesTalk10Wrapper::AquesTalkSetDevKey(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    std::string key = info[0].As<Napi::String>().Utf8Value();
    int success = m_aquestalk10->AquesTalk_SetDevKey(key.c_str());
    return Napi::Boolean::New(env, !(bool)success);
}

Napi::Value AquesTalk10Wrapper::AquesTalkSetUsrKey(const Napi::CallbackInfo& info)
{
    Napi::Env env = info.Env();

    if (info.Length() < 1) {
        Napi::TypeError::New(env, "missing arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "wrong arguments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    std::string key = info[0].As<Napi::String>().Utf8Value();
    int success = m_aquestalk10->AquesTalk_SetUsrKey(key.c_str());
    return Napi::Boolean::New(env, !(bool)success);
}

Napi::Object CreateObject(const Napi::CallbackInfo& info) {
    return AquesTalk10Wrapper::NewInstance(info.Env(), info);
}

Napi::Object Initialize(Napi::Env env, Napi::Object exports) {
    Napi::Object new_exports = Napi::Function::New(env, CreateObject);
    return AquesTalk10Wrapper::Init(env, new_exports);
}

NODE_API_MODULE(aquestalk10, Initialize)

