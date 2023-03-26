if(${PLATFORM} STREQUAL "native")

	set(NAME "ieee-sniffer")

	add_executable(${NAME}
		ieeeSniffer.cpp
		native/ieeeSniffer.hpp
	)
	target_include_directories(${NAME}
		PRIVATE
			native/
	)

	target_link_libraries(${NAME}
		coco::coco
		coco-loop::coco-loop
		coco-device::coco-device
		coco-usb::coco-usb
		coco-ieee802154::coco-ieee802154
		coco-zigbee::coco-zigbee
		coco-file::coco-file
		coco-pcap::coco-pcap
	)

endif()
