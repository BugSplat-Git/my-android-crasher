#include <jni.h>
#include <string>
#include "client/crashpad_client.h"
#include "client/crash_report_database.h"
#include "client/settings.h"

std::string ConvertJString(JNIEnv* env, jstring str);

using namespace base;
using namespace crashpad;
using namespace std;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_androidcrasher_MainActivity_initializeCrashpad(
        JNIEnv* env,
        jobject /* this */) {

    // TODO BG finish implementing
//    string db = ConvertJString(env, database);
//    string appName = ConvertJString(env, application);
//    string appVer = ConvertJString(env, version);

    FilePath handler("/data/data/com.example.androidcrasher/lib/libcrashpad_handler.so");
    FilePath reportsDir("/data/data/com.example.androidcrasher/crashpad");
    FilePath metricsDir("/data/data/com.example.androidcrasher/crashpad");

    string url = "https://fred.bugsplat.com/post/bp/crash/crashpad.php";

    map<string, string> annotations;
    annotations["format"] = "minidump";           // Required: Crashpad setting to save crash as a minidump
    annotations["database"] = "fred";             // Required: BugSplat appName
    annotations["product"] = "AndroidCrasher"; // Required: BugSplat appName
    annotations["version"] = "1.0.0";             // Required: BugSplat appVersion
    annotations["key"] = "Sample key";            // Optional: BugSplat key field
    annotations["user"] = "fred@bugsplat.com";    // Optional: BugSplat user email
    annotations["list_annotations"] = "Sample comment"; // Optional: BugSplat crash description

    vector<string> arguments;
    arguments.push_back("--no-rate-limit");

    unique_ptr<CrashReportDatabase> crashReportDatabase = CrashReportDatabase::Initialize(reportsDir);
    if (crashReportDatabase == NULL) return false;

    // Enable automated crash uploads
    Settings *settings = crashReportDatabase->GetSettings();
    if (settings == NULL) return false;
    settings->SetUploadsEnabled(true);

    // Start crash handler
    CrashpadClient *client = new CrashpadClient();
    bool status = client->StartHandler(handler, reportsDir, metricsDir, url, annotations, arguments, true, false);
    return status;
}

std::string ConvertJString(JNIEnv* env, jstring str)
{
    if ( !str ) std::string();

    const jsize len = env->GetStringUTFLength(str);
    const char* strChars = env->GetStringUTFChars(str, (jboolean *)0);

    std::string Result(strChars, len);

    env->ReleaseStringUTFChars(str, strChars);

    return Result;
}
