#include "tranformcoding.h"

int main(int argc, char *argv[])
{
    Mat originalimage = imread("/home/timlentse/Qtproject/1.tif",CV_LOAD_IMAGE_GRAYSCALE);

     // Tranformcoding is a class which contains many methods
    Tranformcoding *method=new Tranformcoding; Mat dst;

    method->perform(originalimage,dst,1,DCT);  //depend on dct or dft

    imshow("the original image",originalimage);
    imshow ("the effect",dst);
    waitKey(0);

    method->FidelityCriteria(originalimage,dst);
    cout<<"the ems ="<<method->ems<<endl<<"and the SNR ="<<method->SNR<<endl;

    method->Imagentropy(originalimage,FIRST_ORDER); // i want to output the second_order entroy
    cout<<"the entroy ="<<method->H<<endl;
    return 0;
}
