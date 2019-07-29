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

    cv::namedWindow("Source", cv::WINDOW_AUTOSIZE);
    cv::moveWindow("Source", 0, 0);

    cv::VideoCapture cap("/home/roman/Pictures/caltech-lanes/cordova1/f%05d.png");
    if (!cap.isOpened())
    {
        printf("Can't open video file.");
        return -1;
    }

    cv::Size size(IMWIDTH, IMHEIGHT);
    cv::Mat frame; // todo: define as zeros with size and type

    cv::VideoWriter videoWriter("/home/roman/Videos/Processed/processed_video.avi",
                                    cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                    30, size, true);

    if (!videoWriter.isOpened())
    {
        std::cout << "Cannot save the video to a file" << std::endl;
        std::cin.get(); //wait for any key press
        return -1;
    }

    CImageProcess vision;
    MarkingDetector detector;

    /* Possible overflow of index */
    for (uint16 frameNumber = 0; ; frameNumber++)
    {
        cap >> frame;

        if (frame.empty())
            break;

        cv::resize(frame, frame, size);

        vision.segment(frame, frameNumber);

        std::vector<Marking> markings;

        detector.strips = vision.GrayBunches;
        std::cout << vision.LowerSkyFiber << std::endl;
        detector.find(markings, vision.LowerSkyFiber);

        vision.draw_markings(frame, markings);

        videoWriter.write(frame);

        cv::imshow("Source", frame);
        cv::waitKey(3);
    }

    videoWriter.release();
}

