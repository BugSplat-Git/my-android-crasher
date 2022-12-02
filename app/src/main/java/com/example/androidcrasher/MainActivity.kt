package com.example.androidcrasher

import android.content.Context
import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.example.androidcrasher.databinding.ActivityMainBinding
import java.io.IOException
import java.io.OutputStreamWriter

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        val view = binding.root
        setContentView(view)

        writeLogFile()

        val info = applicationInfo
        val initialized = initializeCrashpad(
            info.dataDir,
            info.nativeLibraryDir
        )
        val text = if(initialized) "initialized" else "fail"

        binding.sampleText.text = text;
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
    external fun initializeCrashpad(dataDir: String, nativeLibraryDir: String): Boolean
    external fun crash(): Boolean

    companion object {
        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("native-lib")
        }
    }
}
