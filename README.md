###实验11 图像压缩

这是关于数字图像处理手册`project-8 `实验报告
查看实验手册点击[LabpProject](https://drive.google.com/file/d/0BwHUE1e8o2K1bHJEeWNEQnBXYm8/edit?usp=sharing "实验手册链接地址")

###实验内容
* 图像保真度计算
* 图像信息熵的计算
* 图像的转换编码
* 小波编码

###实验目的
本次实验的目的在于通过实验操作了解图像的保真度的准则计算，图像的块转换编码的原理及其在OpenCV中的实验过程。同时我们需要通过这次实验学会使用图像的信息熵以及保真度来衡量图像压缩的质量，并且通过这些衡量标准来评价傅立叶编码、余弦编码、小波编码的各自优势。

###实验原理
1. 保真度  
关于保真度的原理我没太多的解释的。我们可以参考书本有关图像保真度的定义，它描述的是原图像跟压缩后的图像的信息保留程度，用均方根误差ems跟信噪比SNR来表征，这是我找的的维基[链接](http://en.wikipedia.org/wiki/Fidelity)。

2. 信息熵  
关于图像的信息熵的定义以及计算可以参考书本，当然，懒得翻书的话可以点击[here](http://hustlg.bokee.com/1600245.html)

3. 图像的块转换编码  
图像的块转换编码大概流程也就是：首先我们先将一幅M X N的图片分成许多张n X n的子图片 ，假设取n=8，然后我们对子图像作傅立叶变换或者余弦变换
然后我们将变换的结果除以标准化矩阵，这是量化的结果，再将结果做编码 ，这样的流程就是压缩的过程，同时，解压缩过程刚好相反，我们先把编码解码，然后把解码结果乘以标准化矩阵，再将结果作反变换，将所有的子图片合成原来大小的图片，整个压缩和解压缩的过程也就是这么一回事！

4. 小波编码  
小波编码跟上面所说的块转换的思想差不多，只是变换为小波变化，以及压缩过程不需要将原图拆分成多张子图像。

###实验结果讨论  
说明： 程序里面的Tranformcoding 类已经实现了关于以上实验任务的功能 ，`FidelityCriteria`求保真度，`perform`执行块转换编码,`Imagentropy`执行信息熵的计算,这样的花我们的程序就会变得很简单了，我们只要创建该类的对象就可以轻松的调用方法来完成我们的实验任务，而且通过输入不同的参数以及标志位，我们可以比较不同的结果。
#####DCT 编码，保留8个系数，输出0阶熵：
结果如下：  
<p style = "text-align:center"> 原图</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/oringnal.png"/></center>

<p style = "text-align:center">效果图</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/dct_8_reserve.png"/></center>

<p style = "text-align:center">信息熵及保真度</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/5.png"/></center>

#####DCT 编码,保留4个系数,输出0阶熵：
结果如下       
<p style = "text-align:center"> 效果图</p>   
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/dct_4_reserve.png"/></center>

<p style = "text-align:center"> 信息熵及保真度</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/result_dct_4_reserve.png"/></center>

#####DCT 编码,保留2个系数,输出0阶熵：
结果如下  
<p style = "text-align:center"> 效果图</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/dct_2_reserve.png"/></center>

<p style = "text-align:center"> 信息熵及保真度</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/result_dct_2_reserve.png"/></center>

#####DFT 编码,保留4个系数,输出0阶熵：
<p style = "text-align:center"> 效果图</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/dft_4_reserve.png"/></center>

<p style = "text-align:center"> 信息熵及保真度</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/result_dft_4_reserve.png"/></center>

#####DFT 编码,保留8个系数,输出0阶熵：
<p style = "text-align:center"> 效果图</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/dft_8_reserve.png"/></center>

<p style = "text-align:center"> 信息熵及保真度</p>
<center><img src ="https://raw.githubusercontent.com/timlentse/tranfrom-coding/master/result_dft_8_reserve.png"/></center>

`讨论`

从以上的结果我们可以看出随着我们保留的系数个数减少,压缩图片的质量会降低,这是符合常理的结论,因为随着我们保留的系数越少,原图丢掉的信息就会越多,要是从数据上说话的话,我们可以看看信噪比以及均方根误差,信噪比越小，说明压缩图像跟原图差别越大。另一方面我们发现,经过傅立叶转换编码的效果跟余弦转换编码的效果相比差很多,而且出现很明显的马赛克,这是由于傅立叶变换的吉布斯现象造成的,所以在块转换编码方面，余弦变换更具优势。
