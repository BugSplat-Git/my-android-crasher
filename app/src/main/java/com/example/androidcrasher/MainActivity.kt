package com.example.androidcrasher

import android.R.attr.data
import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import kotlinx.android.synthetic.main.activity_main.*
import java.io.IOException
import java.io.OutputStreamWriter


class MainActivity : AppCompatActivity() {

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        writeLogFile()
        sample_text.text = if (initializeCrashpad()) "initialized" else "fail"
    }

    public fun btnCrashClick(view: View) {
        // Example of a call to a native method
        crash()
    }

    private fun writeLogFile() {
        try {
            val outputStreamWriter = OutputStreamWriter(
                applicationContext.openFileOutput(
                    "attachment.txt",
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
    external fun initializeCrashpad(): Boolean
    external fun crash(): Boolean

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
