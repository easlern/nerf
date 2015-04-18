#include <stdint.h>        /* For uint8_t definition */
#include <stdbool.h>       /* For true/false definition */

#include "testing.h"


bool testing_shouldEqual (uint8_t x, uint8_t y){
    if (x != y){
        return false;
    }
    return true;
}

bool testing_shouldBeGreaterThan (uint8_t x, uint8_t y){
    if (x <= y){
        return false;
    }
    return true;
}