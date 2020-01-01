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