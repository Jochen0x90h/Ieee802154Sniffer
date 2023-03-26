#pragma once

#include <coco/platform/Loop_native.hpp>
#include <coco/platform/Ieee802154Radio_usb.hpp>
#include <coco/platform/File_native.hpp>
#include <coco/pcap.hpp>


using namespace coco;

constexpr int FILE_BUFFER_SIZE = sizeof(pcap::PacketHeader) + Ieee802154Radio::MAX_PAYLOAD_LENGTH;

// drivers for sniffer and pcap writing
struct Drivers {
	Loop_native loop;

	// usb
	UsbHost_native host{loop};
	UsbHost_native::Device device{host, [](const usb::DeviceDescriptor &deviceDescriptor) {
		return deviceDescriptor.idVendor == 0x1915 && deviceDescriptor.idProduct == 0x1337;
	}};
	UsbHost_native::ControlBuffer controlBuffer{device, 32};
	UsbHost_native::BulkEndpoint bulkEndpoint{device, 1};
	UsbHost_native::BulkBuffer bulkBuffer{bulkEndpoint, Ieee802154Radio::PACKET_LENGTH};

	// radio on usb (flash RadioDevice from coco-ieee802154 onto a nRF52840 dongle)
	Ieee802154Radio_usb radio{controlBuffer};
	Ieee802154Radio_usb::Node node{radio, bulkEndpoint};
	Ieee802154Radio_usb::Buffer radioBuffer{node, bulkBuffer};

	// file
	File_native file{loop};
	File_native::Buffer fileBuffer{file, FILE_BUFFER_SIZE};
};

// drivers pcap reading
struct DriversReader {
	Loop_native loop;

	// file
	File_native file{loop};
	File_native::Buffer fileBuffer{file, FILE_BUFFER_SIZE};
};
