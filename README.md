# smartsnipe-pose-estimation
In-net hockey training pose estimation for smartsnipe project

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
