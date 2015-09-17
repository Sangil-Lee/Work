/* linux/arch/arm/mach-s5pv210/mach-sv210.c
 *
 * HyBus - SV210 by newboder
 */

#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/serial_core.h>
#include <linux/gpio.h>
#include <linux/videodev2.h>
#include <linux/i2c.h>
#include <linux/regulator/max8698.h>
#include <linux/clk.h>
#include <linux/delay.h>
#include <linux/usb/ch9.h>
#include <linux/pwm_backlight.h>
#include <linux/spi/spi.h>

#include <asm/mach/arch.h>
#include <asm/mach/map.h>
#include <asm/setup.h>
#include <asm/mach-types.h>

#include <mach/map.h>
#include <mach/regs-clock.h>
#include <mach/regs-mem.h>
#include <mach/regs-gpio.h>
#include <mach/gpio-bank.h>
#include <mach/ts.h>
#include <mach/adc.h>

#include <media/po1150k_platform.h>

#include <plat/regs-serial.h>
#include <plat/s5pv210.h>
#include <plat/devs.h>
#include <plat/cpu.h>
#include <plat/fb.h>
#include <plat/gpio-cfg.h>
#include <plat/iic.h>
#include <plat/spi.h>
#include <plat/fimc.h>
#include <plat/csis.h>
#include <plat/mfc.h>
#include <plat/sdhci.h>
#include <plat/ide.h>
#include <plat/regs-otg.h>
#include <plat/clock.h>
#include <mach/gpio-bank-b.h>
#ifdef CONFIG_ANDROID_PMEM
#include <linux/android_pmem.h>
#include <plat/media.h>
#endif

#if defined(CONFIG_PM)
#include <plat/pm.h>
#endif

#include <linux/sv210_keys.h>
#include <linux/input.h>
#include <linux/leds.h>
#include <mach/gpio_sv210.h>

/* Following are default values for UCON, ULCON and UFCON UART registers */
#define S5PV210_UCON_DEFAULT	(S3C2410_UCON_TXILEVEL |	\
				 S3C2410_UCON_RXILEVEL |	\
				 S3C2410_UCON_TXIRQMODE |	\
				 S3C2410_UCON_RXIRQMODE |	\
				 S3C2410_UCON_RXFIFO_TOI |	\
				 S3C2443_UCON_RXERR_IRQEN)

#define S5PV210_ULCON_DEFAULT	S3C2410_LCON_CS8

#define S5PV210_UFCON_DEFAULT	(S3C2410_UFCON_FIFOMODE |	\
				 S5PV210_UFCON_TXTRIG4 |	\
				 S5PV210_UFCON_RXTRIG4)

extern void s5pv210_reserve_bootmem(void);
extern void s3c_sdhci_set_platdata(void);

static struct s3c2410_uartcfg sv210_uartcfgs[] __initdata = {
	[0] = {
		.hwport		= 0,
		.flags		= 0,
		.ucon		= S5PV210_UCON_DEFAULT,
		.ulcon		= S5PV210_ULCON_DEFAULT,
		.ufcon		= S5PV210_UFCON_DEFAULT,
	},
	[1] = {
		.hwport		= 1,
		.flags		= 0,
		.ucon		= S5PV210_UCON_DEFAULT,
		.ulcon		= S5PV210_ULCON_DEFAULT,
		.ufcon		= S5PV210_UFCON_DEFAULT,
	},
	[2] = {
		.hwport		= 2,
		.flags		= 0,
		.ucon		= S5PV210_UCON_DEFAULT,
		.ulcon		= S5PV210_ULCON_DEFAULT,
		.ufcon		= S5PV210_UFCON_DEFAULT,
	},
	[3] = {
		.hwport		= 3,
		.flags		= 0,
		.ucon		= S5PV210_UCON_DEFAULT,
		.ulcon		= S5PV210_ULCON_DEFAULT,
		.ufcon		= S5PV210_UFCON_DEFAULT,
	},
};

#ifdef CONFIG_TOUCHSCREEN_S3C
static struct s3c_ts_mach_info s3c_ts_platform __initdata = {
	.delay                  = 10000,
	.presc                  = 49,
	.oversampling_shift     = 2,
	.resol_bit              = 12,
	.s3c_adc_con            = ADC_TYPE_2,
};
#endif

#ifdef CONFIG_S5PV210_ADC
static struct s3c_adc_mach_info s3c_adc_platform __initdata = {
	/* s5pc100 supports 12-bit resolution */
	.delay  = 10000,
	.presc  = 49,
	.resolution = 12,
};
#endif

#ifdef CONFIG_VIDEO_FIMC
/*
 * External camera reset
 * Because the most of cameras take i2c bus signal, so that
 * you have to reset at the boot time for other i2c slave devices.
 * This function also called at fimc_init_camera()
 * Do optimization for cameras on your platform.
*/
static int sv210_cam0_power(int onoff)
{
	//printk(KERN_ERR "%s %d\n", __func__, onoff);
	/* Camera A */

#if 1	
	gpio_request(S5PV210_GPH3(2), "GPH3_2"); //power
	s3c_gpio_setpull(S5PV210_GPH3(2), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPH3(2), "GPH3_2");

	gpio_request(S5PV210_GPH0(2), "GPH0_2"); //reset
	s3c_gpio_setpull(S5PV210_GPH0(2), S3C_GPIO_PULL_NONE);
	gpio_direction_output(S5PV210_GPH0(2), "GPH0_2");

	if(onoff)
	{
	    gpio_set_value(S5PV210_GPH3(2), 1);
	    mdelay(10);

		//reset
		gpio_set_value(S5PV210_GPH0(2), 1);
		udelay(200);
		gpio_set_value(S5PV210_GPH0(2), 0);
		udelay(200);
		gpio_set_value(S5PV210_GPH0(2), 1);
	}
	else
	{
        gpio_set_value(S5PV210_GPH3(2), 0);
	}

	gpio_free(S5PV210_GPH0(2));
	gpio_free(S5PV210_GPH3(2));


#endif	
	return 0;
}

