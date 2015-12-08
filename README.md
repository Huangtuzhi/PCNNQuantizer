# PCNNQuantizer
使用 Pulse Coupled Neural Network(PCNN) 来对图像进行量化，最后移植到 Mini2440(ARM9) 嵌入式平台上,基于 QT 的图形界面。是 [《A New Method for Image Quantization Based on Adaptive Region Related Heterogeneous PCNN》](http://link.springer.com/chapter/10.1007%2F978-3-319-25393-0_30) 部分内容的 C++ 实现。

# 目录结构

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

# 结果

![](./quantizer.png)