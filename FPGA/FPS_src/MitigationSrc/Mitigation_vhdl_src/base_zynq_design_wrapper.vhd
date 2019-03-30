--Copyright 1986-2016 Xilinx, Inc. All Rights Reserved.
----------------------------------------------------------------------------------
--Tool Version: Vivado v.2016.1 (lin64) Build 1538259 Fri Apr  8 15:45:23 MDT 2016
--Date        : Thu Jul 21 14:06:41 2016
--Host        : localhost.localdomain running 64-bit CentOS Linux release 7.2.1511 (Core)
--Command     : generate_target base_zynq_design_wrapper.bd
--Design      : base_zynq_design_wrapper
--Purpose     : IP block netlist
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
library UNISIM;
use UNISIM.VCOMPONENTS.ALL;
entity base_zynq_design_wrapper is
  port (
    DDR_addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
    DDR_ba : inout STD_LOGIC_VECTOR ( 2 downto 0 );
    DDR_cas_n : inout STD_LOGIC;
    DDR_ck_n : inout STD_LOGIC;
    DDR_ck_p : inout STD_LOGIC;
    DDR_cke : inout STD_LOGIC;
    DDR_cs_n : inout STD_LOGIC;
    DDR_dm : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dq : inout STD_LOGIC_VECTOR ( 31 downto 0 );
    DDR_dqs_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dqs_p : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_odt : inout STD_LOGIC;
    DDR_ras_n : inout STD_LOGIC;
    DDR_reset_n : inout STD_LOGIC;
    DDR_we_n : inout STD_LOGIC;
    FIXED_IO_ddr_vrn : inout STD_LOGIC;
    FIXED_IO_ddr_vrp : inout STD_LOGIC;
    FIXED_IO_mio : inout STD_LOGIC_VECTOR ( 53 downto 0 );
    FIXED_IO_ps_clk : inout STD_LOGIC;
    FIXED_IO_ps_porb : inout STD_LOGIC;
    FIXED_IO_ps_srstb : inout STD_LOGIC;
    SFP_IIC_SELECT : out STD_LOGIC_VECTOR ( 2 downto 0 );
    gpio_fan_speed_tri_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    gpio_firmware_date_tri_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    gpio_sfp_status_tri_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    sfp_iic_scl_io : inout STD_LOGIC;
    sfp_iic_sda_io : inout STD_LOGIC
  );
end base_zynq_design_wrapper;

architecture STRUCTURE of base_zynq_design_wrapper is
  component base_zynq_design is
  port (
    DDR_cas_n : inout STD_LOGIC;
    DDR_cke : inout STD_LOGIC;
    DDR_ck_n : inout STD_LOGIC;
    DDR_ck_p : inout STD_LOGIC;
    DDR_cs_n : inout STD_LOGIC;
    DDR_reset_n : inout STD_LOGIC;
    DDR_odt : inout STD_LOGIC;
    DDR_ras_n : inout STD_LOGIC;
    DDR_we_n : inout STD_LOGIC;
    DDR_ba : inout STD_LOGIC_VECTOR ( 2 downto 0 );
    DDR_addr : inout STD_LOGIC_VECTOR ( 14 downto 0 );
    DDR_dm : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dq : inout STD_LOGIC_VECTOR ( 31 downto 0 );
    DDR_dqs_n : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    DDR_dqs_p : inout STD_LOGIC_VECTOR ( 3 downto 0 );
    FIXED_IO_mio : inout STD_LOGIC_VECTOR ( 53 downto 0 );
    FIXED_IO_ddr_vrn : inout STD_LOGIC;
    FIXED_IO_ddr_vrp : inout STD_LOGIC;
    FIXED_IO_ps_srstb : inout STD_LOGIC;
    FIXED_IO_ps_clk : inout STD_LOGIC;
    FIXED_IO_ps_porb : inout STD_LOGIC;
    GPIO_SFP_STATUS_tri_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    GPIO_FIRMWARE_DATE_tri_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    GPIO_FAN_SPEED_tri_i : in STD_LOGIC_VECTOR ( 31 downto 0 );
    SFP_IIC_scl_i : in STD_LOGIC;
    SFP_IIC_scl_o : out STD_LOGIC;
    SFP_IIC_scl_t : out STD_LOGIC;
    SFP_IIC_sda_i : in STD_LOGIC;
    SFP_IIC_sda_o : out STD_LOGIC;
    SFP_IIC_sda_t : out STD_LOGIC;
    SFP_IIC_SELECT : out STD_LOGIC_VECTOR ( 2 downto 0 )
  );
  end component base_zynq_design;
  component IOBUF is
  port (
    I : in STD_LOGIC;
    O : out STD_LOGIC;
    T : in STD_LOGIC;
    IO : inout STD_LOGIC
  );
  end component IOBUF;
  signal sfp_iic_scl_i : STD_LOGIC;
  signal sfp_iic_scl_o : STD_LOGIC;
  signal sfp_iic_scl_t : STD_LOGIC;
  signal sfp_iic_sda_i : STD_LOGIC;
  signal sfp_iic_sda_o : STD_LOGIC;
  signal sfp_iic_sda_t : STD_LOGIC;
