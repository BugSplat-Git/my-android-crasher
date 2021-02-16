#include <jni.h>
#include <string>
#include <unistd.h>
#include "client/crashpad_client.h"
#include "client/crash_report_database.h"
#include "client/settings.h"

void stackFrame1(void);
void stackFrame2(void);
void stackFrame3(void);
void crash(void);

using namespace base;
using namespace crashpad;
using namespace std;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_androidcrasher_MainActivity_initializeCrashpad(
        JNIEnv* env,
        jobject /* this */,
        jstring appDataDir,
        jstring libDir
    ) {

    // Device file paths
    string nativeLibraryDir = env->GetStringUTFChars(libDir, 0);
    string dataDir = env->GetStringUTFChars(appDataDir, 0);

    // Crashpad file paths
    FilePath handler(nativeLibraryDir + "/libcrashpad_handler.so");
    FilePath reportsDir(dataDir + "/crashpad");
    FilePath metricsDir(dataDir + "/crashpad");

    // Crashpad upload URL for BugSplat database
    string url = "http://fred.bugsplat.com/post/bp/crash/crashpad.php";

    // Crashpad annotations
    map<string, string> annotations;
    annotations["format"] = "minidump";           // Required: Crashpad setting to save crash as a minidump
    annotations["database"] = "fred";             // Required: BugSplat appName
    annotations["product"] = "AndroidCrasher"; // Required: BugSplat appName
    annotations["version"] = "1.0.1";             // Required: BugSplat appVersion
    annotations["key"] = "Samplekey";            // Optional: BugSplat key field
    annotations["user"] = "fred@bugsplat.com";    // Optional: BugSplat user email
    annotations["list_annotations"] = "Sample comment"; // Optional: BugSplat crash description

    // Crashpad arguments
    vector<string> arguments;
    arguments.push_back("--no-rate-limit");

    // Crashpad local database
    unique_ptr<CrashReportDatabase> crashReportDatabase = CrashReportDatabase::Initialize(reportsDir);
    if (crashReportDatabase == NULL) return false;

    // Enable automated crash uploads
    Settings *settings = crashReportDatabase->GetSettings();
    if (settings == NULL) return false;
    settings->SetUploadsEnabled(true);

    // File paths of attachments to be uploaded with the minidump file at crash time - default bundle limit is 2MB
    vector<FilePath> attachments;
    FilePath attachment(dataDir + "/files/attachment.txt");
    attachments.push_back(attachment);

    // Start Crashpad crash handler
    static CrashpadClient *client = new CrashpadClient();
    bool status = client->StartHandlerAtCrash(handler, reportsDir, metricsDir, url, annotations, arguments, attachments);
    return status;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_androidcrasher_MainActivity_crash(
        JNIEnv* env,
        jobject /* this */) {

    stackFrame1();

    return true;
}

void stackFrame1() {
    stackFrame2();
}

void stackFrame2() {
    stackFrame3();
}

void stackFrame3() {
    crash();
}

void crash() {
    *(volatile int *)0 = 0;
}
