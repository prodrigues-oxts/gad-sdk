#include <iostream>
#include <string>

#include "oxts/gal-cpp/gad.hpp"
#include "oxts/gal-cpp/gad_encoders/gad_encoder_bin.hpp"

#ifdef _WIN32
#include <windows.h>
#elif __linux__
#include <unistd.h>
#endif

namespace OxTS
{
	void sleep(int sleepMs)
	{
#ifdef __linux__
		usleep(sleepMs * 1000);   // usleep takes sleep time in us (1 millionth of a second)
#endif
#ifdef _WIN32
		Sleep(sleepMs);    // Sleep takes sleep time in ms.
#endif
	}
} // OxTSs


int main(int argc, char* argv[])
{
	int num_packets = 1000; // Total number of packets to send

	if (argc > 1)
	{
		num_packets = std::stoi(argv[1]);
	}

	//============================================================================
	// Construct the position aiding class with stream ID 129.
	OxTS::GadPosition gp = OxTS::GadPosition(129);
	// Set the aiding position
	gp.SetPosGeodetic(51.91520330, -1.24479140, 111.525);
	// Set the estimated variance on this position
	gp.SetPosGeodeticVar(1.0, 1.0, 1.0);
	// Set the time mode to Void, since we are not timestamping the aiding data.
	// With no timestamp, the INS will timestamp the data upon arrival.
	gp.SetTimeVoid();
	// Set the lever arm between the aiding source and the IMU, in the IMU frame.
	gp.SetAidingLeverArmFixed(0.5, 0.5, 1.0);
	gp.SetAidingLeverArmVar(0.1, 0.1, 0.1);
	//============================================================================
	// Initialise the encoder

	OxTS::GadEncoderBin geb = OxTS::GadEncoderBin();

	//============================================================================
	for (int i = 0; i < num_packets; ++i)
	{
		// Encode Packet
		geb.EncodePacket(gp);

		// Add your code here for interacting with the static aiding packets.
		// Use accessor functions geb.GetPacket() and geb.GetPacketSize() to send 
		// the packet via chosen UDP socket library to the INS IP address and port 
		// 50485.

		if (i % 10 == 0)
		{
			std::cout << i << " packets sent" << std::endl;
			std::cout << "packet size: " << geb.GetPacketSize() << std::endl;
		}


		OxTS::sleep(100);
	}

	return 0;
}
