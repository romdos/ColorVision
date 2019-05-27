/*
 *  Starts a program. Opens video stream and calls a main segmentation function for each frame.
 *  Finds a road marking lines and pushes them to <markings> list.
 *  Draws results (markings).
 *  Records video with drawn results.
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

    CImageProcess vision;
    MarkingDetector marking_detector;

    std::string video_path = "/home/roman/Pictures/caltech-lanes/cordova1/f%05d.png";
    std::string window_name = "Source";

    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE);
    cv::moveWindow(window_name, 0, 0);

    cv::VideoCapture cap(video_path);
    if (!cap.isOpened())
    {
        printf("Can't open video file.");
        return -1;
    }

    cv::Size size(IMAGE_WIDTH, IMAGE_HEIGHT);
    cv::Mat frame, resized_fr; // todo: define as zeros with size and type

    cv::VideoWriter video_writer("/home/roman/Videos/Processed/processed_video.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, size, true);

    if (video_writer.isOpened() == false)
    {
        std::cout << "Cannot save the video to a file" << std::endl;
        std::cin.get(); //wait for any key press
        return -1;
    }

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

        vision.draw_markings(resized_fr, markings);

        video_writer.write(resized_fr);

        cv::imshow(window_name, resized_fr);
        cv::waitKey(3);
    }

    video_writer.release();
}

