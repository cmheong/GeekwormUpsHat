#include <unistd.h>			// close read write
#include <stdio.h>			// printf
#include <fcntl.h>			// open
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <getopt.h>


#define VREG 2
#define CREG 4
#define CFG 12 // 2010-05-13 0x0c
#define CRATE 22 // 2010-05-13 0x16
#define MODE 6
#define VER 8
#define VRST 24  // 2010-05-13 0x18
#define STAT 26  // 2010-05-13 0x1a

#define BUFSIZE	16
#define DEV "/dev/i2c-1"
#define ADRS 0x36


static int readReg(int busfd, __uint16_t reg, unsigned char *buf, int bufsize)
{
    unsigned char reg_buf[2];

    reg_buf[0] = (reg >> 0) & 0xFF;
    reg_buf[1] = (reg >> 8) & 0xFF;

    int ret = write(busfd, reg_buf, 2);

    if (ret < 0) {
        printf("Write failed trying to read reg: %04x (0x%02x 0x%02x)\n", reg, reg_buf[0], reg_buf[1], reg);
        return ret;
    }

    return read(busfd, buf, bufsize);
}

int main(int argc, char **argv)
{
    int vOpt = 0, cOpt = 0, aOpt = 0, o;
    
    while ((o = getopt (argc, argv, "vca")) != -1) {
        switch (o)
        {
        case 'v':
            vOpt = 1;
            break;
        case 'c':
            cOpt = 1;
            break;
        case 'a': // 2010-05-13 6th week of lockdown
            aOpt = 1;
            cOpt = 1;
            vOpt = 1;
            break;


        }
    }

    int bus = 1;
    unsigned char buf[BUFSIZE] = {0};

    int busfd;
    if ((busfd = open(DEV, O_RDWR)) < 0) {
        printf("can't open %s (running as root?)\n",DEV);
        return(-1);
    }

    int ret = ioctl(busfd, I2C_SLAVE, ADRS);
    if (ret < 0)
        printf("i2c device initialisation failed\n");

    if (ret < 0) return(-1);


    int hi,lo;

    readReg(busfd, MODE, buf, 2);
    hi = buf[0];
    lo = buf[1];
    if (aOpt) {
      printf("MODE %02x %02x\n",hi, lo);
    }

    readReg(busfd, CFG, buf, 2);
    hi = buf[0];
    lo = buf[1];
    if (aOpt) {
      printf("CFG %02x %02x\n",hi, lo);
    }

    readReg(busfd, CRATE, buf, 2);
    hi = buf[0];
    lo = buf[1];
    if (aOpt) {
      printf("CRATE %02x %02x\n",hi, lo);
    }

    readReg(busfd, VER, buf, 2);
    hi = buf[0];
    lo = buf[1];
    if (aOpt) {
      printf("VER %02x %02x\n",hi, lo);
    }

    readReg(busfd, STAT, buf, 2);
    hi = buf[0];
    lo = buf[1];
    if (aOpt) {
      printf("STAT %02x %02x\n",hi, lo);
    }

    readReg(busfd, VRST, buf, 2);
    hi = buf[0];
    lo = buf[1];
    if (aOpt) {
      printf("VRST %02x %02x\n",hi, lo);
    }
 
    readReg(busfd, VREG, buf, 2);
    hi = buf[0];
    lo = buf[1];
    if (aOpt) {
      printf("VCELL %02x %02x\n",hi, lo);
    }
	
    readReg(busfd, CREG, buf, 2);
    hi = buf[0];
    lo = buf[1];
    if (aOpt) {
      printf("CREG %02x %02x\n",hi, lo);
    }

    readReg(busfd, VREG, buf, 2);
    hi = buf[0];
    lo = buf[1];
    int v = (hi << 8)+lo;
    if (vOpt) {
		printf("%fV ",(((float)v)* 78.125 / 1000000.0));
	}

    if (!cOpt && !vOpt) {
		printf("%i",(int)(((float)v) / 256.0));
	}

    readReg(busfd, CREG, buf, 2);
    hi = buf[0];
    lo = buf[1];
    int w = (hi << 8)+lo;
	if (cOpt) {
		printf("%f%%",(((float)w) / 256.0));
	}
	
	printf("\n");

    close(busfd);
    return 0;

}
