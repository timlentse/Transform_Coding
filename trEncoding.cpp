#include "tranformcoding.h"

int main(int argc, char *argv[])
{
    Mat originalimage = imread("/home/timlentse/Qtproject/lena.tif",CV_LOAD_IMAGE_GRAYSCALE);

     // Tranformcoding是我定义的一个类，使用这个类来实例化一个对象,名字叫method
    Tranformcoding *method=new Tranformcoding; Mat dst;

  //前面我们已经有了method这个对象了，当然可以使用它来调用它本身的成员函数，来执行我们预先定义好的行为
  //例如method->Imagentropy 执行信息熵的计算
      method->Imagentropy(originalimage,ZERO_ORDER); // i want to output the second_order entroy
      cout<<"------the entropy ="<<method->H<<endl;

  // method->perform 执行块转换编码
    method->perform(originalimage,dst,8,DFT);  //depend on dct or dft

    imshow("the original image",originalimage);
    imshow ("the effect", dst);


  //method->FidelityCriteria 执行保真度计算
    method->FidelityCriteria(originalimage,dst);
    cout<<"------the ems ="<<method->ems<<endl<<"------and the SNR ="<<method->SNR<<endl;
    waitKey(0);
    return 0;
}
