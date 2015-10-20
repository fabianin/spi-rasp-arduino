#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <fcntl.h>
#include <cstring>
#include <cstdio>
#include <cstdio>
#include <iostream>


using namespace std;

char spiTxRxx(unsigned char txDat) {
	struct spi_ioc_transfer spi;
	memset ( &spi, 0, sizeof (spi));
	spi.tx_buf = (unsigned char) &txDat;
	spi.rx_buf = (unsigned char) &rxDat;
	spi.len = 1;

	ioctl (fd, SPI_IOC_MESSAGE(1), &spi);

	return rxDat;
}

int fd;
float enviaComando(char identificador) {
	union {
		float floatRx;
		unsigned char byteRx[4];
	} resultado;
	unsigned char byteResultado;
	bool ack;
	do {
		ack = false;
		spiTxRxx('s');
		byteResultado = spiTxRxx(identificador);
		if(byteResultado == 'a') {
			ack = true;
		}
	} while(ack == false);
	resultado.byteRx[0] = spiTxRxx(0);
	resultado.byteRx[1] = spiTxRxx(0);
	resultado.byteRx[2] = spiTxRxx(0);
	resultado.byteRx[3] = spiTxRxx(0);
	return resultado.floatRx;

}


int main() {
	int t;
	float retorno;
	fd = open("/dev/spidev0.0", O_RDWR);
	unsigned int speed = 1000000;
	ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, & speed);
	while (1) {
		retorno = enviaComando('s');
		cout << retorno << endl;
		scanf("%f", &t);
	}
	return 0;
}
