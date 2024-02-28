# kernel version -> linux-6.7.6
# syscall_64.tbl -> linux-6.7.6/arch/x86/entry/syscalls/syscall_64.tbl
# kernel_config -> linux-6.7.6/.config 
# make
# sudo make modules_install
# sudo cp -iv arch/x86/boot/bzImage /boot/vmlinuz-6.7.6-lenart
# sudo cp -iv System.map /boot/System.map-6.7.6
# sudo cp -iv .config /boot/config-6.7.6
# sudo cp -r Documentation/ /usr/share/doc/linux-6.7.6
# Add a new entry to grub
# menuentry "GNU/Linux, Linux 6.7.6-lenart" {
#        linux   /boot/vmlinuz-6.7.6-lenart root=/dev/sda3 ro
# }
