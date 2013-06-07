
#include <string.h>
#include <avr/eeprom.h>

#define kOK 		0
#define KError 	-1

#define log_kBytes	512
#define log_kWords	(log_kBytes/2)
#define log_kErased	0xFFFF

static uint16_t* writeIndex = NULL;
static uint16_t* readIndex = NULL;


int8_t log_init(void)
{
	if(writeIndex) return kOK;
	do {
		writeIndex += sizeof(uint16_t);
		if(eeprom_read_word(writeIndex) == log_kErased) break;
		if(writeIndex >= log_kBytes) return kError;
	} while(writeIndex < log_kBytes);
	if(writeIndex >= log_kBytes) return kError;
	
}


int8_t log_write(uint16_t* data)
{

}


int8_t log_read(uint16_t* data)
{

}


int8_t log_seek(void)
{

}

