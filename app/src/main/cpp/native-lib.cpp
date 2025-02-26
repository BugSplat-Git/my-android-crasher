#include <jni.h>
#include <string>
#include <unistd.h>

void stackFrame1();
void stackFrame2();
void stackFrame3();
void crash();

using namespace std;

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_my_1android_1crasher_MainActivity_crash(
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
