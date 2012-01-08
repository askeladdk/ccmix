/* 
 * File:   main.cpp
 * Author: ivosh-l
 *
 * Created on 29. prosinec 2011, 11:30
 */

//#include <boost/filesystem.hpp>
//#include <boost/system/system_error.hpp>
#include <cstdlib>
#include "mix_file.h"
#include <iostream>
#include <string.h>

using namespace std;
//using namespace boost::filesystem;

#ifdef WINDOWS
    #define DIR_SEPARATOR '\\'
#else
    #define DIR_SEPARATOR '/'
 #endif


static const char*
getCurrentDir(const char* const program_location) {
    register int i;
    const char* const loc = program_location;

    size_t size = strlen(loc);

    for (i = (size - 1); (i >= 0) && (loc[i] != DIR_SEPARATOR); --i)
        ;

    if (loc[i] == DIR_SEPARATOR) {
        char* curdir = (char*) malloc((i + 1) * sizeof (char));
        if (curdir != NULL) {
            strncpy(curdir, loc, (size_t) i);
            curdir[i] = '\0';
            return curdir;
        }
    }

    const char* curdir = ".";
    return curdir;
}

int main(int argc, char** argv) {
    MixFile mix(getCurrentDir(argv[0]));
    string oPath;
    string iFile;
    if (argc < 2) {
        cout << "Usage \"tsunmix <filename.mix> [list,extract,extid,getid] {file1 {-o file1out} {file2 {-o file2out}} ...}\"." << endl;
        return 0;
    }
    if (!mix.open(argv[1])) {
        cout << "File \"" << argv[1] << "\" not found!" << endl;
        return 1;
    }
    if (argc < 3) {
        cout << mix.printFileList(0);
    } else {
        if (!strcmp(argv[2], "list")) {
            cout << mix.printFileList(1);
        } else if (!strcmp(argv[2], "extract") || !strcmp(argv[2], "extid") || !strcmp(argv[2], "extid16")) {
            if (!strcmp(argv[2], "extid16")) {
                cout << "Warning: option \"extid16\" is deprecated. Use \"extid\" insted, \"0x\" id prefix for hex format." << endl;
            }
            if (argc < 4) {
                if (!strcmp(argv[2], "extract"))
                    cout << "Usage \"tsunmix <filename.mix> extract <file>\"." << endl;
                else if (!strcmp(argv[2], "extid"))
                    cout << "Usage \"tsunmix <filename.mix> extid <crc id>\". Use \"0x\" id prefix for hex format." << endl;
                else if (!strcmp(argv[2], "extid16"))
                    cout << "Usage \"tsunmix <filename.mix> extid16 <hex crc id>\"." << endl;
            } else {

                for (int i = 3; i < argc; i++) {
                    oPath.clear();
                    iFile = argv[i];
                    if (argc > (i + 2)) {
                        if (!strcmp(argv[i + 1], "-o")) {
                            oPath = argv[i + 2];
                        }
                    }
                    if (oPath.empty())
                        oPath = argv[i];
                    else
                        i += 2;
                    if (!strcmp(argv[2], "extid")) {
                        if (iFile.length() > 1 && iFile[0] == '0' && iFile[1] == 'x') {
                            if (!mix.extractFile(strtol(iFile.c_str(), NULL, 16), oPath.c_str())) {
                                cout << "File \"" << iFile.c_str() << "\" not found in the archive." << endl;
                            }
                        } else if (!mix.extractFile(strtol(iFile.c_str(), NULL, 10), oPath.c_str())) {
                            cout << "File \"" << iFile.c_str() << "\" not found in the archive." << endl;
                        }
                    } else if (!strcmp(argv[2], "extid16")) {
                        if (!mix.extractFile(strtol(iFile.c_str(), NULL, 16), oPath.c_str())) {
                            cout << "File \"" << iFile.c_str() << "\" not found in the archive." << endl;
                        }
                    } else {
                        if (!mix.extractFile(iFile.c_str(), oPath.c_str())) {
                            cout << "File \"" << iFile.c_str() << "\" not found in the archive." << endl;
                        }
                    }
                }
            }
        } else if (!strcmp(argv[2], "getid")) {
            if (argc < 4) {
                cout << "Usage \"tsunmix <filename.mix> getid <file>\"." << endl;

            } else {
                cout << argv[3] << " : " << hex << mix.getID(game_ts, argv[3]) << ((mix.checkFileName(argv[3])) ? "" : " (NOT PRESENT)") << endl;
            }
        } else if (!strcmp(argv[2], "all")) {
            if (argc > 4) {
                if (!strcmp(argv[3], "-o")) {
                    oPath = argv[4];
                } else
                    oPath = ".";
            } else
                oPath = ".";
            //create_directory((const path)argv[0]);
            if (argc > 5) {
                if (!strcmp(argv[5], "-f")) {
                    mix.extractAll(oPath, false);
                } else
                    mix.extractAll(oPath);
            } else
                mix.extractAll(oPath);
        } else if (!strcmp(argv[2], "decrypt")) {
            if (argc > 4) {
                if (!strcmp(argv[3], "-o")) {
                    oPath = argv[4];
                } else
                    oPath = string(argv[1]) + "_decoded.mix";
            } else
                    oPath = string(argv[1]) + "_decoded.mix";
            //create_directory((const path)argv[0]);
            if(mix.decrypt(oPath))
            cout << "File " << oPath << " created successfuly." << endl;
        }
    }


    return 0;
}
