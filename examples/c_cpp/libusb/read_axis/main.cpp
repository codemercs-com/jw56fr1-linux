#include <stdio.h>
#include <thread>
#include <chrono>
#include <iostream>
#include <stdio.h>
#include <iomanip>

#include </usr/include/libusb-1.0/libusb.h>
#include "kbhit.h"
#include "jw56fr1.h"

//Print out a table formated list
void print_output(Jw56fr1_data data)
{
	std::cout << "aX: " << std::right << std::setfill(' ') << std::setw(6) << data.ax << " | ";
	std::cout << "aY: "<< std::right << std::setfill(' ') << std::setw(6) << data.ay << " | ";
	std::cout << "aZ: "<< std::right << std::setfill(' ') << std::setw(6) << data.az << " | ";
	std::cout << "rX: " << std::right << std::setfill(' ') << std::setw(6) << data.rx << " | ";
	std::cout << "rY: "<< std::right << std::setfill(' ') << std::setw(6) << data.ry << " | ";
	std::cout << "rZ: "<< std::right << std::setfill(' ') << std::setw(6) << data.rz << " | ";
	std::cout << "Btn: "<< std::right << std::setfill(' ') << std::setw(6) << (data.btn & 0xff);
	std::cout << std::endl;
}

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

	uint8_t data[JW56FR1_READ_SIZE_DEFAULT] = {0};
	int size = 0;

	Jw56fr1_data fw56fr1 = {0};

	for(;;)
	{
		ret = libusb_bulk_transfer(device, /*0x81*/LIBUSB_ENDPOINT_IN | 0x01, data, (JW56FR1_READ_SIZE_DEFAULT-1), &size, 1000); //-1 to remove the reportID

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

			break; //exit for()
		}

		//build data
		fw56fr1.ax = (((data[1] << 8) | data[0]) - JW56FR1_OFFSET);
		fw56fr1.ay = (((data[3] << 8) | data[2]) - JW56FR1_OFFSET);
		fw56fr1.az = (((data[5] << 8) | data[4]) - JW56FR1_OFFSET);
		fw56fr1.rx = (((data[7] << 8) | data[6]) - JW56FR1_OFFSET);
		fw56fr1.ry = (((data[9] << 8) | data[8]) - JW56FR1_OFFSET);
		fw56fr1.rz = (((data[11] << 8) | data[10]) - JW56FR1_OFFSET);
		fw56fr1.btn = data[12];

		//print data
		print_output(fw56fr1);

		//Exit on keyboard event
		if(kbhit())
			break;

		std::this_thread::sleep_for(std::chrono::milliseconds(2));
	}

	//free, clean and close
	libusb_release_interface(device, 0);
	libusb_attach_kernel_driver(device, 0);
	libusb_close(device);
	libusb_exit(NULL);

	return 0;
}
