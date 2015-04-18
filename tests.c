#include "tests.h"
#include "bitbangTests.h"
#include "gpioProviderTests.h"


void runTests(){
    bitbangTests_runTests();
    gpioProviderTests_run();
}
