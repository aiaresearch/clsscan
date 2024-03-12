//
// Created by Tony Mai on 2024/3/12.
//

#ifndef CLSSCAN_SCAN_H
#define CLSSCAN_SCAN_H

#define SOURCE_OPT 2
#define SOURCE_STR "ADF Front"
#define SCANNER_NAME "fujitsu:fi-6225dj:27240"

void init();
void init_device();
void scan();
void save();
void release();

#endif //CLSSCAN_SCAN_H
