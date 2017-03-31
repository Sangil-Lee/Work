#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include <wiringPi.h>

#define PIN_NUM 1

float ds1820_read();
int onewire_reset();
void onewire_write(uint8_t data);
void onewire_write_bit(int bit);
uint8_t onewire_read();
int onewire_read_bit();
uint8_t crc_read();
uint8_t crc_cal(uint8_t crc, uint8_t data);

int main()
{
  if(wiringPiSetup() == -1)
    return 1;

  float temp = 0.0f;

  while(1)
  {
    temp = ds1820_read();
    printf("%.1f\n", temp);

   delay(1000);
  }
}
float ds1820_read()
{
  uint8_t busy = 1;

  onewire_reset();
  onewire_write(0xCC);
  onewire_write(0x44);

  delay(750);
  while(busy == 0)
  {
    busy = onewire_read();
    printf("busy: %d\n", busy);
  }

  onewire_reset();
  onewire_write(0xCC);
  onewire_write(0xBE);

  uint8_t lsb, msb, th, tl, reserved1, reserved2, count_remain, count_per_c, crc;
  float real_temp = 0.0f;
  signed char temp_read = 0;

  lsb = onewire_read();
  msb = onewire_read();
  th = onewire_read();
  tl = onewire_read();
  reserved1 = onewire_read();
  reserved2 = onewire_read();
  count_remain = onewire_read();
  count_per_c = onewire_read();
  crc = onewire_read();

  uint8_t data[] = {lsb, msb, th, tl, reserved1, reserved2, count_remain, count_per_c};

  onewire_reset();

  if(crc_read(data) == crc)
  {
    temp_read = (signed char)(lsb>>1);

    if(msb == 255)
      temp_read = temp_read | 0x80;

    real_temp = (float)temp_read + 0.85f - (float)count_remain/(float)count_per_c;
    real_temp = (int)(real_temp * 10) / 10.0f;
  }
  else
    printf("CRC Error  ");

  return real_temp;
}

int onewire_reset()
{
  int result;

  pinMode(PIN_NUM, OUTPUT);

  digitalWrite(PIN_NUM, LOW);
  delayMicroseconds(480);

  pinMode(PIN_NUM, INPUT);
  delayMicroseconds(70);

  result = digitalRead(PIN_NUM);

  delayMicroseconds(410);

  return result;
}

void onewire_write(uint8_t data)
{
  int loop;

  for(loop=0; loop<8; loop++)
  {
    onewire_write_bit(data & 0x01);

    data >>= 1;
  }
}

void onewire_write_bit(int bit)
{
  pinMode(PIN_NUM, OUTPUT);

  if(bit)
  {
    digitalWrite(PIN_NUM, LOW);
    delayMicroseconds(6);
    digitalWrite(PIN_NUM, HIGH);
    delayMicroseconds(64);
  }
  else
  {
    digitalWrite(PIN_NUM, LOW);
    delayMicroseconds(60);
    digitalWrite(PIN_NUM, HIGH);
    delayMicroseconds(10);
  }

}

uint8_t onewire_read()
{
  int loop, result=0;

  for(loop=0; loop<8; loop++)
  {
    result >>= 1;

    if(onewire_read_bit())
      result |= 0x80;
  }

  return result;
}

int onewire_read_bit()
{
  int result;

  pinMode(PIN_NUM, OUTPUT);

  digitalWrite(PIN_NUM, LOW);
  delayMicroseconds(6);

  pinMode(PIN_NUM, INPUT);
  delayMicroseconds(9);

  result = digitalRead(PIN_NUM) & 0x01;
  delayMicroseconds(55);

  return result;
}

uint8_t crc_read(uint8_t *data)
{
 uint8_t i, crc;

 crc = 0x00;

 for(i=0; i<8; i++)
  crc = crc_cal(crc, data[i]);

 return crc;
}

uint8_t crc_cal(uint8_t crc, uint8_t data)
{
  int j;
  for(j=0;j<8;j++) {
      if ((data & 0x01 ) ^ (crc & 0x01)) {
          // DATA ^ LSB CRC = 1
          crc = crc>>1;
          // Set the MSB to 1
          crc = crc | 0x80;
          // Check bit 3
          if (crc & 0x04) {
              crc = crc & 0xFB; // Bit 3 is set, so clear it
          } else {
              crc = crc | 0x04; // Bit 3 is clear, so set it
          }
          // Check bit 4
          if (crc & 0x08) {
              crc = crc & 0xF7; // Bit 4 is set, so clear it
          } else {
              crc = crc | 0x08; // Bit 4 is clear, so set it
          }
      } else {
          // DATA ^ LSB CRC = 0
          crc = crc>>1;
          // clear MSB
          crc = crc & 0x7F;
          // No need to check bits, with DATA ^ LSB CRC = 0, they will remain unchanged
      }
      data = data>>1;
  }

  return crc;
}
