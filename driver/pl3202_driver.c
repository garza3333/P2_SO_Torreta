#include <linux/module.h>
#include <linux/init.h>
#include <linux/usb.h>

/* Meta Information */
MODULE_AUTHOR("");
MODULE_DESCRIPTION("Device driver for PL2303");

#define VENDOR_ID 0x067b
#define PRODUCT_ID 0x2303

static struct usb_device_id usb_dev_table [] = {
	{ USB_DEVICE(VENDOR_ID, PRODUCT_ID) },
	{},
};
MODULE_DEVICE_TABLE(usb, usb_dev_table);

static int my_usb_probe(struct usb_interface *intf, const struct usb_device_id *id) {
	// Print to kernel log
	printk("PL2303 - Probe Function\n");
	return 0;
}

static void my_usb_disconnect(struct usb_interface *intf) {
	printk("PL2303 - Disconnect Function\n");
}

static struct usb_driver my_usb_driver = {
	.name = "pl2303_driver",
	.id_table = usb_dev_table,
	.probe = my_usb_probe,
	.disconnect = my_usb_disconnect,
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init my_init(void) {
	int result;
	printk("pl2303_driver- Init Function\n");
	result = usb_register(&my_usb_driver);
	if(result) {
		printk("pl2303 - Error during register!\n");
		return -result;
	}

	return 0;
}

/**
 * This function is called when the module is removed from the kernel
 */
static void __exit my_exit(void) {
	printk("pl2302 - Exit Function\n");
	usb_deregister(&my_usb_driver);
}

module_init(my_init);
module_exit(my_exit);


