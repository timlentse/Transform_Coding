#include "tranformcoding.h"


//这是计算图像保真度（包括ems,SNR)的子程序，你需要输入两幅图像，即原图和压缩的图
//第一个参数是原图，第二个参数是压缩后的图
void Tranformcoding::FidelityCriteria(const Mat &compressed,  const Mat &decompressed)
{
    if (compressed.size != decompressed.size|| compressed.type()!= decompressed.type())
    {
        cout<<"----------------------------------------------"<<endl;
        cout<<"error:nothing done because the two images are not in the same size or type "<<endl
           <<"It is recommended to check your two images"<<endl;
        cout<<"----------------------------------------------"<<endl;
        return ;
    }

    SNR = norm(decompressed, NORM_L2);
    ems = norm(compressed, decompressed, NORM_L2);
    SNR = pow((SNR/ems), 2);
    ems = sqrt(ems*ems/(compressed.rows* compressed.cols));
}


//以下是求图像信息熵的子函数 ,flag标志位：
//--ZERO ——ORDER ：求零阶熵
//--FIRST ——ORDER ：求一阶熵
//--SECOND ——ORDER ：求二阶熵
void Tranformcoding::Imagentropy(const Mat &InputArray, const int flag) {
    int array[766]= {0};
    {
        //像素值统计部分，放在array数组里面
        int row= InputArray.rows;
        int col= InputArray.cols;
        if (InputArray.isContinuous()) {
            col= col*row;  row= 1;
        }
        for (int i(0); i<row; ++i) {
            for (int j (0); j<col; ++j){
                const  uchar* p =InputArray.ptr<uchar>(i);
                if (flag== 2){
                    index=p[j];   }
                else if (flag== 3) {
                    index= p[j]+(j!=col-1)*p[j+1];  }

                else if(flag== 4) {
                    index= (j!=0)*p[j-1]+p[j]+(j!=col-1)*p[j+1];}
                array[index] ++;
            }
        }
    }
    //计算信息熵
    {
        double pr(0);
        for (int k(0); k<766; k++) {
            pr = array[k];
            if (pr!= 0) {
                pr/= (InputArray.cols* InputArray.rows);
                H =- (log(pr)/log(2))*pr + H; }
        }
    }
}


//定义perform函数来响应用户的需求，perform函数是用来实施块转换编码的
//根据标志位flag和保留系数个数来执行块转换编码
void Tranformcoding::perform( const Mat &InputArray,  Mat &OutputArray, const int reserve, const int flag) {

    OutputArray= Mat_<float>(InputArray);
    float t[8][8]= { {16,11,10,16,24,40,51,61},   {12,12,14,19,26,58,60,55},
                    {14,13,16,24,40,57,69,56},   {14,17,22,29,51,87,80,62},
                    {18,22,37,56,68,109,103,77},  {24,35,55,64,81,104,113,92},
                    {49,64,78,87,103,121,120,101},  {72,92,95,98,112,100,103,99}};
    Mat Z = Mat(8, 8, CV_32F, t);
    for (int u(0);  u<OutputArray.rows/8; ++u)
      {
        for (int v(0); v<OutputArray.cols/8;  ++v)
        {
            Mat temp(OutputArray,Rect(v*8,u*8,8,8));
            if (flag==1) {  Fouriercoding(temp, Z, reserve);  }
               else if (flag==0) {  Cosinecoding(temp, Z, reserve);  }
                  else
            {
                std::cout<<"--no matching fuction call,maybe the flag is wrong--"<<std::endl;
                OutputArray.convertTo(OutputArray, CV_8U);  return;}
        }
    }
    OutputArray.convertTo(OutputArray, CV_8U);
}



//这是余弦变换编码子函数
void Tranformcoding::Cosinecoding ( Mat &InputArray,  Mat &Z, const int reserve){
    dct(InputArray-128, InputArray, 0);  //to perform DCT
    divide(InputArray, Z, InputArray);
         Mat temp1 = InputArray.clone();
    for (int i(0);  i< reserve;  ++i) {
        minMaxLoc( abs(temp1), 0, 0, 0, &maxidex);
        temp1.at<float> (maxidex)=0;
    }
        subtract(InputArray, temp1, InputArray);
        multiply(InputArray, Z, InputArray);
        idct(InputArray, InputArray, 0);// now we need to perform idct
    InputArray +=128;
}


//这是傅立叶变换编码子函数
void Tranformcoding::Fouriercoding( Mat &InputArray,  Mat &Z,  const int reserve){
    // 新建一个Mat数组来存储傅立叶变换的实部，虚部
       Mat planes[] = {Mat::zeros(InputArray.size(),CV_32F), Mat::zeros(InputArray.size(), CV_32F)} ;
       Mat OutputArrayArr;
    dft(InputArray , OutputArrayArr);// Now to perform DFT
    split(OutputArrayArr, planes);
    divide(planes[0], Z, planes[0]);
    divide(planes[1], Z, planes[1]);
       Mat temp1 = planes[0].clone();
       Mat temp2 = planes[1].clone();
         for (int i(0); i<reserve; ++i)
   {
             minMaxLoc(abs(temp1),0,0,0, &maxidex);
             temp1.at<float> (maxidex)= 0;
             minMaxLoc(abs(temp2),0,0,0, &maxidex);
             temp2.at<float> (maxidex)= 0;
    }
       subtract(planes[0],temp1,planes[0]);

       subtract(planes[1],temp2, planes[1]);

       multiply(planes[0], Z, planes[0]);

       multiply(planes[1], Z, planes[1]);

       merge(planes,2, OutputArrayArr);
 dft(OutputArrayArr, InputArray,DFT_INVERSE+DFT_SCALE+DFT_REAL_OUTPUT);   //finally,to perform idft
}
