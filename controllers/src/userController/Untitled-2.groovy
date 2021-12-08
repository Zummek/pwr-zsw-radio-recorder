src\audioController\Recorder.cpp: In function 'bool Recorder::startRecording(File&)':
src\audioController\Recorder.cpp:101:26: warning: passing 'volatile WavFile' as 'this' argument discards qualifiers [-fpermissive]
     if (outFile.isOpened()) return false;
                          ^
In file included from src\audioController\Recorder.cpp:2:0:
include/WavFile.hpp:22:10: note:   in call to 'bool WavFile::isOpened()'
     bool isOpened();
          ^~~~~~~~
src\audioController\Recorder.cpp:108:48: warning: passing 'volatile WavFile' as 'this' argument discards qualifiers [-fpermissive]
     outFile = WavFile(outputFile, outSampleRate);
                                                ^
In file included from src\audioController\Recorder.cpp:2:0:
include/WavFile.hpp:9:7: note:   in call to 'WavFile& WavFile::operator=(WavFile&&)'
 class WavFile
       ^~~~~~~
src\audioController\Recorder.cpp:109:19: warning: passing 'volatile WavFile' as 'this' argument discards qualifiers [-fpermissive]
     outFile.begin();
                   ^
In file included from src\audioController\Recorder.cpp:2:0:
include/WavFile.hpp:21:10: note:   in call to 'void WavFile::begin()'
     void begin();
          ^~~~~
src\audioController\Recorder.cpp: In function 'void Recorder::stopRecording()':
src\audioController\Recorder.cpp:118:27: warning: passing 'volatile WavFile' as 'this' argument discards qualifiers [-fpermissive]
     if (!outFile.isOpened()) return;
                           ^
In file included from src\audioController\Recorder.cpp:2:0:
include/WavFile.hpp:22:10: note:   in call to 'bool WavFile::isOpened()'
     bool isOpened();
          ^~~~~~~~
src\audioController\Recorder.cpp:121:19: warning: passing 'volatile WavFile' as 'this' argument discards qualifiers [-fpermissive]
     outFile.close();
                   ^
In file included from src\audioController\Recorder.cpp:2:0:
include/WavFile.hpp:24:10: note:   in call to 'void WavFile::close()'
     void close();