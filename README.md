# nintendont-autoboot-forwarder
a simple forwarder for wii vc to autoboot a included game  
just compile with the options you want and replace main.dol in whatever wii vc you want to use it in.  

## Compiling on linux

### Install devkit pro.
```
wget https://apt.devkitpro.org/install-devkitpro-pacman
chmod +x ./install-devkitpro-pacman
sudo ./install-devkitpro-pacman
```

### Install devkit packages.
```
sudo dkp-pacman -S gamecube-dev wii-dev wiiu-dev
```

### Get the special versions of devkitPPC, devkitARM, libogc for Nintendont.
```
wget https://wii.leseratte10.de/devkitPro/devkitPPC/r35/devkitPPC-r35-1-linux.pkg.tar.xz

wget https://wii.leseratte10.de/devkitPro/devkitARM/r53%20%282019-06%29/devkitARM-r53-1-linux.pkg.tar.xz

wget https://wii.leseratte10.de/devkitPro/libogc/libogc_1.8.23%20%282019-10-02%29/libogc-1.8.23-1-any.pkg.tar.xz
```


### Install the special versions of the packages.
```
sudo dkp-pacman -U devkitARM-r53-1-linux.pkg.tar.xz

sudo dkp-pacman -U libogc-1.8.23-1-any.pkg.tar.xz

# This will fail and complain about existing files.
sudo dkp-pacman -U devkitPPC-r35-1-linux.pkg.tar.xz
```

### Remove the existing files for devkitPPC.
```
sudo rm /opt/devkitpro/devkitPPC/bin/powerpc-eabi-gdb \
        /opt/devkitpro/devkitPPC/bin/powerpc-eabi-run \
        /opt/devkitpro/devkitPPC/include/gdb/jit-reader.h \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/amd64-linux.xml \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/gdb-syscalls.dtd \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/i386-linux.xml \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/mips-n32-linux.xml \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/mips-n64-linux.xml \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/mips-o32-linux.xml \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/ppc-linux.xml \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/ppc64-linux.xml \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/sparc-linux.xml \
        /opt/devkitpro/devkitPPC/share/gdb/syscalls/sparc64-linux.xml
```

### Try again.
```
sudo dkp-pacman -U devkitPPC-r35-1-linux.pkg.tar.xz
```

### Add environment variables pointing to the devkits and the tools.
```
source /etc/profile.d/devkit-env.sh
```

### Build.
```
./Build.sh
```
