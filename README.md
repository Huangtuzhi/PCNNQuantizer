# PCNNQuantizer
基于 QT 的图形界面，使用 Pulse Coupled Neural Network(PCNN) 对图像进行量化，最后移植到 Mini2440(ARM9) 嵌入式平台上。是本人论文部分内容的 C++ 实现。

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

# 界面

![](./quantizer.png)