static struct po1150k_platform_data po1150k_plat = {
   .default_width = 640,
   .default_height = 480,
   .freq = 24000000,
   .is_mipi = 0,
};

#define CAMERA_WRITE_ADDRESS    (0xE8)
#define CAMERA_READ_ADDRESS     (0xE9)
static struct i2c_board_info po1150k_i2c_info = {
   I2C_BOARD_INFO("po1150k", 0xE8>>1),
   .platform_data = &po1150k_plat,
};

static struct s3c_platform_camera po1150k = {
   .id     = CAMERA_PAR_A,
   .type       = CAM_TYPE_ITU,
   .fmt        = ITU_601_YCBCR422_8BIT,
   .order422   = CAM_ORDER422_8BIT_CBYCRY,

   .i2c_busnum = 2,
   .info       = &po1150k_i2c_info,
   .pixelformat    = V4L2_PIX_FMT_VYUY,

   .srclk_name = "mout_mpll",
   .clk_name   = "sclk_cam0",
   .clk_rate   = 24000000,

   .line_length    = 1920,

   .width      = 640,
   .height     = 480,
   .window     = {
       .left   = 0,
       .top    = 0,
       .width  = 640,
       .height = 480,
   },

   /* Polarity */
   .inv_pclk   = 0,
   .inv_vsync  = 1,
   .inv_href   = 0,
   .inv_hsync  = 0,

   .initialized    = 0,
   .cam_power  = sv210_cam0_power,
};

/* Interface setting */
static struct s3c_platform_fimc fimc_plat = {
	.srclk_name	= "mout_mpll",
	.clk_name	= "fimc",
	.clk_rate	= 166000000,

	.default_cam    = CAMERA_PAR_A,
	.camera     = {
		&po1150k,
	},

	.hw_ver		= 0x43,
};
#endif

#ifdef CONFIG_LEDS_GPIO
/* gpio led */
static struct gpio_led sv210_debug_leds[] = {
    [0] = {
        .name           = "cpu:led",
        .default_trigger    = "cpu",
        .gpio = S5PV210_GPH0(4),
        .active_low = 0,
    },
    [1] = {
        .name           = "timer:led",
        .default_trigger    = "timer",
        .gpio = S5PV210_GPH0(5),
        .active_low = 0,
    },
};

static struct gpio_led_platform_data sv210_debug_leds_info = {
    .leds       = sv210_debug_leds,
    .num_leds   = ARRAY_SIZE(sv210_debug_leds),
};

static struct platform_device sv210_device_leds = {
    .name       = "leds-gpio",
    .id     = -1,
    .dev        = {
        .platform_data = &sv210_debug_leds_info,
    }
};

static void __init sv210_init_leds(void)
{
    sv210_debug_leds[0].gpio = S5PV210_GPH0(4);
    sv210_debug_leds[1].gpio = S5PV210_GPH0(5);

    platform_device_register(&sv210_device_leds);
}
#endif

#if defined(CONFIG_HAVE_PWM)
static struct platform_pwm_backlight_data smdk_backlight_data = {
	.pwm_id  = 2,
	.max_brightness = 255,
	.dft_brightness = 255,
	//.pwm_period_ns  = 78770,
	.pwm_period_ns  = 35000,
};

static struct platform_device smdk_backlight_device = {
	.name      = "pwm-backlight",
	.id        = -1,
	.dev        = {
		.parent = &s3c_device_timer[2].dev,
		.platform_data = &smdk_backlight_data,
	},
};
static void __init smdk_backlight_register(void)
{
	int ret = platform_device_register(&smdk_backlight_device);
	if (ret)
		printk(KERN_ERR "smdk: failed to register backlight device: %d\n", ret);
}
#endif

#include <linux/max17040_battery.h>
static int act8937_charger_enable(void)
{
/*	
    gpio_set_value(GPIO_CHARGER_ENABLE, 0);
    gpio_direction_output(GPIO_CHARGER_STATUS, 0);
    gpio_direction_input(GPIO_CHARGER_STATUS);

    return  gpio_get_value(GPIO_CHARGER_STATUS) ? 0 : 1;
*/
    return 1;
}

static void act8937_charger_disable(void)
{
	//reserve
}

static int act8937_charger_done(void)
{
    //reserve
    return  0;
}

static int act8937_charger_online(void)
{
    return  gpio_get_value(GPIO_CHARGER_ONLINE) ? 1 : 0;
}

static int act8937_battery_online(void)
{
    /*think that  battery always is inserted */
    return 1;
}

static struct max17040_platform_data max17040_platform_data = {
    .charger_enable = act8937_charger_enable,
    .charger_online = act8937_charger_online,
    .battery_online = act8937_battery_online,
    .charger_done = act8937_charger_done,
    .charger_disable = act8937_charger_disable,
};

