# Auto calibration of multi-camera system for human pose estimation

## Introduction
this is a work-in-progress c++ implementation of the following paper using Qt libraries, onnxruntime, arrayfire and opencv :

> [**Auto calibration of multi-camera system for human pose estimation**](https://ietresearch.onlinelibrary.wiley.com/doi/abs/10.1049/cvi2.12130)  
> Liu, Kang and Chen, Lingling and Xie, Liang and Yin, Jian and Gan, Shuwei and Yan, Ye and Yin, Erwei <br>
> *IET Computer Vision 2022*

## Installation
Under releases, download the executable for your platform, no further setup required.

## Usage
A minimum of 2 cameras are required. All cameras must be in sync, meaning that a camera connected over wifi will not be compatible with a camera connected by cable generally speaking.

## Building from source
Qt creator is required and cmakelists.txt must be configured to get opencv and onnxruntime linked on your system properly.
