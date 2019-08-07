/*
 *  Opens a video stream.
 *  Finds a road marking and pushes it to <markings> list.
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

    cv::VideoCapture cap(argv[1]);
    if (!cap.isOpened())
    {
        printf("Can't open video file.");
        return -1;
    }

    cv::Size size(IMWIDTH, IMHEIGHT);
    cv::Mat frame; // todo: define as zeros with size and type

    #if (ON == VIDEOWRITER)
    cv::VideoWriter videoWriter("/home/roman/Videos/Processed/processed_video.avi",
                                    cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                                    30, size, true);


    if (!videoWriter.isOpened())
    {
        std::cout << "Cannot save the video to a file" << std::endl;
        std::cin.get(); //wait for any key press
        return -1;
    }
#endif
    CImageProcess vision;
    MarkingDetector detector;

    /* Possible overflow of index */
    for (uint16 frameNumber = 0; ; frameNumber++)
    {
        cap >> frame;

        if (frame.empty())
            break;

        cv::resize(frame, frame, size);

        vision.segmentation(frame, frameNumber);
        detector.strips = vision.GrayBunches;
        std::cout << vision.LowerSkyFiber << std::endl;

        std::cout << "Markings num: " << int(detector.find(vision.LowerSkyFiber)) << std::endl;
        vision.drawMarkings(frame, detector.markings);

#if (ON == VIDEOWRITER)
        videoWriter.write(frame);
#endif
        cv::imshow("Source", frame);
        cv::waitKey(3);
    }

#if (ON == VIDEOWRITER)
    videoWriter.release();
#endif

}