/* I2C0 */
static struct i2c_board_info i2c_devs0[] __initdata = {
    {
        I2C_BOARD_INFO("bma150", 0x38),
    },
    {
        I2C_BOARD_INFO("hmc5843", 0x1E),
    },
//Added by SOO
    {
        I2C_BOARD_INFO("dummy_i2c_dev", 0x1F),
    },
};

/* I2C1 */
static struct i2c_board_info i2c_devs1[] __initdata = {
#ifdef CONFIG_VIDEO_TV20
	{
		I2C_BOARD_INFO("s5p_ddc", (0x74>>1)),
	},
#endif
};

/* I2C2 */
static struct i2c_board_info i2c_devs2[] __initdata = {
#ifdef CONFIG_REGULATOR_ACT8937
    {
        I2C_BOARD_INFO("act8937", 0x5B),
    },
#endif
#ifdef CONFIG_BATTERY_MAX17040	
    {
        I2C_BOARD_INFO("max17040", 0x36),
		.platform_data = &max17040_platform_data,
    },
#endif
#ifdef CONFIG_SND_SV210_RT5621
    {
        I2C_BOARD_INFO("rt5621", 0x1a),
    },
#endif
};

#ifdef CONFIG_USB_SUPPORT
static void __init sv210_usb_host_set(void)
{
    int err;

    err = gpio_request(S5PV210_ETC2(7), "ETC2");
    if (err)
        printk(KERN_ERR "#### failed to request ETC2 for USB host\n");

    s3c_gpio_setpull(S5PV210_ETC2(7), S3C_GPIO_PULL_DOWN);
    gpio_free(S5PV210_ETC2(7));
}
#endif

#ifdef CONFIG_ANDROID_PMEM
static struct android_pmem_platform_data pmem_pdata = {
	.name = "pmem",
	.no_allocator = 1,
	.cached = 1,
	.start = 0, // will be set during proving pmem driver.
	.size = 0 // will be set during proving pmem driver.
};

static struct android_pmem_platform_data pmem_gpu1_pdata = {
   .name = "pmem_gpu1",
   .no_allocator = 1,
   .cached = 1,
   .buffered = 1,
   .start = 0,
   .size = 0,
};

static struct android_pmem_platform_data pmem_adsp_pdata = {
   .name = "pmem_adsp",
   .no_allocator = 1,
   .cached = 1,
   .buffered = 1,
   .start = 0,
   .size = 0,
};

static struct platform_device pmem_device = {
   .name = "android_pmem",
   .id = 0,
   .dev = { .platform_data = &pmem_pdata },
};

static struct platform_device pmem_gpu1_device = {
	.name = "android_pmem",
	.id = 1,
	.dev = { .platform_data = &pmem_gpu1_pdata },
};

static struct platform_device pmem_adsp_device = {
	.name = "android_pmem",
	.id = 2,
	.dev = { .platform_data = &pmem_adsp_pdata },
};

static void __init android_pmem_set_platdata(void)
{
	pmem_pdata.start = (u32)s3c_get_media_memory_bank(S3C_MDEV_PMEM, 0);
	pmem_pdata.size = (u32)s3c_get_media_memsize_bank(S3C_MDEV_PMEM, 0);

	pmem_gpu1_pdata.start = (u32)s3c_get_media_memory_bank(S3C_MDEV_PMEM_GPU1, 0);
	pmem_gpu1_pdata.size = (u32)s3c_get_media_memsize_bank(S3C_MDEV_PMEM_GPU1, 0);

	pmem_adsp_pdata.start = (u32)s3c_get_media_memory_bank(S3C_MDEV_PMEM_ADSP, 0);
	pmem_adsp_pdata.size = (u32)s3c_get_media_memsize_bank(S3C_MDEV_PMEM_ADSP, 0);
}
#endif
struct platform_device sec_device_battery = {
	.name	= "sv210-vbat",
	.id		= -1,
};

static struct platform_device sv210_rfkill = {
    .name = "sv210_rfkill",
    .id = -1,
};

static struct platform_device sv210_sysfs = {
    .name = "sv210-sysfs",
    .id = -1,
};

/*led test*/
static struct platform_device sv210_led = {
    .name = "sv210_led",
    .id   = -1,
};

#ifdef CONFIG_EX_CLCD
static struct platform_device ex_clcd = {
    .name = "ex_clcd",
    .id   = -1,
};
#endif
#ifdef CONFIG_EX_DOT
static struct platform_device ex_dot = {
    .name = "ex_dot",
    .id   = -1,
};
#endif
#ifdef CONFIG_EX_FND
static struct platform_device ex_fnd = {
    .name = "ex_fnd",
    .id   = -1,
};
#endif
#ifdef CONFIG_EX_KEYLED
static struct platform_device ex_keyled = {
    .name = "ex_keyled",
    .id   = -1,
};
#endif
#ifdef CONFIG_EX_STEPMOTOR
static struct platform_device ex_stepmotor = {
    .name = "ex_stepmotor",
    .id   = -1,
};
#endif


