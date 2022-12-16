wsl mkdir -p ./tmp/x86_64
wsl ./linux/dump_syms ../app/build/intermediates/merged_native_libs/debug/out/lib/x86_64/libnative-lib.so > ./tmp/x86_64/libnative-lib.so.sym

$FilePath = ".\tmp\x86_64\libnative-lib.so.sym";
$URL = "https://fred.bugsplat.com/post/bp/symbol/breakpadsymbols.php?appName=my-android-crasher&appVer=1.0.1&code_file=libnative-lib.so.sym";
$fileBytes = [System.IO.File]::ReadAllBytes($FilePath);
$fileEnc = [System.Text.Encoding]::GetEncoding('UTF-8').GetString($fileBytes);
$boundary = [System.Guid]::NewGuid().ToString(); 
$LF = "`r`n";
$bodyLines = ( 
    "--$boundary",
    "Content-Disposition: form-data; name=`"symbol_file`"; filename=`"libnative-lib.so.sym`"",
    "Content-Type: application/octet-stream$LF",
    $fileEnc,
    "--$boundary--$LF" 
) -join $LF

Invoke-RestMethod -Uri $URL -Method Post -ContentType "multipart/form-data; boundary=`"$boundary`"" -Body $bodyLines