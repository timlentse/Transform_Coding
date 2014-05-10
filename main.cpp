#include "tranformcoding.h"

using namespace cv;

int main(int argc, char *argv[])
{
    Mat decompress = imread("/home/timlentse/Qtproject/1.tif",CV_LOAD_IMAGE_GRAYSCALE);
    Tranformcoding *a=new Tranformcoding;
    Mat dst;
    a->perform(decompress,dst,6,"dft");
    imshow ("the effect",dst);
    waitKey(0);
    a->FidelityCriteria(decompress,dst);
    std::cout<<a->ems<<" "<<a->SNR;
    a->Imagentropy(decompress,"SECOND_ORDER");
    std::cout<<a->H;
    return 0;
}