static struct sv210_keys_button sv210_gpio_keys_table[] = {
    {
        .code           = KEY_BACK,
        .gpio           = S5PV210_GPH1(0),
        .active_low     = 1,
        .desc           = "BACK",
        .type           = EV_KEY,
        .wakeup         = 1,
        .irq            = IRQ_EINT8,
        .config         = (0xf << (4 * 0)),
        .pull           = S3C_GPIO_PULL_NONE,
    },
    {
        .code           = KEY_MENU,
        .gpio           = S5PV210_GPH1(1),
        .active_low     = 1,
        .desc           = "MENU",
        .type           = EV_KEY,
        .wakeup         = 1,
        .irq            = IRQ_EINT9,
        .config         = (0xf<< (4 * 1)),
        .pull           = S3C_GPIO_PULL_NONE,
    },
    {
        .code           = KEY_ENTER,
        .gpio           = S5PV210_GPH1(2),
        .active_low     = 1,
        .desc           = "ENTER",
        .type           = EV_KEY,
        .wakeup         = 1,
        .irq            = IRQ_EINT10,
        .config         = (0xf<< (4 * 1)),
        .pull           = S3C_GPIO_PULL_NONE,
    },
    {
        .code           = KEY_HOME,
        .gpio           = S5PV210_GPH1(3),
        .active_low     = 1,
        .desc           = "HOME",
        .type           = EV_KEY,
        .wakeup         = 1,
        .irq            = IRQ_EINT11,
        .config         = (0xf<< (4 * 1)),
        .pull           = S3C_GPIO_PULL_NONE,
    },

    {
        .code           = KEY_VOLUMEDOWN,
        .gpio           = S5PV210_GPH1(6),
        .active_low     = 0,
        .desc           = "VOLUMEDOWN",
        .type           = EV_KEY,
        .wakeup         = 1,
        .irq            = IRQ_EINT14,
        .config         = (0xf<< (4 * 6)),
        .pull           = S3C_GPIO_PULL_NONE,
    },
    {
        .code           = KEY_SEARCH,
        .gpio           = S5PV210_GPH1(7),
        .active_low     = 0,
        .desc           = "SEARCH",
        .type           = EV_KEY,
        .wakeup         = 1,
        .irq            = IRQ_EINT15,
        .config         = (0xf<< (4 * 7)),
        .pull           = S3C_GPIO_PULL_NONE,
    },
    {
        .code           = KEY_VOLUMEUP,
        .gpio           = S5PV210_GPH2(0),
        .active_low     = 0,
        .desc           = "VOLUMEUP",
        .type           = EV_KEY,
        .wakeup         = 1,
        .irq            = IRQ_EINT(16),
        .config         = (0xf<< (4 * 0)),
        .pull           = S3C_GPIO_PULL_NONE,
    },
};

static struct sv210_keys_platform_data sv210_gpio_keys_data = {
    .buttons    = sv210_gpio_keys_table,
    .nbuttons   = ARRAY_SIZE(sv210_gpio_keys_table),
    .rep        = 0,
};

static struct platform_device sv210_device_gpiokeys = {
    .name   = "sv210_gpiokeys",
    .dev    = {
        .platform_data  = &sv210_gpio_keys_data,
    },
};

static struct platform_device *sv210_devices[] __initdata = {
#ifdef CONFIG_MTD_NAND
	&s3c_device_nand,
#endif
#ifdef CONFIG_FB_S3C
	&s3c_device_fb,
#endif

#ifdef CONFIG_TOUCHSCREEN_S3C
	&s3c_device_ts,
#endif
#ifdef CONFIG_S5PV210_ADC
	&s3c_device_adc,
#endif
#ifdef CONFIG_S3C2410_WATCHDOG
	&s3c_device_wdt,
#endif
#ifdef CONFIG_RTC_DRV_S3C
	&s5p_device_rtc,
#endif
#ifdef CONFIG_HAVE_PWM
	&s3c_device_timer[2],
#endif
#ifdef CONFIG_SND_S3C24XX_SOC
	&s3c64xx_device_iis0,
#endif
#ifdef CONFIG_SND_S3C_SOC_PCM
	&s3c64xx_device_pcm1,
#endif
#ifdef CONFIG_SND_S5P_SOC_SPDIF
	&s5p_device_spdif,
#endif
#ifdef CONFIG_VIDEO_FIMC
	&s3c_device_fimc0,
	&s3c_device_fimc1,
	&s3c_device_fimc2,
#endif
#ifdef CONFIG_VIDEO_MFC50
	&s3c_device_mfc,
#endif

#ifdef CONFIG_VIDEO_JPEG_V2
	&s3c_device_jpeg,
#endif

#ifdef CONFIG_VIDEO_ROTATOR
	&s5p_device_rotator,
#endif
	&s3c_device_i2c0,
	&s3c_device_i2c1,
	&s3c_device_i2c2,

#ifdef CONFIG_USB
	&s3c_device_usb_ehci,
	&s3c_device_usb_ohci,
#endif
#ifdef CONFIG_USB_GADGET
	&s3c_device_usbgadget,
#endif
#ifdef CONFIG_USB_ANDROID
	&s3c_device_android_usb,
	&s3c_device_usb_mass_storage,
#endif

#ifdef CONFIG_S3C_DEV_HSMMC
	&s3c_device_hsmmc0,
#endif
#ifdef CONFIG_S3C_DEV_HSMMC1
	&s3c_device_hsmmc1,
#endif
#ifdef CONFIG_S3C_DEV_HSMMC2
	&s3c_device_hsmmc2,
#endif
#ifdef CONFIG_S3C_DEV_HSMMC3
	&s3c_device_hsmmc3,
#endif

#ifdef CONFIG_VIDEO_TV20
	&s5p_device_tvout,
	&s5p_device_cec,
	&s5p_device_hpd,
#endif

#ifdef CONFIG_ANDROID_PMEM
	&pmem_device,
	&pmem_gpu1_device,
	&pmem_adsp_device,
#endif

#ifdef CONFIG_BATTERY_SV210
	&sec_device_battery,
#endif

#ifdef CONFIG_VIDEO_G2D
	&s5p_device_g2d,
#endif
	&sv210_rfkill,
	&sv210_sysfs,
	&sv210_led,

#ifdef CONFIG_EX_CLCD
	&ex_clcd,
#endif
#ifdef CONFIG_EX_DOT
	&ex_dot,
#endif
#ifdef CONFIG_EX_FND
	&ex_fnd,
#endif
#ifdef CONFIG_EX_KEYLED
	&ex_keyled,
#endif
#ifdef CONFIG_EX_STEPMOTOR
	&ex_stepmotor,
#endif

