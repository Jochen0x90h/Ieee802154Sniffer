#pragma once

#include <coco/platform/Loop_native.hpp>
#include <coco/platform/Ieee802154Radio_usb.hpp>
#include <coco/platform/File_native.hpp>
#include <coco/pcap.hpp>


using namespace coco;

constexpr int FILE_BUFFER_SIZE = sizeof(pcap::PacketHeader) + Ieee802154Radio::MAX_PAYLOAD_SIZE;

// drivers for sniffer and pcap writing
struct Drivers {
	Loop_native loop;

	using UsbHost = UsbHost_native;
	using Radio = Ieee802154Radio_usb;

	// usb
	UsbHost host{loop};
	UsbHost::Device device{host, [](const usb::DeviceDescriptor &deviceDescriptor) {
		return deviceDescriptor.idVendor == 0x1915 && deviceDescriptor.idProduct == 0x1337;
	}};
	UsbHost::ControlBuffer controlBuffer{32, device};
	UsbHost::Endpoint bulkEndpoint{device, 1};
	UsbHost::Buffer bulkBuffer{Radio::BUFFER_SIZE, bulkEndpoint};

	// radio on usb (flash RadioDevice from coco-ieee802154 onto a nRF52840 dongle)
	Radio radio{controlBuffer, Radio::MAX_HEADER_SIZE};
	Radio::Node node{radio, bulkEndpoint};
	Radio::Buffer radioBuffer{node, bulkBuffer};

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
