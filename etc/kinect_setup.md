# Microsoft Kinect v1 setup on Jetson TX1
## Build the OpenKinect drivers

Install package dependencies
```
$ sudo apt-add-repository universe
$ sudo apt-get update
$ sudo apt-get install cmake freeglut3-dev pkg-config build-essential libxmu-dev libxi-dev libusb-1.0-0-dev
```

Clone the **libfreenect** driver
```
$ git clone https://github.com/OpenKinect/libfreenect.git
```

To build **libfreenect**, add ```set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++11")``` to ```libfreenect/CMakeLists.txt``` (line 119) and start build process:
```
$ cd libfreenect
$ mkdir build && cd build
$ cmake ..
$ make
$ sudo make install
```

Connect Kinect sensor to the Jetson via USB; grant user privileges and add Kinect device udev rules:
```
$ sudo adduser $USER video
$ sudo cp platform/linux/udev/51-kinect.rules /etc/udev/rules.d/
$ sudo udevadm control --reload-rules && sudo service udev restart && sudo udevadm trigger
```

The following test may be run, but it will likely fail (due to known issues addressed below)
```
$ cd bin
$ 
$ ./freenect-glview
```

## Troubleshooting
### Disable USB-autosuspend

Add ```echo -1 > /sys/module/usbcore/parameters/autosuspend``` to ```/etc/rc.local``` (before exit line) and then reboot

### Updating firmware

To address **LIBUSB_ERROR_IO** and **LIBUSB_ERROR_NO_DEVICE** due to Xbox NUI Audio (check with ```lsusb``` and ```dmesg -wH```) repeatedly connecting/disconnecting, run the following:
```
$ cd libfreenect/src
$ python fwfetcher.py  # grabs latest firmware
$ cd ../build
$ mkdir Resources
$ cp ../src/audios.bin ./Resources
$ cd bin
$ ./freenect-micview  # Updates firmware and opens mic channel -> Ctrl+c
$ ./freenect-glview
```
Running the last command should have opened a double-pane window with a camera view on one side and a depth view on the other

### OpenGL error
A known OpenGL [issue](https://devtalk.nvidia.com/default/topic/1007290/jetson-tx2/building-opencv-with-opengl-support-/post/5141945/#5141945) may be encountered when building OpenCV from source on the Jetson. Edit ```/usr/local/cuda-9.0/include/cuda_gl_interop.h``` according to the following diff:
```
diff --git a/cuda_gl_interop.h b/cuda_gl_interop.h
index 0f4aa17..e8c538c 100644
--- a/cuda_gl_interop.h
+++ b/cuda_gl_interop.h
@@ -59,13 +59,13 @@
 
 #else /* __APPLE__ */
 
-#if defined(__arm__) || defined(__aarch64__)
-#ifndef GL_VERSION
-#error Please include the appropriate gl headers before including cuda_gl_interop.h
-#endif
-#else
+//#if defined(__arm__) || defined(__aarch64__)
+//#ifndef GL_VERSION
+//#error Please include the appropriate gl headers before including cuda_gl_interop.h
+//#endif
+//#else
 #include <GL/gl.h>
-#endif
+//#endif
```

# Installing OpenCV-3.4.0
Build **opencv-3.4.0** from the install directory (on SD card or SSD)
```
$ cd ~/install
$ wget https://github.com/opencv/opencv/archive/3.4.0.zip -O opencv-3.4.0.zip
$ unzip opencv-3.4.0.zip
$ cd opencv-3.4.0
$ mkdir build
$ cd build
```
Set **CUDA_ARCH_BIN="5.3"** for the Jetson TX1
```
$ cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local \
        -D WITH_CUDA=ON -D CUDA_ARCH_BIN="5.3" -D CUDA_ARCH_PTX="" \
        -D WITH_CUBLAS=ON -D ENABLE_FAST_MATH=ON -D CUDA_FAST_MATH=ON \
        -D ENABLE_NEON=ON -D WITH_LIBV4L=ON -D BUILD_TESTS=OFF \
        -D BUILD_PERF_TESTS=OFF -D BUILD_EXAMPLES=OFF \
        -D WITH_QT=ON -D WITH_OPENGL=ON ..
$ make -j4
$ sudo make install
```
The following OpenGL errors may occur during the make process:
```
/usr/lib/gcc/aarch64-linux-gnu/5/../../../aarch64-linux-gnu/libGL.so: undefined reference to `drmMap'
/usr/lib/gcc/aarch64-linux-gnu/5/../../../aarch64-linux-gnu/libGL.so: undefined reference to `drmCloseOnce'
/usr/lib/gcc/aarch64-linux-gnu/5/../../../aarch64-linux-gnu/libGL.so: undefined reference to `drmUnmap'
/usr/lib/gcc/aarch64-linux-gnu/5/../../../aarch64-linux-gnu/libGL.so: undefined reference to `drmOpenOnce'
```
Execute the following
```
$ cd /usr/lib/aarch64-linux-gnu/
$ sudo ln -sf tegra/libGL.so libGL.so
$ cd -
```

