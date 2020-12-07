int8_t spi_write(uint32_t cs, uint8_t cmd, uint8_t *addr, uint8_t *data, uint16_t len);
int8_t spi_read(uint32_t cs, uint8_t reg_addr, uint8_t *addr, uint8_t *data, uint16_t len);
void spi_begin(uint32_t cs, uint32_t max_slave_freq);
