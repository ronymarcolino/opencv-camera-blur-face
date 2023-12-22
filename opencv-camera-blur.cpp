#include <opencv2/opencv.hpp>
#include "FaceDetector.h"

constexpr int POINT_SIZE = 50;

int main(int argc, char** argv) {

    // open the default camera 0 using DirectShow API
    int deviceID = 1;             
    int apiID = cv::CAP_DSHOW;      

    cv::VideoCapture video_capture;
    video_capture.open(0);
    if (!video_capture.open(deviceID, apiID)) {
        std::cout << "Error opening camera" << "\n";
        return 0;
    }

    cv::Mat frame;
    FaceDetector face_detector;
    std::string data = "Face counter: ";

    // Set up proper region of interest (ROI) using a cv::Rect from the two cv::Points.
    // Only blur image within ROI.

    while (true) {
        video_capture >> frame;

        auto faces = face_detector.detect_face_rectangles(frame);
        cv::Scalar color(0, 105, 205);
        cv::Point bottomLeftCornerOfText(1, POINT_SIZE);
        cv::putText(frame, data + std::to_string(faces.size()), bottomLeftCornerOfText, cv::FONT_HERSHEY_SIMPLEX, 1, color, 2, 2);

        for (int index = 0; index < faces.size(); index++) {
            auto topLeft = cv::Point(faces[index].x, faces[index].y);
            auto bottomRight = cv::Point(faces[index].x + faces[index].width, faces[index].y + faces[index].height);
            auto roi = cv::Rect(topLeft, bottomRight);
            cv::rectangle(frame, faces[index], color, 4);
            cv::GaussianBlur(frame(roi), frame(roi), cv::Size(POINT_SIZE + 1, POINT_SIZE + 1), 0);
        }

        imshow("Face detection and blur", frame);
        int esc_key = 27; // Close screen if the ESC key was pressed
        if (cv::waitKey(10) == esc_key) {
            std::cout << "Exit" << "\n";
            break;
        }
    }

    cv::destroyAllWindows();
    video_capture.release();

    return 0;
}