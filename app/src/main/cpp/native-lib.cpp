#include <jni.h>
#include <string>
#include "client/crashpad_client.h"
#include "client/crash_report_database.h"
#include "client/settings.h"
#include "base/logging.h"
#include <android/log.h>
#include <unistd.h>

using namespace base;
using namespace crashpad;
using namespace std;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_androidcrasher_MainActivity_initializeCrashpad(
        JNIEnv* env,
        jobject /* this */) {

    FilePath handler("/data/data/com.example.androidcrasher/lib/libcrashpad_handler.so");
    FilePath reportsDir("/data/data/com.example.androidcrasher/crashpad");
    FilePath metricsDir("/data/data/com.example.androidcrasher/crashpad");

    string url = "http://fred.bugsplat.com/post/bp/crash/crashpad.php";

    map<string, string> annotations;
    annotations["format"] = "minidump";           // Required: Crashpad setting to save crash as a minidump
    annotations["database"] = "fred";             // Required: BugSplat appName
    annotations["product"] = "AndroidCrasher"; // Required: BugSplat appName
    annotations["version"] = "1.0.0";             // Required: BugSplat appVersion
    annotations["key"] = "Samplekey";            // Optional: BugSplat key field
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
    static CrashpadClient *client = new CrashpadClient();
    bool status = client->StartHandlerAtCrash(handler, reportsDir, metricsDir, url, annotations, arguments);

    return status;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_androidcrasher_MainActivity_crash(
        JNIEnv* env,
        jobject /* this */) {

    *(volatile int *)0 = 0;

    return true;
}

