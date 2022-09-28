#include "EVE_Filesystem.h"

#define DEBUG_SERIAL Serial // TODO: Declare this in a variants file
 
bool initSDCard() {
    #ifdef SDCARD_DEBUG
    DEBUG_SERIAL.print("Initializing filesystem...");
    #endif
    bool _success = SD.begin();
    #ifdef SDCARD_DEBUG
    DEBUG_SERIAL.println(_success ? "done!" : "Failed to initialize filesystem!");
    #endif
    return _success;
}

void listDir(fs::FS &fs, const char * dirname, uint8_t levels) {
    DEBUG_SERIAL.printf("Listing directory: %s\n\r", dirname);
    File root = fs.open(dirname);
    if (!root) {
        DEBUG_SERIAL.println("Failed to open directory");
        return;
    }
    if (!root.isDirectory()) {
        DEBUG_SERIAL.println("Not a directory");
        return;
    }

    File file = root.openNextFile();
    while (file) {
        if (file.isDirectory()) {
            DEBUG_SERIAL.print("  DIR : ");
            DEBUG_SERIAL.println(file.name());
            if (levels){
                listDir(fs, file.path(), levels--);
            }
        } 
        else {
            DEBUG_SERIAL.print("  FILE: ");
            DEBUG_SERIAL.print(file.name());
            DEBUG_SERIAL.print("  SIZE: ");
            DEBUG_SERIAL.println(file.size());
        }
        file = root.openNextFile();
    }
}

bool createDir(fs::FS &fs, const char * path) {
    DEBUG_SERIAL.printf("Creating Dir: %s ...", path);
    bool _success = fs.mkdir(path);
    DEBUG_SERIAL.println(_success ? "done!" : "couldn't create directory!");
    return _success;
}

bool removeDir(fs::FS &fs, const char * path) {
    DEBUG_SERIAL.printf("Removing Dir: %s ", path);
    bool _success = fs.rmdir(path);
    DEBUG_SERIAL.println(_success ? "done!" : "couldn't remove directory!");
    return _success;
}

bool readFile(fs::FS &fs, const char * path) {
    DEBUG_SERIAL.printf("Reading file: %s\r\n", path);

    File file = fs.open(path);
    if (!file) {
        DEBUG_SERIAL.println("Failed to open file for reading");
        return false;
    }

    DEBUG_SERIAL.print("Read from file: ");
    while (file.available()) {
        DEBUG_SERIAL.write(file.read());
    }
    file.close();
    return true;
}

bool writeFile(fs::FS &fs, const char * path, const char * message) {
    #ifdef SDCARD_DEBUG
    DEBUG_SERIAL.printf("Writing file: %s\n\r", path);
    #endif
    File file = fs.open(path, FILE_WRITE);
    if (!file) {
        #ifdef SDCARD_DEBUG
        DEBUG_SERIAL.println("Failed to open file for writing");
        #endif
        return false;
    }

    bool _success = file.print(message);
    #ifdef SDCARD_DEBUG
    DEBUG_SERIAL.println(_success ? "Message written!" : "Failed to write message");
    #endif
    file.close();
    return _success;
}

bool appendFile(fs::FS &fs, const char * path, const char * message) {
    #ifdef SDCARD_DEBUG
    DEBUG_SERIAL.printf("Appending file: %s\n\r", path);
    #endif

    File file = fs.open(path, FILE_APPEND);
    if (!file) {
        #ifdef SDCARD_DEBUG
        DEBUG_SERIAL.println("Failed to open file for appending");
        #endif
        return false;
    }

    bool _success = file.print(message);
    #ifdef SDCARD_DEBUG
    DEBUG_SERIAL.println(_success ? "Message appended!" : "Failed to append message");
    #endif
    file.close();
    return _success;
}

bool renameFile(fs::FS &fs, const char * path1, const char * path2) {
    DEBUG_SERIAL.printf("Renaming file %s to %s\n\r", path1, path2);
    bool _success = fs.rename(path1, path2);
    DEBUG_SERIAL.println(_success ? "File renamed" : "Failed to rename file");
    return _success;
}

bool deleteFile(fs::FS &fs, const char * path) {
    DEBUG_SERIAL.printf("Deleting file: %s\n\r", path);
    bool _success = fs.remove(path);
    DEBUG_SERIAL.println(_success ? "File deleted" : "Failed to delete file");
    return _success;
}

void testFileIO(fs::FS &fs, const char * path) {
    File file = fs.open(path);
    static uint8_t buf[512];
    size_t len = 0;
    uint32_t start = millis();
    uint32_t end = start;
    if (file) {
        len = file.size();
        size_t flen = len;
        start = millis();
        while (len) {
            size_t toRead = len;
            if (toRead > 512) {
                toRead = 512;
            }
            file.read(buf, toRead);
            len -= toRead;
        }
        end = millis() - start;
        DEBUG_SERIAL.printf("%u bytes read for %u ms\n\r", flen, end);
        file.close();
    } 
    else {
        DEBUG_SERIAL.println("Failed to open file for reading");
    }

    file = fs.open(path, FILE_WRITE);
    if (!file) {
        DEBUG_SERIAL.println("Failed to open file for writing");
        return;
    }

    size_t i;
    start = millis();
    for (i=0; i<2048; i++) {
        file.write(buf, 512);
    }
    end = millis() - start;
    DEBUG_SERIAL.printf("%u bytes written for %u ms\n\r", 2048 * 512, end);
    file.close();
}

bool initLogFile(fs::FS &fs, char * path, char * header) {
    for (uint8_t x=0; x<255; x++) { // Initialize log file
        sprintf(path, "/log_%03d.csv", x);
        if (!fs.exists(path)) break; // If a new unique log file has been named, exit loop
        if (x==254) return false; // If no unique log could be created, return an error
    }
    if (!fs.open(path, FILE_WRITE)) {
        #ifdef SDCARD_DEBUG
        DEBUG_SERIAL.println("Unable to open file for writing");
        #endif
        return false; // If unable to open the new log file, return an error
    }
    #ifdef SDCARD_DEBUG
    DEBUG_SERIAL.printf("Logging to: %s", path);
    #endif

    // Write header for the log file
    writeFile(fs, path, header);
    return true;
}