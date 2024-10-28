# Auto calibration of multi-camera system for human pose estimation

## Introduction
this is a work-in-progress c++ implementation of the following paper using Qt libraries, onnxruntime, arrayfire and opencv :

> [**Auto calibration of multi-camera system for human pose estimation**](https://ietresearch.onlinelibrary.wiley.com/doi/abs/10.1049/cvi2.12130)  
> Liu, Kang and Chen, Lingling and Xie, Liang and Yin, Jian and Gan, Shuwei and Yan, Ye and Yin, Erwei <br>
> *IET Computer Vision 2022*

Essentially, it allows for multi camera calibration without any specific calibration patterns on cubes or a flat sheet, just using the human body from different perspectives and clever use of RANSAC to find the camera parameters which result in the lowest reprojection error.

## Installation
Under releases, download the executable for your OS, no further setup required.

## Usage
A minimum of 2 cameras are required. All cameras involved must be in a fixed position and orientation, and must be real-time capture. This means no delays. For example, a camera connected over wifi will likely have too much latency(additionally the latency will be variable) compared to cameras connected over cable/in-built, and will result in junk calibration results.

Additionally, as per the guidance in the paper, holding your hands up in the air(as if you are surrenderring) spreads out your limbs and gives better results.

## Building from source
Qt creator is required and cmakelists.txt must be configured to get opencv and onnxruntime linked on your system properly.

## Limitations
while this implementation is functional, there is plenty of room for improvement that would improve results, however these are rather troublesome to implement. these include:
1. better pose estimation models that have higher accuracy could be used. A recent example is Sharpose, a new efficient 2d human pose model, however unlike RTMpose and RTMdet, they do not offer a c++ example of running inference on the model, and from experience this is very troublesome to implement.
2. Latency between camera captures - currently, camera captures are done in a single threaded fashion, calling the cameras sequentially until a capture has been taken from the last camera. This means that there is a (minor) time gap between captures, especially since this implementation uses Qt's various QMultiMedia objects that are cross platform and easy use but add alot of unavoidable overhead. This affects the output as the RANSAC model and algorithm assumes that all captures are taken in batches at the same time. Some solutions to this include:
   - distributed captures on edge devices: each camera has an associated edge device timestamping each capture and relaying it to a main computer. the main computer compares timestamps and then batches together captures within a given tolerance.
   - multithreading: this does not solve the problem however it does reduce latency. this does not solve the case where there are more cameras than cores on the processor, however is sufficient for most users 
