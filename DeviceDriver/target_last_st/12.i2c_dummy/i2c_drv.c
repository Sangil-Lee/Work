#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>

#include <linux/i2c.h>

static int i2ctest_probe(struct i2c_client *client, const struct i2c_device_id *id)
{
	unsigned char temp;
	temp = i2c_smbus_read_byte_data(client, 0x00);
	pr_info("\ni2ctest_probe():Check Dummy I2C dev 0x00 = %x \n", temp);
	return 0;
}

static int i2ctest_remove(struct i2c_client *client)
{
	return 0;
}

static const struct i2c_device_id i2ctest_ids[] = {
	{"dummy_i2c_dev", 0},
	{}
};
MODULE_DEVICE_TABLE(i2c, i2ctest_ids);

static struct i2c_driver i2ctest_driver = {
	.driver = {
		.name = "dummy_i2c_dev",
		.owner = THIS_MODULE,
	},
	.probe = i2ctest_probe,
	.remove = i2ctest_remove,
	.id_table = i2ctest_ids,
};

static int __init i2ctest_init(void)
{
	int ret;
	pr_info("i2ctest_init()...\n");
	ret = i2c_add_driver(&i2ctest_driver);
	pr_info("i2ctest_init():Check Dummy I2C dev  = %d\n",ret);
	return ret;
}

static void __exit i2ctest_exit(void)
{
	pr_info("i2ctest_exit()...\n");
	i2c_del_driver(&i2ctest_driver);
}

module_init(i2ctest_init);
module_exit(i2ctest_exit);

MODULE_AUTHOR("LDD");
MODULE_DESCRIPTION("i2c test driver");
MODULE_LICENSE("GPL");
