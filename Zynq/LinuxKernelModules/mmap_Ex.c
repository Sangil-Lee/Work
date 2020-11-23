/*****************************************************************************/
/*                               H E A D E R                                 */
/*****************************************************************************/

// Program Name          : LED_DimmerUIO.c
// Program Type          : Linux C application program for Zynq
// Platform              : Zynq All Programmable SoC
// Board                 : ZedBoard
// Description           : Use userspace IO (UIO) device driver to control GPIO
// Date                  : 2014-05-02


/*****************************************************************************/
/*                         I N C L U D E   F I L E S                         */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


/*****************************************************************************/
/*                            D E F I N E S                                  */
/*****************************************************************************/


#define GPIO_MAP_SIZE 		0x10000
#define GPIO_DATA_OFFSET 	0x00
#define GPIO_TRI_OFFSET 	0x04
#define GPIO2_DATA_OFFSET 	0x00
#define GPIO2_TRI_OFFSET 	0x04


/*****************************************************************************/
/*                                M A I N                                    */
/*****************************************************************************/


int main(void)
{
	int 		value 		= 0;
	int 		period 		= 0;
	int 		brightness 	= 0;
	int 		fd;
	void 		*ptr;
	

	fd = open("/dev/uio0", O_RDWR);
	if (fd < 1) {
		fprintf(stderr,"Invalid UIO device file.\n");
		return -1;
	}

	// mmap the UIO device
	ptr = mmap(NULL, GPIO_MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	
	// Clear LED
	fprintf(stdout,"\nClear LEDs\n");
	*((unsigned *)(ptr + GPIO_TRI_OFFSET)) = 0;
	

	while (1) {
		fprintf(stdout,"\nSelect a Brightness between 0 and 9: ");
		scanf("%d", &value);
		if (value < 0) period = 0;
		else
		  if (value > 9) break;
		    else
		       period = value; 
		
		fprintf(stdout,"Brightness Level %d selected\n", period);
		
		//  0 turns off LEDs, 999,000 is full
		brightness = period * 110000;

		// Write the duty_cycle width out to the PL GPIO peripheral
		*((unsigned *)(ptr + GPIO_DATA_OFFSET)) = brightness;

		}

	// Unmap the address range		
	munmap(ptr, GPIO_MAP_SIZE);
	return 0;
	
}
