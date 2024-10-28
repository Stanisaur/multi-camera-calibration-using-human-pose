# Auto calibration of multi-camera system for human pose estimation

## Introduction
this is a work-in-progress c++ implementation of the following paper using Qt libraries, onnxruntime, arrayfire and opencv :

> [**Auto calibration of multi-camera system for human pose estimation**](https://ietresearch.onlinelibrary.wiley.com/doi/abs/10.1049/cvi2.12130)  
> Liu, Kang and Chen, Lingling and Xie, Liang and Yin, Jian and Gan, Shuwei and Yan, Ye and Yin, Erwei <br>
> *IET Computer Vision 2022*

Essentially, it allows for multi camera calibration without any specific calibration patterns on cubes or a flat sheet, just using the human body from different perspectives RANSAC to find the camera parameters which result in the lowest reprojection error.

## Installation
Under releases, download the executable for your OS, no further setup required.

## Usage
A minimum of 2 cameras are required. All cameras involved must be real-time capture. This means no delays. For example, a camera connected over wifi will likely have too much latency(additionally the latency will be variable) compared to cameras connected over cable/in-built, and will result in junk calibration results.

Additionally, as per the guidance in the paper, holding your hands up in the air(as if you are surrenderring) spreads out your limbs and gives better results.

## Building from source
Qt creator is required and cmakelists.txt must be configured to get opencv and onnxruntime linked on your system properly.