    &sv210_device_gpiokeys,

};

static void __init sv210_fixup(struct machine_desc *desc,
                                       struct tag *tags, char **cmdline,
                                       struct meminfo *mi)
{
    mi->bank[0].start = 0x20000000;
    mi->bank[0].size = 256 * SZ_1M;
    mi->bank[0].node = 0;

    mi->bank[1].start = 0x40000000;
    mi->bank[1].size = 256 * SZ_1M;
    mi->bank[1].node = 1;

    mi->nr_banks = 2;
}

static void __init sv210_map_io(void)
{
	s5p_init_io(NULL, 0, S5P_VA_CHIPID);
	s3c24xx_init_clocks(24000000);
	s3c24xx_init_uarts(sv210_uartcfgs, ARRAY_SIZE(sv210_uartcfgs));
	s5pv210_reserve_bootmem();

#ifdef CONFIG_MTD_NAND
	s3c_device_nand.name = "s5pv210-nand";
#endif
}

#ifdef CONFIG_S3C_SAMSUNG_PMEM
static void __init s3c_pmem_set_platdata(void)
{
	pmem_pdata.start = s3c_get_media_memory_bank(S3C_MDEV_PMEM, 1);
	pmem_pdata.size = s3c_get_media_memsize_bank(S3C_MDEV_PMEM, 1);
}
#endif

#ifdef CONFIG_FB_S3C_LTE480WV
static struct s3c_platform_fb lte480wv_fb_data __initdata = {
	.hw_ver	= 0x62,
	.nr_wins = 5,
	.default_win = CONFIG_FB_S3C_DEFAULT_WINDOW,
	.swap = FB_SWAP_WORD | FB_SWAP_HWORD,
};
#endif
/* this function are used to detect s5pc110 chip version temporally */

int s5pc110_version ;

void _hw_version_check(void)
{
	void __iomem * phy_address ;
	int temp; 

	phy_address = ioremap (0x40,1);

	temp = __raw_readl(phy_address);


	if (temp == 0xE59F010C)
	{
		s5pc110_version = 0;
	}
	else
	{
		s5pc110_version=1 ;
	}
	printk("S5PC110 Hardware version : EVT%d \n",s5pc110_version);
	
	iounmap(phy_address);
}

/* Temporally used
 * return value 0 -> EVT 0
 * value 1 -> evt 1
 */

int hw_version_check(void)
{
	return s5pc110_version ;
}
EXPORT_SYMBOL(hw_version_check);

extern int Display_On(int on);
extern int s3cfb_backlight_off(struct platform_device *pdev);
static void sv210_power_off(void)
{
	/* PS_HOLD --> Output Low */
	printk(KERN_EMERG "%s : setting GPIO_PDA_PS_HOLD low.\n", __func__);
	/* PS_HOLD output High --> Low  PS_HOLD_CONTROL, R/W, 0xE010_E81C */
	writel(readl(S5P_PSHOLD_CONTROL) & 0xFFFFFEFF, S5P_PSHOLD_CONTROL);

	//LCD OFF
	s3cfb_backlight_off(NULL);
	Display_On(0);
	
	while(1);

	printk(KERN_EMERG "%s : should not reach here!\n", __func__);
}

