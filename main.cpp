#include "tranformcoding.h"

using namespace cv;

int main(int argc, char *argv[])
{
    Mat originalimage = imread("/home/timlentse/Qtproject/1.tif",CV_LOAD_IMAGE_GRAYSCALE);
     // Tranformcoding is a class which contains many methods
    Tranformcoding *method=new Tranformcoding; Mat dst;
    method->perform(originalimage,dst,6,"dft");  //depend on dct or dft
    imshow("the original image",originalimage);
    imshow ("the effect",dst);
    waitKey(0);
    method->FidelityCriteria(originalimage,dst);
    std::cout<<"the ems ="<<method->ems<<std::endl<<"and the SNR ="<<method->SNR<<std::endl;
    method->Imagentropy(originalimage,"SECOND_ORDER"); // i want to output the second_order entroy
    std::cout<<method->H;
    return 0;
}
