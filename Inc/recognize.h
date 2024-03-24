#ifndef CLSSCAN_RECOGNIZE_H
#define CLSSCAN_RECOGNIZE_H


#include <opencv2/opencv.hpp>
#include <vector>
#include <config.h>

cv::Mat img_preprocess(const cv::Mat& image);

std::vector<cv::Point2f> find_mark_points(const cv::Mat& img);

std::vector<cv::Point2f> transform_points(const std::vector<cv::Point2f>& locations, int im_w, int im_h, const clsscan_config& config);

int extract_class_id(const std::vector<cv::Point2f>& locations, const clsscan_config& config);

#endif //CLSSCAN_RECOGNIZE_H