//
// Created by Tony Mai on 2024/3/12.
//

#include "scan.h"

#include <sane/sane.h>
#include <opencv2/opencv.hpp>

#include <cstdio>
#include <cstdlib>

using namespace std;

SANE_Status status;
SANE_Handle handle;
SANE_String_Const scanner_name = SCANNER_NAME;
SANE_Parameters parameters;
const SANE_Option_Descriptor *option;

SANE_String_Const source_str = SOURCE_STR;

//SANE_Byte buffer[BUFFER_SIZE];
SANE_Byte *buffer;
SANE_Int bytes_read;

void init() {
    SANE_Int version_code = 0;
    status = sane_init(&version_code, NULL);
    if (status != SANE_STATUS_GOOD) {
        fprintf(stderr, "Error: Sane_init %s\n", sane_strstatus(status));
        return;
    }
    printf("SANE version code: %d\n", version_code);
}

void init_device() {
    status = sane_open(scanner_name, &handle);
    if (status != SANE_STATUS_GOOD) {
        fprintf(stderr, "Error: Open device %s\n", sane_strstatus(status));
        return;
    }
    printf("Device opened successfully\n");

    // Get parameters
    status = sane_get_parameters(handle, &parameters);
    if (status != SANE_STATUS_GOOD) {
        fprintf(stderr, "Error: Get parameters %s\n", sane_strstatus(status));
        return;
    }
    printf("Parameters retrieved\n");
    printf("Format: %d\n", parameters.format);
    printf("Last frame: %d\n", parameters.last_frame);
    printf("Bytes per line: %d\n", parameters.bytes_per_line);
    printf("Pixels per line: %d\n", parameters.pixels_per_line);
    printf("Lines: %d\n", parameters.lines);
    printf("Depth: %d\n", parameters.depth);
    printf("Total bytes: %d\n", parameters.bytes_per_line * parameters.lines);

    // Set source to ADF Front
    option = sane_get_option_descriptor(handle, SOURCE_OPT);
    if (option == NULL) {
        fprintf(stderr, "Error: Get option descriptor\n");
        return;
    }
    printf("Option descriptor retrieved\n");
    printf("Option name: %s\n", option->name);
    printf("Option title: %s\n", option->title);
    printf("Option type: %d\n", option->type);

    sane_control_option(handle, SOURCE_OPT, SANE_ACTION_SET_VALUE, (void *) source_str, NULL);
}

void scan() {
    status = sane_start(handle);
    if (status != SANE_STATUS_GOOD) {
        fprintf(stderr, "Error: Scan: %s\n", sane_strstatus(status));
        return;
    }
    printf("Scanning started\n");

    double progr;
    SANE_Int total_bytes = 0;
    SANE_Int max_length = parameters.bytes_per_line * parameters.lines;
    buffer = (SANE_Byte *) malloc(max_length);
    printf("Buffer allocated, max_length=%d\n", max_length);

    while (true) {
        status = sane_read(handle, buffer + total_bytes, max_length, &bytes_read);
        total_bytes += bytes_read;
        progr = ((total_bytes * 100.) / (double) max_length);
        if (progr > 100.) progr = 100.;
        printf("Progress: %.2f%%\n", progr);
        if (status == SANE_STATUS_EOF || !bytes_read)
            break;
        if (status != SANE_STATUS_GOOD) {
            fprintf(stderr, "Error: Read: %s\n", sane_strstatus(status));
            return;
        }
    }
    printf("Scanning finished\n");
    printf("Bytes read: %d\n", bytes_read);

    cv::Mat image(parameters.lines, parameters.pixels_per_line, CV_8UC1);
    // Iterate over each line of the image buffer
    for (int y = 0; y < parameters.lines; ++y) {
        // Iterate over each pixel of the line
        for (int x = 0; x < parameters.pixels_per_line; ++x) {
            // Calculate the byte index and bit position of the current pixel
            int byteIndex = x / 8 + y * parameters.bytes_per_line;
            int bitIndex = 7 - (x % 8);

            // Read the bit value from the buffer and set it in the image matrix
            uchar bitValue = ~(buffer[byteIndex] >> bitIndex) & 1;
            image.at<uchar>(y, x) = bitValue * 255; // Scaling to 0-255 range for display
        }
    }

    printf("Image created\n, rows=%d, cols=%d", image.rows, image.cols);
    printf("Image mean intensity: %f\n", cv::mean(image)[0]);
    cv::imshow("Scanned Image", image);
    cv::waitKey(0); // Wait for a key press to close the window
    cv::destroyAllWindows();
    cv::imwrite("test.png", image);
}

void release() {
    sane_close(handle);
    sane_exit();
}