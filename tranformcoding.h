#ifndef TRANFORM_CODING_H
#define TRANFORM_CODING_H
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
 using namespace cv;
 using namespace std;
enum { DCT=0,DFT=1,ZERO_ORDER=2,FIRST_ORDER=3,SECOND_ORDER=4};

class Tranformcoding
{
public:
    double ems,SNR,H;
    //构造函数，初始化成员变量
    Tranformcoding() {
        this->ems=0; this->SNR=0; this->H=0; this->index=0; }
    ~Tranformcoding(){
        delete this;
    }

    void  FidelityCriteria(const Mat &compressed, const Mat &decompressed);
    void  perform( const Mat &InputArray, Mat &OutputArray,const int reserve,const int flag);
    void  Imagentropy(const Mat &InputArray,const int flag);

private:
    void  Cosinecoding(Mat &InputArray,Mat &Z,const int reserve);
    void  Fouriercoding( Mat &InputArray, Mat &Z,const int reserve);
    int   index;
    Point maxidex;
};

#endif // TRANFORM_CODING_H