void s3c_setup_uart_cfg_gpio(unsigned char port)
{
    switch(port)
    {
    case 0:
        s3c_gpio_cfgpin(GPIO_GPS_RXD, S3C_GPIO_SFN(GPIO_GPS_RXD_AF));
        s3c_gpio_setpull(GPIO_GPS_RXD, S3C_GPIO_PULL_NONE);
        s3c_gpio_cfgpin(GPIO_GPS_TXD, S3C_GPIO_SFN(GPIO_GPS_TXD_AF));
        s3c_gpio_setpull(GPIO_GPS_TXD, S3C_GPIO_PULL_NONE);
        break;
    case 1:
        s3c_gpio_cfgpin(GPIO_BT_RXD, S3C_GPIO_SFN(GPIO_BT_RXD_AF));
        s3c_gpio_setpull(GPIO_BT_RXD, S3C_GPIO_PULL_UP);
        s3c_gpio_cfgpin(GPIO_BT_TXD, S3C_GPIO_SFN(GPIO_BT_TXD_AF));
        s3c_gpio_setpull(GPIO_BT_TXD, S3C_GPIO_PULL_NONE);
        s3c_gpio_cfgpin(GPIO_BT_CTS, S3C_GPIO_SFN(GPIO_BT_CTS_AF));
        s3c_gpio_setpull(GPIO_BT_CTS, S3C_GPIO_PULL_NONE);
        s3c_gpio_cfgpin(GPIO_BT_RTS, S3C_GPIO_SFN(GPIO_BT_RTS_AF));
        s3c_gpio_setpull(GPIO_BT_RTS, S3C_GPIO_PULL_NONE);
        break;
    case 2:
        s3c_gpio_cfgpin(GPIO_EXT_RXD, S3C_GPIO_SFN(GPIO_EXT_RXD_AF));
        s3c_gpio_setpull(GPIO_EXT_RXD, S3C_GPIO_PULL_NONE);
        s3c_gpio_cfgpin(GPIO_EXT_TXD, S3C_GPIO_SFN(GPIO_EXT_TXD_AF));
        s3c_gpio_setpull(GPIO_EXT_TXD, S3C_GPIO_PULL_NONE);
        break;
    case 3:
        s3c_gpio_cfgpin(GPIO_DEBUG_RXD, S3C_GPIO_SFN(GPIO_DEBUG_RXD_AF));
        s3c_gpio_setpull(GPIO_DEBUG_RXD, S3C_GPIO_PULL_NONE);
        s3c_gpio_cfgpin(GPIO_DEBUG_TXD, S3C_GPIO_SFN(GPIO_DEBUG_TXD_AF));
        s3c_gpio_setpull(GPIO_DEBUG_TXD, S3C_GPIO_PULL_NONE);
        break;
    default:
        break;
    }
}

#ifdef CONFIG_VIDEO_TV20
void s3c_set_qos()
{
    /* VP QoS */
    __raw_writel(0x00400001, S5P_VA_DMC0 + 0xC8);
    __raw_writel(0x387F0022, S5P_VA_DMC0 + 0xCC);
    /* MIXER QoS */
    __raw_writel(0x00400001, S5P_VA_DMC0 + 0xD0);
    __raw_writel(0x3FFF0062, S5P_VA_DMC0 + 0xD4);
    /* LCD1 QoS */
    __raw_writel(0x00800001, S5P_VA_DMC1 + 0x90);
    __raw_writel(0x3FFF005B, S5P_VA_DMC1 + 0x94);
    /* LCD2 QoS */
    __raw_writel(0x00800001, S5P_VA_DMC1 + 0x98);
    __raw_writel(0x3FFF015B, S5P_VA_DMC1 + 0x9C);
    /* VP QoS */
    __raw_writel(0x00400001, S5P_VA_DMC1 + 0xC8);
    __raw_writel(0x387F002B, S5P_VA_DMC1 + 0xCC);
    /* DRAM Controller QoS */
    __raw_writel((__raw_readl(S5P_VA_DMC0)&~(0xFFF<<16)|(0x100<<16)),
            S5P_VA_DMC0 + 0x0);
    __raw_writel((__raw_readl(S5P_VA_DMC1)&~(0xFFF<<16)|(0x100<<16)),
            S5P_VA_DMC1 + 0x0);
    /* BUS QoS AXI_DSYS Control */
    __raw_writel(0x00000007, S5P_VA_BUS_AXI_DSYS + 0x400);
    __raw_writel(0x00000007, S5P_VA_BUS_AXI_DSYS + 0x420);
    __raw_writel(0x00000030, S5P_VA_BUS_AXI_DSYS + 0x404);
    __raw_writel(0x00000030, S5P_VA_BUS_AXI_DSYS + 0x424);
}
#endif

#define GPH0CON (*(unsigned long *)S5PV210_GPH0CON)
#define GPH0PUD (*(unsigned long *)S5PV210_GPH0PUD)
#define GPH0DAT (*(unsigned long *)S5PV210_GPH0DAT)

#define GPH1CON (*(unsigned long *)S5PV210_GPH1CON)
#define GPH1PUD (*(unsigned long *)S5PV210_GPH1PUD)
#define GPH1DAT (*(unsigned long *)S5PV210_GPH1DAT)

#define GPH2CON (*(unsigned long *)S5PV210_GPH2CON)
#define GPH2PUD (*(unsigned long *)S5PV210_GPH2PUD)
#define GPH2DAT (*(unsigned long *)S5PV210_GPH2DAT)

#define GPH3CON (*(unsigned long *)S5PV210_GPH3CON)
#define GPH3PUD (*(unsigned long *)S5PV210_GPH3PUD)
#define GPH3DAT (*(unsigned long *)S5PV210_GPH3DAT)