begin
base_zynq_design_i: component base_zynq_design
     port map (
      DDR_addr(14 downto 0) => DDR_addr(14 downto 0),
      DDR_ba(2 downto 0) => DDR_ba(2 downto 0),
      DDR_cas_n => DDR_cas_n,
      DDR_ck_n => DDR_ck_n,
      DDR_ck_p => DDR_ck_p,
      DDR_cke => DDR_cke,
      DDR_cs_n => DDR_cs_n,
      DDR_dm(3 downto 0) => DDR_dm(3 downto 0),
      DDR_dq(31 downto 0) => DDR_dq(31 downto 0),
      DDR_dqs_n(3 downto 0) => DDR_dqs_n(3 downto 0),
      DDR_dqs_p(3 downto 0) => DDR_dqs_p(3 downto 0),
      DDR_odt => DDR_odt,
      DDR_ras_n => DDR_ras_n,
      DDR_reset_n => DDR_reset_n,
      DDR_we_n => DDR_we_n,
      FIXED_IO_ddr_vrn => FIXED_IO_ddr_vrn,
      FIXED_IO_ddr_vrp => FIXED_IO_ddr_vrp,
      FIXED_IO_mio(53 downto 0) => FIXED_IO_mio(53 downto 0),
      FIXED_IO_ps_clk => FIXED_IO_ps_clk,
      FIXED_IO_ps_porb => FIXED_IO_ps_porb,
      FIXED_IO_ps_srstb => FIXED_IO_ps_srstb,
      GPIO_FAN_SPEED_tri_i(31 downto 0) => gpio_fan_speed_tri_i(31 downto 0),
      GPIO_FIRMWARE_DATE_tri_i(31 downto 0) => gpio_firmware_date_tri_i(31 downto 0),
      GPIO_SFP_STATUS_tri_i(31 downto 0) => gpio_sfp_status_tri_i(31 downto 0),
      SFP_IIC_SELECT(2 downto 0) => SFP_IIC_SELECT(2 downto 0),
      SFP_IIC_scl_i => sfp_iic_scl_i,
      SFP_IIC_scl_o => sfp_iic_scl_o,
      SFP_IIC_scl_t => sfp_iic_scl_t,
      SFP_IIC_sda_i => sfp_iic_sda_i,
      SFP_IIC_sda_o => sfp_iic_sda_o,
      SFP_IIC_sda_t => sfp_iic_sda_t
    );
sfp_iic_scl_iobuf: component IOBUF
     port map (
      I => sfp_iic_scl_o,
      IO => sfp_iic_scl_io,
      O => sfp_iic_scl_i,
      T => sfp_iic_scl_t
    );
sfp_iic_sda_iobuf: component IOBUF
     port map (
      I => sfp_iic_sda_o,
      IO => sfp_iic_sda_io,
      O => sfp_iic_sda_i,
      T => sfp_iic_sda_t
    );
end STRUCTURE;
