ifeq ($(CONFIG_MACH_LENTISLTE_SKT),y)
KBUILD_CPPFLAGS += -DFEATURE_UKNIGHT_MHI_DEDICATE_CHANNEL
endif

ifeq ($(CONFIG_MACH_LENTISLTE_LGT),y)
KBUILD_CPPFLAGS += -DFEATURE_UKNIGHT_MHI_DEDICATE_CHANNEL
endif

obj-m := mhi.o
mhi-objs := mhi_main.o mhi_iface.o mhi_init.o mhi_isr.o mhi_mmio_ops.o mhi_ring_ops.o mhi_states.o mhi_sys.o mhi_uci.o msm_rmnet_mhi.o mhi_bhi.o mhi_pm.o mhi_ssr.o
