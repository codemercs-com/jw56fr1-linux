#include <stdio.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <iomanip>

#include </usr/include/libusb-1.0/libusb.h>
#include "jw56fr1.h"

int main(void)
{
	int ret = 0;

	ret = libusb_init(NULL);

	if (ret < 0)
	{
		std::cout << "libusb_init() Failed" << std::endl;
		return 1;
	}

	// find device with VID and PID
	static struct libusb_device_handle *device = NULL;

	device = libusb_open_device_with_vid_pid(NULL, CODEMERCS_VID, JW56FR1_PID);

	if (!device)
	{
		std::cout << "Can not find or open JoyWarrior56FR1 on USB" << std::endl;
		return 1;
	}

	ret = libusb_detach_kernel_driver(device, 0);

	if(ret != 0)
	{
		std::cout << "Error libusb_detach_kernel_driver(): ";
		switch(ret)
		{
			case LIBUSB_ERROR_NOT_FOUND: std::cout << "LIBUSB_ERROR_NOT_FOUND" << std::endl; break;
			case LIBUSB_ERROR_INVALID_PARAM  : std::cout << "LIBUSB_ERROR_INVALID_PARAM" << std::endl; break;
			case LIBUSB_ERROR_NO_DEVICE : std::cout << "LIBUSB_ERROR_NO_DEVICE" << std::endl; break;
			default: std::cout << "UNKNOWN" << std::endl;
		}

		libusb_close(device);
		libusb_exit(NULL);
		return 1;
	}

	ret = libusb_claim_interface(device, 0);

	if (ret != 0)
	{
		std::cout << "Error libusb_claim_interface(): ";
		switch(ret)
		{
			case LIBUSB_ERROR_NOT_FOUND: std::cout << "LIBUSB_ERROR_NOT_FOUND" << std::endl; break;
			case LIBUSB_ERROR_BUSY : std::cout << "LIBUSB_ERROR_BUSY" << std::endl; break;
			case LIBUSB_ERROR_NO_DEVICE : std::cout << "LIBUSB_ERROR_NO_DEVICE" << std::endl; break;
			default: std::cout << "UNKNOWN" << std::endl;
		}

		libusb_close(device);
		libusb_exit(NULL);
		return 1;
	}

	//Change sensor setup temporary
	uint8_t write_data[JW56FR1_WRITE_SIZE_DEFAULT] = {0};

	write_data[0] = 0x00; //report ID
	write_data[1] = JW56FR1_ARNG_RANGE_2G;	//ARng
	write_data[2] = JW56FR1_AFILT_LP1_ONLY | JW56FR1_AFILT_BAND_0 | JW56FR1_AFILT_BW_0;	//AFilt
	write_data[3] = JW56FR1_GRNG_500DPS;	//GRng
	write_data[4] = JW56FR1_GFILT_BW_0 | JW56FR1_GFILT_HP_OFF;	//GFilt
	write_data[5] = 0x00;	//unused
	write_data[6] = 0x00;	//unused
	write_data[7] = 0x00;	//unused
	write_data[8] = 0x00;	//unused


	int size = 0;
	ret = libusb_bulk_transfer(device, /*0x02*/LIBUSB_ENDPOINT_OUT | 0x02, write_data, (JW56FR1_WRITE_SIZE_DEFAULT-1), &size, 1000); //-1 to remove the reportID

	if(ret != 0)
	{
		std::cout << "Error libusb_bulk_transfer(): ";
		switch(ret)
		{
			case LIBUSB_ERROR_TIMEOUT : std::cout << "LIBUSB_ERROR_TIMEOUT" << std::endl; break;
			case LIBUSB_ERROR_PIPE : std::cout << "LIBUSB_ERROR_PIPE" << std::endl; break;
			case LIBUSB_ERROR_OVERFLOW : std::cout << "LIBUSB_ERROR_OVERFLOW" << std::endl; break;
			case LIBUSB_ERROR_NO_DEVICE : std::cout << "LIBUSB_ERROR_NO_DEVICE" << std::endl; break;
			default: std::cout << "UNKNOWN" << std::endl;
		}
	}

	//free, clean and close
	libusb_release_interface(device, 0);
	libusb_attach_kernel_driver(device, 0);
	libusb_close(device);
	libusb_exit(NULL);

	return 0;
}
