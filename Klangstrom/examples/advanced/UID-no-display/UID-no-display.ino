/**
 * this example demonstrates how to print the unique ID of the device
 */

#include <iomanip>
#include <sstream>
#include <string>

#include "Arduino.h"
#include "System.h"
#include "Console.h"

std::string formatHex(const int num) {
    std::stringstream ss;
    ss << "0x" << std::uppercase << std::setfill('0') << std::setw(8) << std::hex << num;
    return ss.str();
}

void print_UID() {
    console_print("{");
    for (int i = 0; i < 3; i++) {
        const std::string mUID = formatHex(system_get_UID(i));
        console_print("%s", mUID.c_str());
        if (i < 2) {
            console_print(", ");
        }
    }
    console_print("}, ");
}


void setup() {
    system_init();

    print_UID();
    console_println("// KLST_..._%i", system_get_UID_index());
}

void loop() {}
