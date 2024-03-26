#include "main.h"
#include "scan.h"
#include "recognize.h"
#include "serial.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>

using json = nlohmann::json;
using namespace std;
using namespace cv;

Mat img;
int class_number;

int main() {
    // Init scanner
    init();
    init_device();

    // Load the configuration file
    ifstream f("config.json");
    clsscan_config conf = json::parse(f);
    cout << "Config loaded" << endl;

    // Init serial

    // Main loop
    for(int i = 0; i < 5; i++){
	    cout << "Scanning " << i << " image...  ";
        //Load the image
        img = scan();    

        //img = cv::imread("img/test.png");
        //img = img_preprocess(img);

        // Extract the class number
        vector<Point2f> mark_points = find_mark_points(img);
        vector<Point2f> transformed_points = transform_points(mark_points, img.cols, img.rows, conf, img);

        class_number = extract_class_id(transformed_points, conf);
        cout << "Class number: " << class_number << endl;

        if (class_num>=1 && class_num<=26){
            if (class_num>4) class_num = 4;
            cout << "Switching servo: " << class_number << endl;
            // Send the class number to the serial
            //send_serial(class_number);
        }
        else{
            cout << "Invalid class number" << endl;
        }
    }
    
    release();
    return 0;
}
