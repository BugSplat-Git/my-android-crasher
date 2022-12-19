#include <jni.h>
#include <string>
#include <unistd.h>
#include "client/crashpad_client.h"
#include "client/crash_report_database.h"
#include "client/settings.h"

// Uncomment and replace value with your BugSplat database name
// #define BUGSPLAT_DATABASE "your-database-here"

#ifndef BUGSPLAT_DATABASE
#error "Please define your BugSplat database above"
#endif

void stackFrame1();
void stackFrame2();
void stackFrame3();
void crash();

using namespace base;
using namespace crashpad;
using namespace std;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_bugsplat_my_1android_1crasher_MainActivity_initializeCrashpad(
        JNIEnv* env,
        jobject /* this */,
        jstring appDataDir,
        jstring libDir
    ) {
    // Device file paths
    string nativeLibraryDir = env->GetStringUTFChars(libDir, nullptr);
    string dataDir = env->GetStringUTFChars(appDataDir, nullptr);

    // Crashpad file paths
    FilePath handler(nativeLibraryDir + "/libcrashpad_handler.so");
    FilePath reportsDir(dataDir + "/crashpad");
    FilePath metricsDir(dataDir + "/crashpad");

    // Crashpad upload URL for BugSplat database
    string database = BUGSPLAT_DATABASE;
    string url = "https://" + database + ".bugsplat.com/post/bp/crash/crashpad.php";

    // Crashpad annotations
    map<string, string> annotations;
    annotations["format"] = "minidump";             // Required: Crashpad setting to save crash as a minidump
    annotations["database"] = database;             // Required: BugSplat database
    annotations["product"] = "my-android-crasher";  // Required: BugSplat appName
    annotations["version"] = "1.0.1";               // Required: BugSplat appVersion
    annotations["key"] = "key!";                    // Optional: BugSplat key field
    annotations["user"] = "fred@bugsplat.com";      // Optional: BugSplat user email
    annotations["list_annotations"] = "Sample comment"; // Optional: BugSplat crash description

    // Crashpad arguments
    vector<string> arguments;
    arguments.emplace_back("--no-rate-limit");

    // Crashpad local database
    unique_ptr<CrashReportDatabase> crashReportDatabase = CrashReportDatabase::Initialize(reportsDir);
    if (crashReportDatabase == nullptr) return false;

    // Enable automated crash uploads
    Settings *settings = crashReportDatabase->GetSettings();
    if (settings == nullptr) return false;
    settings->SetUploadsEnabled(true);

    // File paths of attachments to be uploaded with the minidump file at crash time - default bundle limit is 20MB
    vector<FilePath> attachments;
    FilePath attachment(dataDir + "/files/attachment.txt");
    attachments.push_back(attachment);

    // Start Crashpad crash handler
    auto *client = new CrashpadClient();
    bool status = client->StartHandlerAtCrash(
        handler,
        reportsDir,
        metricsDir,
        url,
        annotations,
        arguments,
        attachments
    );
    return status;
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_bugsplat_my_1android_1crasher_MainActivity_crash(
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
    *(volatile int *)nullptr = 0;
}
