# smartsnipe-pose-estimation
In-net hockey training pose estimation for smartsnipe project.

The objective is to infer player performance from an estimation of the pose (through action recognition and tracking of the stick)

Pre-trained OpenPose model (without stick):

![demo_wo_stick](/img/demo_wo_stick.gif?raw=true "not a hockey player")

## Mounting the Jetson filesystem
Development on the Jetson TX1 may be facilitated by locally mounting its file system
```
sudo mkdir /mnt/jetson
sudo sshfs -o allow_other,default_permissions nvidia@10.1.1.2:/ /mnt/jetson
```
Unmounting:
```
sudo umount /mnt/jetson
```
