/*
 *  Starts a program. Runs a main segmentation function.
 *
 *
 *
 *
 *
 *
 */






#include "ColorVision.h"





int main(int argc, char* argv[])
{
    int RealDimX;
    int *BoundariesOfTheSky;

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


    Input_Data InputData;
    InputData.CameraNumber = 0;
    InputData.HorizontalVertical = 0;
    InputData.NumberOfStrips = 48;
    InputData.PermuteRatios = 1;
    InputData.VideoInputLimit = 500;
    InputData.m_NetworkDirectX = 1;
    InputData.m_ParamHaveBeenChanged = 0;
    InputData.m_VideoCameraIsLoaded = 1;
    InputData.SkyFinding = true;
    InputData.GreenFinding = true;


    CColorVisionApp *colorVisionProces = new CColorVisionApp;
    colorVisionProces->InputDataCP = &InputData;
    colorVisionProces->aligned_width = 3 * IMAGE_WIDTH; //DimenX_bytes;
    colorVisionProces->pBuffer = NULL;
    colorVisionProces->top_bottom_origin = 0;

    cv::Size size(IMAGE_WIDTH, IMAGE_HEIGHT);
    cv::Mat frame, resizedFrame;

    for (int iImg = 0;; ++iImg)
    {
        cap >> frame;
        if (frame.empty())
            break;

        cv::resize(frame, resizedFrame, size);
        RealDimX = IMAGE_WIDTH;

        BoundariesOfTheSky = new int[RealDimX];
        memset(BoundariesOfTheSky, (int) '\0', sizeof(int) * RealDimX);

        std::vector<Marking> markings;

        colorVisionProces->OnVideoCameraprocess(&resizedFrame, markings, IMAGE_WIDTH, IMAGE_HEIGHT, &InputData, iImg,
                10, BoundariesOfTheSky);


        colorVisionProces->DirectArrayDraw(0);
        colorVisionProces->draw_markings(markings);


        resizedFrame.data = colorVisionProces->pBuffer;

        imshow(window_name, resizedFrame);

        cv::waitKey(3);
    }

    delete[] BoundariesOfTheSky;
}

