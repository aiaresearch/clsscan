//
// Created by Tony Mai on 2024/3/12.
//

#include "main.h"
#include "scan.h"

int main() {
    init();
    init_device();
    scan();
    release();
    return 0;
}