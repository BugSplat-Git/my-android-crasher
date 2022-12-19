ABI=x86_64

mkdir -p ./tmp/$ABI

./linux/dump_syms ../app/build/intermediates/merged_native_libs/debug/out/lib/$ABI/libnative-lib.so > ./tmp/$ABI/libnative-lib.so.sym

./linux/symupload ./tmp/$ABI/libnative-lib.so.sym "https://fred.bugsplat.com/post/bp/symbol/breakpadsymbols.php?appName=my-android-crasher&appVer=1.0.1"
