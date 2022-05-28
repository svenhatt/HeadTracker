/*
 * This file is part of the Head Tracker distribution (https://github.com/dlktdr/headtracker)
 * Copyright (c) 2021 Cliff Blackburn
  *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* 4 Channels of PWM output, limited to 200Hz - 31Hz Update Rates
 *  31 Comes from coounter top max value = 32767us
 *   1/32768 = 30.5Hz
 *    */

#include "MCP492X.h"



static const nrfx_spim_t spi = NRFX_SPIM_INSTANCE(1);  /**< SPI instance. */

static volatile bool spi_xfer_done = true;  /**< Flag used to indicate that SPI instance completed the transfer. */
static const bool buffered = false;
static const bool gain = true;
static const bool active = true;

static uint8_t       m_tx_buf[2];                       /**< TX buffer. */
static const uint8_t m_length = sizeof(m_tx_buf);        /**< Transfer length. */


#define SPIM1_IRQn SPI1_TWI1_IRQn
#define ISR_PRIORITY 5

void spim_event_handler(nrfx_spim_evt_t const * p_event,
                       void *                  p_context)
{
    spi_xfer_done = true;
}

void DAC_init()
{
  nrfx_spim_config_t spi_config = NRFX_SPIM_DEFAULT_CONFIG(13, 33, 40, 23);
  spi_config.frequency = NRF_SPIM_FREQ_125K;

	uint32_t err = nrfx_spim_init(&spi, &spi_config, spim_event_handler, NULL);
	if (err != NRFX_SUCCESS) {
			return;
	}
	/* Zephyr IRQ wrapper, corresponds to NVIC_* functions */
	IRQ_CONNECT(SPIM1_IRQn, ISR_PRIORITY, nrfx_spim_1_irq_handler, NULL, 0);
}

void setDACValue(bool ch, uint16_t value)
{
    uint8_t configBits = ch << 3 | buffered << 2 | gain << 1 | active;

    m_tx_buf[0] = configBits << 4 | (value & 0xF00) >> 8;
    m_tx_buf[1] = value & 0xFF;

		nrfx_spim_xfer_desc_t xfer_desc = NRFX_SPIM_XFER_TX(m_tx_buf, m_length);
		if (spi_xfer_done)
		{
		  uint32_t ret = nrfx_spim_xfer(&spi, &xfer_desc, 0);
			if (ret == NRFX_SUCCESS) {
				spi_xfer_done = false;
			}
		}

}
