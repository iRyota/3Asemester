typedef unsigned char uint8_t;

int main(int argc, char* argv[])
{
	unsigned char* x = (unsigned char*) 0x24;
	unsigned char* y = (unsigned char*) 0x25;
	*x = 0b00100000;
	*y = *x;

	while(1);

	return 0;
}
