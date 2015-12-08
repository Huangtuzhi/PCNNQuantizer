## PCNNQuantizer
基于 QT 图形界面，使用 Pulse Coupled Neural Network(PCNN) 算法对图像进行量化，最后移植到 Mini2440(ARM9) 嵌入式平台。

PCNN 是脉冲耦合神经网络模型，具有很好的同步脉冲发放特性。同时对于图像来说，产生的时间序列脉冲不受图像旋转、放大、缩小影响，这个性质可以用来减少图像处理中的几何变换，也可以用来提取图像特征。从 2-D 图像到 1-D 时间序列脉冲，这实质上就是降维。实现量化的具体原理参见

> Huang Y, Ma Y, Li S. A New Method For Image Quantization Based On Adaptive Region Related Heterogeneous PCNN. ISNN2015.

## 基本模型

PCNN 可以用下列公式来定量描述

<img src="http://latex.codecogs.com/gif.latex?\frac{\partial J}{\partial \theta_k^{(j)}}=\sum_{i:r(i,j)=1}{\big((\theta^{(j)})^Tx^{(i)}-y^{(i,j)}\big)x_k^{(i)}}+\lambda \theta_k^{(j)}" />

## 目录结构

```
.
├── cameraman.bmp #测试图片
├── main.cpp 
├── quantizer.cpp #主要的类
├── quantizer.h
├── Quantizer.pro
├── Quantizer.pro.user
├── quantizer.ui #UI界面
├── utils.cpp #算法和工具类
└── utils.h
```

## 界面

![](./quantizer.png)
