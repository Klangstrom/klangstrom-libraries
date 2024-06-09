#include "Arduino.h"
#include "Klangstrom.h"
#include "KlangstromSerialDebug.h"
#include "KlangstromSDCard.h"

SerialDebug console;
Klangstrom  klangstrom;
SDCard      sdcard;

void setup() {
    /* init section */
    klangstrom.init();
    console.init();
    console.clear();
    console.info();
    console.timestamp();
    console.println("starting init");
    sdcard.init();

    console.timestamp();
    console.println("finished init");

    /* setup section */
    console.timestamp();
    console.println("starting setup");
    klangstrom.setup();

    console.timestamp();
    console.println("finished setup");
    console.println("---------------------------------------------------------");

    /* SD Card */
    sdcard.status();
    sdcard.mount();

    std::vector<std::string> files;
    std::vector<std::string> directories;
    sdcard.list("/", files, directories);
    console.println("Files: %i", files.size());
    console.println("Directories: %i", directories.size());

    console.timestamp();
    sdcard.open_file("KLST3.TXT", SDCard::WRITE | SDCard::CREATE_ALWAYS);
    static const char text[] = "Klangstrom (KLST) is an infrastructure to facilitate generative, networked, embedded sound + music + composition."; /* File write buffer */
    static const uint32_t length = strlen((char*) text);
    sdcard.write((uint8_t*) text, length);
    sdcard.close_file();

    console.timestamp();
    sdcard.open_file("KLST2.TXT", SDCard::READ);
    uint8_t bytes_read_buffer[length];
    sdcard.read(bytes_read_buffer, length);
    sdcard.close_file();

    console.timestamp();
    console.println("finished setup");
    console.println("---------------------------------------------------------");
}

void loop() {
    console.timestamp();
    console.println("---");
    klangstrom.loop();
    delay(1000);
}
