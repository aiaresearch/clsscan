#include "main.h"
#include "scan.h"
#include "recognize.h"
#include "servo.h"
#include "GPIO.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <nlohmann/json.hpp>
#include <opencv2/opencv.hpp>
#include <thread>
#include <chrono>

using json = nlohmann::json;
using namespace std;
using namespace cv;


Mat img;

int main() {
    init();
    init_device();
    // Load the configuration file
    ifstream f("config.json");
    clsscan_config conf = json::parse(f);
    cout << "Config loaded" << endl;
//    GPIOmotor();
    cout << "GPIO inited" << endl;
    for(int i=0;i<20;i++){
	    cout << "Scanning " << i << " image..." << endl;
        // Load the image
        img = scan();                
        img = img_preprocess(img);

        // Find the mark points
        vector<Point2f> mark_points = find_mark_points(img);

        // Transform the points
        vector<Point2f> transformed_points = transform_points(mark_points, img.cols, img.rows, conf);

        // Extract the class number
        int class_number = extract_class_id(transformed_points, conf);
        cout << "Class number: " << class_number << endl;

        vector<int> option_numbers={3,4,5,21};
        // Control servo
        for(int i=0;i<4;i++){
            if (class_number == option_numbers[i]) {
            std::thread t(servo, i);
            t.detach();
            }
        }
    }
    GPIOmotoroff();
    
    release();
    return 0;
}
