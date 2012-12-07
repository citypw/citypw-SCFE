#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x8495e121, "module_layout" },
	{ 0x3a3f86d, "unregister_jprobe" },
	{ 0xb94db510, "register_jprobe" },
	{ 0x1b9aca3f, "jprobe_return" },
	{ 0x27e1a049, "printk" },
	{ 0xb7c91aa8, "commit_creds" },
	{ 0x83f14dec, "prepare_kernel_cred" },
	{ 0xdf9dc79c, "current_task" },
	{ 0xb4390f9a, "mcount" },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "40A90E1E4C0002DE451EA9C");
