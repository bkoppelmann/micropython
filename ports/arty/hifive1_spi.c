#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "hifive1_spi.h"

#define BIT(x) (1<<(x))
#define IDLE asm volatile ("")

#define SPI_MODE0 0x00
#define SPI_MODE_CPHA1_CPOL1 0x3
#define SPI_REG(x) SPI1_REG(x)

static const uint32_t SPI_IOF_MASK = (1 << IOF_SPI1_SCK) | (1 << IOF_SPI1_MOSI) | (1 << IOF_SPI1_MISO);

typedef enum {
    SPI_CONTINUE,
    SPI_LAST
} SPITransferMode;

typedef enum {
    LSBFIRST,
    MSBFIRST
} BitOrder;


void spi_setClockDivider(uint8_t _divider) {
  SPI_REG(SPI_REG_SCKDIV) = _divider;
}

void spi_setDataMode(uint8_t _mode) {
  SPI_REG(SPI_REG_SCKMODE) = _mode;
}

void spi_setBitOrder(BitOrder _bitOrder) {
  SPI_REG(SPI_REG_FMT) = SPI_FMT_PROTO(SPI_PROTO_S) |
    SPI_FMT_ENDIAN((_bitOrder == LSBFIRST) ? SPI_ENDIAN_LSB : SPI_ENDIAN_MSB) |
    SPI_FMT_DIR(SPI_DIR_RX) |
    SPI_FMT_LEN(8);
}

uint8_t spi_transfer(uint8_t _data, SPITransferMode _mode) {

  while (SPI_REG(SPI_REG_TXFIFO) & SPI_TXFIFO_FULL) ;
  SPI_REG(SPI_REG_TXFIFO) = _data;

  volatile int32_t x;
  while ((x = SPI_REG(SPI_REG_RXFIFO)) & SPI_RXFIFO_EMPTY);
  return x & 0xFF;

  if (_mode == SPI_LAST) {
    SPI_REG(SPI_REG_CSMODE) = SPI_CSMODE_AUTO;
  }
}

int8_t spi_write(uint32_t cs, uint8_t cmd, uint8_t *addr, uint8_t *data,
                 uint16_t len)
{
    GPIO_REG(GPIO_OUTPUT_VAL) &= ~BIT(cs);
    spi_transfer(cmd, SPI_CONTINUE);

    if (addr != NULL) {
        for (int i = len-1; i >= 0; --i) {
            spi_transfer(addr[i], SPI_CONTINUE);
        }
    }
    for (int i = len-1; i > 0; --i) {
        spi_transfer(data[i], SPI_CONTINUE);
    }
    spi_transfer(data[0], SPI_LAST);
    GPIO_REG(GPIO_OUTPUT_VAL) |= BIT(cs);
    return 0;
}

int8_t spi_read(uint32_t cs, uint8_t reg_addr, uint8_t *addr, uint8_t *data,
                uint16_t len)
{
    GPIO_REG(GPIO_OUTPUT_VAL) &= ~BIT(cs);
    spi_transfer(reg_addr, SPI_CONTINUE);

    if (addr != NULL) {
        for (int i = 31; i >= 0; --i) {
            spi_transfer(addr[i], SPI_CONTINUE);
        }
    }

    for (int i = len-1; i > 0; --i) {
        data[i] = spi_transfer(reg_addr, SPI_CONTINUE);
    }
    data[0] = spi_transfer(reg_addr, SPI_LAST);
    GPIO_REG(GPIO_OUTPUT_VAL) |= BIT(cs);
    return 0;
}


void spi_begin(uint32_t cs, uint32_t max_slave_freq)
{
    GPIO_REG(GPIO_IOF_SEL) &= ~SPI_IOF_MASK;
    GPIO_REG(GPIO_IOF_EN)  |= SPI_IOF_MASK;
    GPIO_REG(GPIO_PULLUP_EN) |= (1 << PIN_12_OFFSET);

    spi_setDataMode(SPI_MODE_CPHA1_CPOL1);
    spi_setBitOrder(MSBFIRST);

    GPIO_REG(GPIO_OUTPUT_EN) |= BIT(cs);
    spi_setClockDivider((get_cpu_freq()/ max_slave_freq) - 1);
    GPIO_REG(GPIO_OUTPUT_VAL) |= BIT(cs);
}
