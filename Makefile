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


.PHONY: all download_kernel extract_kernel copy_files build_kernel install_modules reboot

KERNEL_VERSION := 6.7.6
KERNEL_DIR := linux-$(KERNEL_VERSION)
KERNEL_TAR := $(KERNEL_DIR).tar.gz
KERNEL_URL := https://mirrors.edge.kernel.org/pub/linux/kernel/v6.x/$(KERNEL_TAR)

all: download_kernel extract_kernel copy_files build_kernel install_modules
	
download_kernel:
	@if [ ! -d "$(KERNEL_DIR)" ]; then \
		if [ ! -f "$(KERNEL_TAR)" ]; then \
			wget $(KERNEL_URL); \
		fi; \
		tar -xzf $(KERNEL_TAR); \
	fi

extract_kernel:
	@if [ ! -d "$(KERNEL_DIR)" ]; then \
		tar -xzf $(KERNEL_TAR); \
	fi

copy_files:
	@cp $(SRC_DIR)/get_pid_info.c $(KERNEL_DIR)/kernel/
	@cp $(SRC_DIR)/ft_wait.c $(KERNEL_DIR)/kernel/
	@cp $(SRC_DIR)/ft_kill.c $(KERNEL_DIR)/kernel/
	@cp $(SRC_DIR)/kernel.makefile $(KERNEL_DIR)/kernel/Makefile
	@cp $(SRC_DIR)/syscall_64.tbl $(KERNEL_DIR)/arch/x86/entry/syscalls/
	@cp $(SRC_DIR)/syscalls.h $(KERNEL_DIR)/include/linux/
	@cp $(SRC_DIR)/kernel_config $(KERNEL_DIR)/.config

build_kernel:
	@cd $(KERNEL_DIR) && make -j $$(nproc)

install_modules:
	@cd $(KERNEL_DIR) && sudo make modules_install
	@sudo cp -iv $(KERNEL_DIR)/arch/x86/boot/bzImage /boot/vmlinuz-$(KERNEL_VERSION)-lenart


reboot:
	@echo "Do you want to reboot now? [y/N]" && read ans && [ $${ans:-N} = y ] && sudo reboot

