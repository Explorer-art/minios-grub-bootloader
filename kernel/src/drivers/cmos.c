/*
Original source: https://github.com/prosdev-org/PRosBSD

BSD 3-Clause License:

Copyright (c) 2025 PRosDev.org

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS “AS IS” AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/



#include <drivers/cmos.h>
#include <x86.h>
#include <utils/kpanic.h>
#include <stdint.h>

#define CMOS_ADDRESS_PORT      0x70
#define CMOS_DATA_PORT         0x71
#define CMOS_RTC_SECONDS       0x00
#define CMOS_RTC_MINUTES       0x02
#define CMOS_RTC_HOURS         0x04
#define CMOS_RTC_DAY_OF_WEEK   0x06
#define CMOS_RTC_DAY_OF_MONTH  0x07
#define CMOS_RTC_MONTH         0x08
#define CMOS_RTC_YEAR          0x09
#define CMOS_RTC_CENTURY       0x32
#define CMOS_STATUS_REGISTER_A 0x0A
#define CMOS_STATUS_REGISTER_B 0x0B

uint8_t cmos_read(const uint8_t reg) {
    outb(CMOS_ADDRESS_PORT, reg);
    return inb(CMOS_DATA_PORT);
}

void cmos_write(const uint8_t reg, const uint8_t value) {
    outb(CMOS_ADDRESS_PORT, reg);
    outb(CMOS_DATA_PORT, value);
}

static uint8_t bcd_to_binary(const uint8_t bcd) {
    return (bcd & 0x0F) + ((bcd / 16) * 10);
}

void get_rtc_time(uint8_t *seconds, uint8_t *minutes, uint8_t *hours) {
    const uint8_t regB = cmos_read(CMOS_STATUS_REGISTER_B);
    const uint8_t bcd_format = !(regB & 0x04);

    *seconds = cmos_read(CMOS_RTC_SECONDS);
    *minutes = cmos_read(CMOS_RTC_MINUTES);
    *hours = cmos_read(CMOS_RTC_HOURS);

    if (bcd_format) {
        *seconds = bcd_to_binary(*seconds);
        *minutes = bcd_to_binary(*minutes);
        *hours = bcd_to_binary(*hours);
    }

    if (*seconds > 59 || *minutes > 59 || *hours > 23) {
        kpanic("cmos: invalid time values");
    }
}

void get_rtc_date(uint8_t *day, uint8_t *month, uint8_t *year) {
    const uint8_t regB = cmos_read(CMOS_STATUS_REGISTER_B);
    const uint8_t bcd_format = !(regB & 0x04);

    *day = cmos_read(CMOS_RTC_DAY_OF_MONTH);
    *month = cmos_read(CMOS_RTC_MONTH);
    *year = cmos_read(CMOS_RTC_YEAR);

    if (bcd_format) {
        *day = bcd_to_binary(*day);
        *month = bcd_to_binary(*month);
        *year = bcd_to_binary(*year);
    }

    if (*day == 0 || *day > 31 || *month == 0 || *month > 12) {
        kpanic("cmos: invalid date values");
    }
}