package com.example.my_android_crasher

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.example.my_android_crasher.databinding.ActivityMainBinding
import com.bugsplat.android.BugSplatBridge;
import java.io.IOException
import java.io.OutputStreamWriter

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        val view = binding.root
        setContentView(view)

        val attributes = mapOf(
            "key1" to "value1",
            "key2" to "value2",
            "environment" to "development"
        )

        val attachmentFileName = "log.txt"
        createAttachmentFile(attachmentFileName)
        val attachmentPath = applicationContext.getFileStreamPath(attachmentFileName).absolutePath
        val attachments = arrayOf(attachmentPath)

        BugSplatBridge.initBugSplat(this, "fred", "my-android-crasher", "2.0.0", attributes, attachments)

        binding.sampleText.text = "initialized"
    }

    fun btnCrashClick(view: View) {
        // Example of a call to a native method
        crash()
    }

    private fun createAttachmentFile(fileName: String) {
        try {
            val outputStreamWriter = OutputStreamWriter(
                applicationContext.openFileOutput(
                    fileName,
                    Context.MODE_PRIVATE
                )
            )
            outputStreamWriter.write("BugSplat rocks!")
            outputStreamWriter.close()
        } catch (e: IOException) {
            Log.e("Exception", "File write failed: " + e.toString())
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    external fun crash(): Boolean

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("crash-lib")
        }
    }
}
