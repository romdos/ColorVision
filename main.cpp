/*
 *  Starts a program. Runs a main segmentation function.
 *  Finds a road marking line.
 *
 *
 *
 *
 *
 */



#include "ImageProcess.h"




int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "No video path!\n";
        return -1;
    }

    std::string video_path = argv[1];
    std::string window_name = "Source";

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(window_name, 0, 0);

    cv::VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        printf("Can't open video file.");
        return -1;
    }

    CImageProcess vision;
    MarkingDetector marking_detector;

    cv::Size size(IMAGE_WIDTH, IMAGE_HEIGHT);
    cv::Mat frame;
    cv::Mat resized_fr; // todo: define as zeros with size and type

    for (size_t fr_number = 0; ; ++fr_number)
    {
        cap >> frame;

        if (frame.empty())
            break;

        cv::resize(frame, resized_fr, size);

        vision.segment(resized_fr, fr_number);

        std::vector<Marking> markings;
        marking_detector.strips = vision.GrayBunches;
        marking_detector.find(markings, vision.LowerSkyFiber);

        std::cout << vision.LowerSkyFiber << std::endl;

        vision.draw_markings(resized_fr, markings);

        cv::imshow(window_name, resized_fr);

        cv::waitKey(3);
    }
}

