#include "tranformcoding.h"
using namespace cv;
using namespace std;

void Tranformcoding::FidelityCriteria(const Mat &compressed,  const Mat &decompressed)
{
    if (compressed.size!=decompressed.size||compressed.type()!=decompressed.type())
    {
        cout<<"the compressed and decoompressed images are not in the same szie or in the same type"<<endl
           <<"Please check your two images"<<endl;
        return ;
    }
    SNR=norm(decompressed,NORM_L2);
    ems=norm(compressed,decompressed,NORM_L2);
    SNR=pow((SNR/ems),2);
    ems=sqrt(ems*ems/(compressed.rows*compressed.cols));
}

void Tranformcoding::perform(  const Mat &InputArray,  Mat &OutputArray, const int reserve, const String flag) {
    OutputArray=InputArray.clone();
    OutputArray.convertTo(OutputArray,CV_32F);
    float t[8][8]={ {16,11,10,16,24,40,51,61},   {12,12,14,19,26,58,60,55},
                    {14,13,16,24,40,57,69,56},   {14,17,22,29,51,87,80,62},
                    {18,22,37,56,68,109,103,77},  {24,35,55,64,81,104,113,92},
                    {49,64,78,87,103,121,120,101},  {72,92,95,98,112,100,103,99}};
    Mat Z=Mat(8,8,CV_32F,t);
    for (int u(0);  u<OutputArray.rows/8;  ++u){
        for (int v(0);  v<OutputArray.cols/8;  ++v){
            Mat temp(OutputArray,Rect(v*8,u*8,8,8));
            if (flag=="dft") {   Fouriercoding(temp, Z, reserve);  }
            else if (flag=="dct") {       Cosinecoding(temp, Z, reserve);  }
            else  {
                std::cout<<"no matching fuction call,maybe the flag is wrong"<<std::endl;
                OutputArray.convertTo(OutputArray,CV_8U);    return;}
        }
    }
    OutputArray.convertTo(OutputArray,CV_8U);
}

void Tranformcoding::Cosinecoding( Mat &InputArray,  Mat &Z, const int reserve){
    Point maxidex;
    dct(InputArray-128,InputArray,0);  //to perform DCT
    divide(InputArray,Z,InputArray);
    Mat temp1=InputArray.clone();
    for (int i(0);  i< reserve;  ++i){
        minMaxLoc(abs(temp1),0,0,0,&maxidex);
        temp1.at<float>(maxidex)=0;
    }
    subtract(InputArray,temp1,InputArray);
    multiply(InputArray,Z,InputArray);
    idct(InputArray,InputArray,0);// now we need to perform idct
    InputArray=InputArray+128;
}

void Tranformcoding::Fouriercoding( Mat &InputArray,  Mat &Z,  const int reserve){
    Point maxidex;
    // create a plane containing 2 mat layer to form a 2-channel mat object
    Mat planes[] ={Mat::zeros(InputArray.size(),CV_32F), Mat::zeros(InputArray.size(), CV_32F)} ;
    //new a Mat to store the result of dft
    Mat OutputArrayArr;
    dft(InputArray,OutputArrayArr);// now to perform DFT
    split(OutputArrayArr,planes);
    divide(planes[0],Z,planes[0]);
    divide(planes[1],Z,planes[1]);
    Mat temp1=planes[0].clone();
    Mat temp2=planes[1].clone();
    for (int i(0);i<reserve;++i){
        minMaxLoc(abs(temp1),0,0,0,&maxidex);
        temp1.at<float>(maxidex)=0;
        minMaxLoc(abs(temp2),0,0,0,&maxidex);
        temp2.at<float>(maxidex)=0;
    }
    subtract(planes[0],temp1,planes[0]);

    subtract(planes[1],temp2,planes[1]);

    multiply(planes[0],Z,planes[0]);

    multiply(planes[1],Z,planes[1]);
    merge(planes,2,OutputArrayArr);
    dft(OutputArrayArr,OutputArrayArr,DFT_INVERSE+DFT_SCALE|DFT_REAL_OUTPUT);   //finally,to perform idft
    OutputArrayArr.copyTo(InputArray);
}

void Tranformcoding::Imagentropy(const Mat &InputArray, const String flag) {
    int array[766]={0};
    {
        int row=InputArray.rows;
        int col=InputArray.cols;
        if (InputArray.isContinuous()) {
            col=col*row;  row=1;
        }
        for (int i(0); i<row; ++i) {
            for (int j (0);j<col; ++j){
                const  uchar* p =InputArray.ptr<uchar>(i);
                if (flag=="FIRST_ORDER") {
                    index=p[j]+(j!=col-1)*p[j+1];  }

                else if(flag=="SECOND_ORDER") {
                    index=(j!=0)*p[j-1]+p[j]+(j!=col-1)*p[j+1];}
                array[index]++;
            }
        }
    }
    {
        double pr(0);
        for (int k(0);k<766;k++) {
            pr=array[k];
            if (pr!=0) {
                pr/=(InputArray.cols*InputArray.rows);
                H=-(log(pr)/log(2))*pr+H; }
        }
    }
}
