# Auto calibration of multi-camera system for human pose estimation

## Introduction
This is a work-in-progress c++ implementation of the following paper using Qt libraries, onnxruntime, eigen and opencv:

> [**Auto calibration of multi-camera system for human pose estimation**](https://ietresearch.onlinelibrary.wiley.com/doi/abs/10.1049/cvi2.12130)  
> Liu, Kang and Chen, Lingling and Xie, Liang and Yin, Jian and Gan, Shuwei and Yan, Ye and Yin, Erwei <br>
> *IET Computer Vision 2022*

Essentially, it allows for multi camera calibration without any specific calibration patterns like a cube or a flat sheet, but just using the human body from different perspectives and clever use of RANSAC to find the camera parameters which result in the lowest reprojection error between cameras.

## Installation
Under releases, download the executable for your OS, no further setup required.

## Usage
A minimum of 2 cameras are required. All cameras involved must be in a fixed position and orientation, and must support real-time video capture. Additionally, there must be no delays between cameras. For example, a camera connected over wifi will likely have too much latency(additionally the latency will be variable) compared to cameras connected over cable/in-built, and will result in junk calibration results.

Additionally, as per the guidance in the paper, holding your hands up in the air(as if you are surrenderring) gives better results due to the nature of the projective geometry involved.

## Building from source
Qt creator is required and cmakelists.txt must be configured to get opencv, eigen, poselib and onnxruntime linked on your system properly.

## Limitations
while this implementation is functional, there is plenty of room for improvement that would improve results, however these are rather troublesome to implement. these include:
1. better pose estimation models that have higher accuracy could be used. A recent example is Sharpose, a new efficient 2d human pose model, however unlike RTMpose and RTMdet, they do not offer a c++ example of running inference on the model, and from experience this is very troublesome to implement.
2. Latency between camera captures - currently, camera captures are done in an asynchronous fashion using Qt's slots and signals. This is unpredictable though from testing, it adds negligible overhead and close to no latency, however cameras are not synced up frame-by-frame. This could allow for sub-optimal scenarios where the capture taken at a given moment in time is 1 behind or ahead from another, though we have no way of knowing this. This affects the output as the RANSAC model and algorithm assumes that all captures are taken in batches at the same time. For our purposes however it can be ignored as results are still good.
3. USB bandwidth and buffer overflow - depending on your hardware, a given usb hub can only support a given amount of bits per second, if you have 2 HD webcams streaming at 30fps, it can easily consume all available bandwidth and lead to confusing behaviours
4. pixel density - the AI models can only handle a set dimension of pixels. If one camera is far away from the person while calibrating, the lack of pixel precision places a hard limit on how precise the model can be when for example, it is placing a point to say where the eyes are. There is no fix to this, other than to manually zoom your camera so that it gets a better view of you(assuming your camera supports actual zooming and it is not just a digital effect)
5. RANSAC parameters - the RANSAC process accepts many hyperparameters that for this repo, have been arbritrarily chosen. Thorough investigation could yield better parameters that give better and faster calibration results.
6. Limited No of cameras - since all processing is happening on the machine running the application, and videocaptures are very memory and cpu intensive, you could easily run out of memory or compute. A solution to this would be to run inference on edge devices and just share pose results with main node, however this is beyond the scope of the repo

## Acknowledgements
This repo uses code from a few other repositories, here is each one and how they were used:
1. [RTMPoseDeploy](https://github.com/HW140701/RTMPose-Deploy)  - starting code that with some alterations was used for model inference and image preprocessing
2. [PoseLib](https://github.com/PoseLib/PoseLib) - RANSAC as well as models and solvers for determining fundamental matrix and camera parameters