static void __init sv210_machine_init(void)
{
	/* Find out S5PC110 chip version */
	_hw_version_check();

/*---------------------------------------------------------------------*/
//extern gpio GPH0_1
    GPH0CON &= (~0x0000000F); GPH0CON |= 0x00000001;
    GPH0PUD &= (~0x0000000C); GPH0PUD |= 0x00000008;
    GPH0DAT |= 0x00000002;

//extern gpio GPH2_7
    GPH2CON &= (~0xF0000000); GPH2CON |= 0x10000000;
    GPH2PUD &= (~0x0000C000); GPH2PUD |= 0x00008000;
    GPH2DAT |=  0x00000080;

//CMOS POWER OFF GPH3_2
    GPH3CON &= (~0x00000F00); GPH3CON |= 0x00000100;
    GPH3PUD &= (~0x00000030); GPH3PUD |= 0x00000020;
    GPH3DAT &= ~0x00000004;
//	udelay(200);
//  GPH3DAT |= 0x00000004;
//	udelay(200);

//LED ON - GPH0_4, GPH0_5, GPH0_6
    GPH0CON &= (~0x0FFF0000); GPH0CON |= 0x01110000;
    GPH0PUD &= (~0x00003F00); GPH0PUD |= 0x00002A00;
    GPH0DAT &= (~0x00000070);

//DISPLAY MODE - GPH0_7
    GPH0CON &= (~0xF0000000); GPH0CON |= 0x10000000;
    GPH0PUD &= (~0x0000C000); GPH0PUD |= 0x00008000;
    GPH0DAT |= 0x00000080;

//KEY1,2,3,4 - GPH1_0, GPH1_1, GPH1_2, GPH1_3 input
    GPH1CON &= (~0x0000FFFF);
    GPH1PUD &= (~0x000000FF); GPH1PUD |= 0x000000AA;

//MELTI KEY - GPH1_6, GPH1_7 input
    GPH1CON &= (~0xFF000000);
    GPH1PUD &= (~0x0000F000); GPH1PUD |= 0x0000A000;

//MELTI KEY - GPH2_0 input
    GPH2CON &= (~0x0000000F);
    GPH2PUD &= (~0x00000003); GPH2PUD |= 0x00000002;

//HDMI_I2C_EN - GPH2_6
    GPH2CON &= (~0x0F000000); GPH2CON |= 0x01000000;
    GPH2PUD &= (~0x00003000); GPH2PUD |= 0x00002000;
    GPH2DAT |=  0x00000040;
    //GPH2DAT &=  ~0x00000040;

//USB OTG - GPH3_0 input
    GPH3CON &= (~0x0000000F);
    GPH3PUD &= (~0x00000003); GPH3PUD |= 0x00000002;

//SD_WP - GPH3_1 input
    GPH3CON &= (~0x000000F0);
    GPH3PUD &= (~0x0000000C); GPH3PUD |= 0x00000008;

//MIPI_PWR_ON - GPH3_2
    GPH3CON &= (~0x00000F00); GPH3CON |= 0x00000100;
    GPH3PUD &= (~0x00000030); GPH3PUD |= 0x00000020;
    GPH3DAT &=  ~0x00000004;

//IRQ_AXE - GPH3_3 input
    GPH3CON &= (~0x0000F000);
    GPH3PUD &= (~0x000000c0); GPH3PUD |= 0x00000080;

//COMBO PWR - GPH2_4
    GPH2CON &= (~0x000F0000); GPH2CON |= 0x00010000;
    GPH2PUD &= (~0x00000300); GPH2PUD |= 0x00000200;
    GPH2DAT |=  0x00000010;
    mdelay(1);

//COMBO RESET - GPH3_4
    GPH3CON &= (~0x000F0000); GPH3CON |= 0x00010000;
    GPH3PUD &= (~0x00000300); GPH3PUD |= 0x00000200;
    GPH3DAT |=  0x00000010;
    GPH3DAT &= ~0x00000010;
    GPH3DAT |=  0x00000010;
    udelay(10);

//WIFI nPD - GPH3_5 - drivers loading
    GPH3CON &= (~0x00F00000); GPH3CON |= 0x00100000;
    GPH3PUD &= (~0x00000C00); GPH3PUD |= 0x00000800;
    GPH3DAT |=  0x00000020;
    GPH3DAT &= ~0x00000020;
    GPH3DAT |=  0x00000020;
    udelay(10);

	s3c_setup_uart_cfg_gpio(0);
	s3c_setup_uart_cfg_gpio(1);
	s3c_setup_uart_cfg_gpio(2);
	s3c_setup_uart_cfg_gpio(3);
/*---------------------------------------------------------------------*/

#ifdef CONFIG_MTD_NAND
	//s3c_device_nand.dev.platform_data = &s5p_nand_data;
#endif

#ifdef CONFIG_ANDROID_PMEM
	android_pmem_set_platdata();
#endif
	/* i2c */
	s3c_i2c0_set_platdata(NULL);
	s3c_i2c1_set_platdata(NULL);
	s3c_i2c2_set_platdata(NULL);
	i2c_register_board_info(0, i2c_devs0, ARRAY_SIZE(i2c_devs0));
	i2c_register_board_info(1, i2c_devs1, ARRAY_SIZE(i2c_devs1));
	i2c_register_board_info(2, i2c_devs2, ARRAY_SIZE(i2c_devs2));

	/* to support system shut down */
	pm_power_off = sv210_power_off;

#ifdef CONFIG_FB_S3C_LTE480WV
	s3cfb_set_platdata(&lte480wv_fb_data);
#endif

#if defined(CONFIG_TOUCHSCREEN_S3C)
	s3c_ts_set_platdata(&s3c_ts_platform);
#endif

#if defined(CONFIG_S5PV210_ADC)
	s3c_adc_set_platdata(&s3c_adc_platform);
#endif

#if defined(CONFIG_PM)
	s3c_pm_init();
#endif
#ifdef CONFIG_VIDEO_FIMC
	/* fimc */
	s3c_fimc0_set_platdata(&fimc_plat);
	s3c_fimc1_set_platdata(&fimc_plat);
	s3c_fimc2_set_platdata(&fimc_plat);
#endif

#ifdef CONFIG_VIDEO_MFC50
	/* mfc */
	s3c_mfc_set_platdata(NULL);
#endif

#ifdef CONFIG_VIDEO_TV20
    s3c_set_qos();
#endif

#ifdef CONFIG_S3C_DEV_HSMMC
	s5pv210_default_sdhci0();
#endif
#ifdef CONFIG_S3C_DEV_HSMMC1
	s5pv210_default_sdhci1();
#endif
#ifdef CONFIG_S3C_DEV_HSMMC2
	s5pv210_default_sdhci2();
#endif
#ifdef CONFIG_S3C_DEV_HSMMC3
	s5pv210_default_sdhci3();
#endif
#ifdef CONFIG_S5PV210_SETUP_SDHCI
	s3c_sdhci_set_platdata();
#endif
	platform_add_devices(sv210_devices, ARRAY_SIZE(sv210_devices));
#if defined(CONFIG_HAVE_PWM)
	smdk_backlight_register();
#endif
#ifdef CONFIG_LEDS_GPIO
	sv210_init_leds();
#endif
#ifdef CONFIG_USB_SUPPORT
    sv210_usb_host_set();
#endif
}

