`default_nettype none

module user_project (
`ifdef USE_POWER_PINS
    inout vccd1,
    inout vssd1,
`endif

    input wire wb_clk_i,
    input wire wb_rst_i,
    input wire wbs_stb_i,
    input wire wbs_cyc_i,
    input wire wbs_we_i,
    input wire [3:0] wbs_sel_i,
    input wire [31:0] wbs_dat_i,
    input wire [31:0] wbs_adr_i,
    output wire wbs_ack_o,
    output wire [31:0] wbs_dat_o,

    output wire [2:0] user_irq,

    input wire spi0_miso,
    output wire spi0_mosi,
    output wire spi0_csb,
    output wire spi0_sclk,

    input wire spi1_miso,
    output wire spi1_mosi,
    output wire spi1_csb,
    output wire spi1_sclk,

    input wire i2c_scl_i,
    output wire i2c_scl_o,
    output wire i2c_scl_oen_o,
    input wire i2c_sda_i,
    output wire i2c_sda_o,
    output wire i2c_sda_oen_o,

    input wire [1:0] gpio_in,
    output wire [1:0] gpio_out,
    output wire [1:0] gpio_oe
);

  wire [1:0] periph_sel;
  wire spi0_stb;
  wire spi1_stb;
  wire i2c_stb;
  wire gpio_stb;

  wire [31:0] spi0_dat_o;
  wire [31:0] spi1_dat_o;
  wire [31:0] i2c_dat_o;
  wire [31:0] gpio_dat_o;

  wire spi0_ack;
  wire spi1_ack;
  wire i2c_ack;
  wire gpio_ack;

  wire spi0_irq;
  wire spi1_irq;
  wire i2c_irq;
  wire gpio_irq;

  assign periph_sel = wbs_adr_i[17:16];

  assign spi0_stb = (periph_sel == 2'b00) & wbs_stb_i;
  assign spi1_stb = (periph_sel == 2'b01) & wbs_stb_i;
  assign i2c_stb = (periph_sel == 2'b10) & wbs_stb_i;
  assign gpio_stb = (periph_sel == 2'b11) & wbs_stb_i;

  assign wbs_ack_o = spi0_ack | spi1_ack | i2c_ack | gpio_ack;

  assign wbs_dat_o = (periph_sel == 2'b00) ? spi0_dat_o :
                     (periph_sel == 2'b01) ? spi1_dat_o :
                     (periph_sel == 2'b10) ? i2c_dat_o :
                     (periph_sel == 2'b11) ? gpio_dat_o : 32'hDEADBEEF;

  assign user_irq[0] = spi0_irq;
  assign user_irq[1] = spi1_irq;
  assign user_irq[2] = i2c_irq | gpio_irq;

  wire [7:0] gpio_in_8bit;
  wire [7:0] gpio_out_8bit;
  wire [7:0] gpio_oe_8bit;

  assign gpio_in_8bit = {6'b0, gpio_in};
  assign gpio_out = gpio_out_8bit[1:0];
  assign gpio_oe = gpio_oe_8bit[1:0];

  CF_SPI_WB spi0_inst (
      .clk_i(wb_clk_i),
      .rst_i(wb_rst_i),
      .adr_i(wbs_adr_i),
      .dat_i(wbs_dat_i),
      .dat_o(spi0_dat_o),
      .sel_i(wbs_sel_i),
      .cyc_i(wbs_cyc_i),
      .stb_i(spi0_stb),
      .ack_o(spi0_ack),
      .we_i (wbs_we_i),
      .IRQ  (spi0_irq),
      .miso (spi0_miso),
      .mosi (spi0_mosi),
      .csb  (spi0_csb),
      .sclk (spi0_sclk)
  );

  CF_SPI_WB spi1_inst (
      .clk_i(wb_clk_i),
      .rst_i(wb_rst_i),
      .adr_i(wbs_adr_i),
      .dat_i(wbs_dat_i),
      .dat_o(spi1_dat_o),
      .sel_i(wbs_sel_i),
      .cyc_i(wbs_cyc_i),
      .stb_i(spi1_stb),
      .ack_o(spi1_ack),
      .we_i (wbs_we_i),
      .IRQ  (spi1_irq),
      .miso (spi1_miso),
      .mosi (spi1_mosi),
      .csb  (spi1_csb),
      .sclk (spi1_sclk)
  );

  EF_I2C_WB i2c_inst (
      .clk_i     (wb_clk_i),
      .rst_i     (wb_rst_i),
      .adr_i     (wbs_adr_i),
      .dat_i     (wbs_dat_i),
      .dat_o     (i2c_dat_o),
      .sel_i     (wbs_sel_i),
      .cyc_i     (wbs_cyc_i),
      .stb_i     (i2c_stb),
      .ack_o     (i2c_ack),
      .we_i      (wbs_we_i),
      .IRQ       (i2c_irq),
      .scl_i     (i2c_scl_i),
      .scl_o     (i2c_scl_o),
      .scl_oen_o (i2c_scl_oen_o),
      .sda_i     (i2c_sda_i),
      .sda_o     (i2c_sda_o),
      .sda_oen_o (i2c_sda_oen_o)
  );

  EF_GPIO8_WB gpio_inst (
      .clk_i (wb_clk_i),
      .rst_i (wb_rst_i),
      .adr_i (wbs_adr_i),
      .dat_i (wbs_dat_i),
      .dat_o (gpio_dat_o),
      .sel_i (wbs_sel_i),
      .cyc_i (wbs_cyc_i),
      .stb_i (gpio_stb),
      .ack_o (gpio_ack),
      .we_i  (wbs_we_i),
      .IRQ   (gpio_irq),
      .io_in (gpio_in_8bit),
      .io_out(gpio_out_8bit),
      .io_oe (gpio_oe_8bit)
  );

endmodule

`default_nettype wire
