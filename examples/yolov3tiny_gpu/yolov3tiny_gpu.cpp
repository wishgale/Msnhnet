﻿#include <iostream>
#include "Msnhnet/net/MsnhNetBuilder.h"
#include "Msnhnet/io/MsnhIO.h"
#include "Msnhnet/config/MsnhnetCfg.h"
#include "Msnhnet/utils/MsnhOpencvUtil.h"

int main(int argc, char** argv) 
{
    if(argc != 2)
    {
        std::cout<<"\nYou need to give models dir path.\neg: yolov3tiny /your/models/dir/path/ \n\nModels folder must be like this:\nmodels\n  |-Lenet5\n    |-Lenet5.msnhnet\n    |-Lenet5.msnhbin";
        getchar();
        return 0;
    }

    std::string msnhnetPath = std::string(argv[1]) + "/yolov3_tiny/yolov3_tiny.msnhnet";
    std::string msnhbinPath = std::string(argv[1]) + "/yolov3_tiny/yolov3_tiny.msnhbin";
    std::string labelsPath  = "../labels/coco.names";
    std::string imgPath = "../images/dog.jpg";
    try
    {
        Msnhnet::NetBuilder  msnhNet;
        msnhNet.buildNetFromMsnhNet(msnhnetPath);
        std::cout<<msnhNet.getLayerDetail();
        msnhNet.loadWeightsFromMsnhBin(msnhbinPath);
        std::vector<std::string> labels ;
        Msnhnet::IO::readVectorStr(labels, labelsPath.data(), "\n");
        Msnhnet::Point2I inSize = msnhNet.getInputSize();

        std::vector<float> img;
        std::vector<std::vector<Msnhnet::Yolov3Box>> result;
        
        for (size_t i = 0; i < 10; i++)
        {
            Msnhnet::TimeUtil::startRecord();
            img = Msnhnet::OpencvUtil::getPaddingZeroF32C3(imgPath, cv::Size(inSize.x,inSize.y));
            result = msnhNet.runYolov3GPU(img);
            std::cout<<"time  : " << Msnhnet::TimeUtil::getElapsedTime() <<"ms"<<std::endl<<std::flush;
        }
        
        cv::Mat org = cv::imread(imgPath);
        Msnhnet::OpencvUtil::drawYolov3Box(org,labels,result,inSize);
        cv::imshow("test",org);
        cv::waitKey();
    }
    catch (Msnhnet::Exception ex)
    {
        std::cout<<ex.what()<<std::endl;
    }
    return 0;
}