#ifdef CONFIG_USB_SUPPORT
/* Initializes OTG Phy. */
void otg_phy_init(void)
{
	__raw_writel(__raw_readl(S5P_USB_PHY_CONTROL)
		|(0x1<<0), S5P_USB_PHY_CONTROL); /*USB PHY0 Enable */
	__raw_writel((__raw_readl(S3C_USBOTG_PHYPWR)
		&~(0x3<<3)&~(0x1<<0))|(0x1<<5), S3C_USBOTG_PHYPWR);
	__raw_writel((__raw_readl(S3C_USBOTG_PHYCLK)
		&~(0x5<<2))|(0x3<<0), S3C_USBOTG_PHYCLK);
	__raw_writel((__raw_readl(S3C_USBOTG_RSTCON)
		&~(0x3<<1))|(0x1<<0), S3C_USBOTG_RSTCON);
	udelay(10);
	__raw_writel(__raw_readl(S3C_USBOTG_RSTCON)
		&~(0x7<<0), S3C_USBOTG_RSTCON);
	udelay(10);
}
EXPORT_SYMBOL(otg_phy_init);

/* USB Control request data struct must be located here for DMA transfer */
struct usb_ctrlrequest usb_ctrl __attribute__((aligned(8)));

/* OTG PHY Power Off */
void otg_phy_off(void)
{
	__raw_writel(__raw_readl(S3C_USBOTG_PHYPWR)
		|(0x3<<3), S3C_USBOTG_PHYPWR);
	__raw_writel(__raw_readl(S5P_USB_PHY_CONTROL)
		&~(1<<0), S5P_USB_PHY_CONTROL);
}
EXPORT_SYMBOL(otg_phy_off);

void usb_host_phy_off(void)
{
    __raw_writel(__raw_readl(S3C_USBOTG_PHYPWR)
        |(0x1<<7)|(0x1<<6), S3C_USBOTG_PHYPWR);
    __raw_writel(__raw_readl(S5P_USB_PHY_CONTROL)
        &~(1<<1), S5P_USB_PHY_CONTROL);
}
EXPORT_SYMBOL(usb_host_phy_off);

void usb_host_phy_init(void)
{
	struct clk *otg_clk;

	//otg_clk = clk_get(NULL, "usbotg");
	otg_clk = clk_get(NULL, "usbhost");
	clk_enable(otg_clk);

	if (readl(S5P_USB_PHY_CONTROL) & (0x1<<1))
	{
        usb_host_phy_off();
        udelay(10);
		//return;
	}

    __raw_writel(__raw_readl(S5P_USB_PHY_CONTROL)
        |(0x1<<1), S5P_USB_PHY_CONTROL);
    __raw_writel((__raw_readl(S3C_USBOTG_PHYPWR)
        &~(0x1<<7)&~(0x1<<6))|(0x1<<8)|(0x1<<5), S3C_USBOTG_PHYPWR);
    __raw_writel((__raw_readl(S3C_USBOTG_PHYCLK)
        &~(0x1<<7))|(0x3<<0), S3C_USBOTG_PHYCLK);
    __raw_writel((__raw_readl(S3C_USBOTG_RSTCON))
        |(0x1<<4)|(0x1<<3), S3C_USBOTG_RSTCON);
    udelay(10);
    __raw_writel(__raw_readl(S3C_USBOTG_RSTCON)
        &~(0x1<<4)&~(0x1<<3), S3C_USBOTG_RSTCON);
    udelay(10);

    writel(readl(S5P_CLKGATE_IP1)|S5P_CLKGATE_IP1_USBHOST, S5P_CLKGATE_IP1);

}
EXPORT_SYMBOL(usb_host_phy_init);
#endif

MACHINE_START(SV210, "H-AndroSV210")
	/* Maintainer: newboder <newboder@hybus.net> */
	.phys_io	= S3C_PA_UART & 0xfff00000,
	.io_pg_offst	= (((u32)S3C_VA_UART) >> 18) & 0xfffc,
	.boot_params	= S5P_PA_SDRAM + 0x100,
	.init_irq	= s5pv210_init_irq,
//    .fixup      = sv210_fixup,
	.map_io		= sv210_map_io,
	.init_machine	= sv210_machine_init,
	.timer		= &s5p_systimer,
MACHINE_END
