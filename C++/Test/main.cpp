
#include "mysharemem.h"
#include "memcontent.h"
#include <assert.h>
#include <iostream>
#include <string.h>

#ifdef WIN32
#include<windows.h>
#include "sapi.h"
#include <sphelper.h>
#pragma comment(lib,"ole32.lib")
#pragma comment(lib,"sapi.lib")
#else
#include <unistd.h>
#endif

int main(int argc, char *argv[])
{
    if (FAILED(::CoInitialize(NULL)))
    {
        return 0;
    }
    ISpVoice * pVoice = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SpVoice, NULL, CLSCTX_ALL,IID_ISpVoice, (void **)&pVoice);
    if (SUCCEEDED(hr))
    {
        CComPtr<ISpStream>cpWavStream;
        CComPtr<ISpStreamFormat>cpOldStream;
        CSpStreamFormat originalFmt;
        pVoice ->GetOutputStream(&cpOldStream);
        originalFmt.AssignFormat(cpOldStream);
        hr = SPBindToFile(L"C:\\Users\\h158837\\Desktop\\output.wav", SPFM_CREATE_ALWAYS,
            &cpWavStream, &originalFmt.FormatId(),
            originalFmt.WaveFormatExPtr());
        if (SUCCEEDED(hr))
        {
            pVoice ->SetOutput(cpWavStream, TRUE);
            WCHAR WTX[] = L"＜VOICE REQUIRED=''NAME=Microsoft Mary''/＞我赵星海";
            pVoice ->Speak(WTX, SPF_IS_XML, NULL);
            pVoice ->Release();
            pVoice = NULL;
        }
    }
    ::CoUninitialize();

    return 0;
}
