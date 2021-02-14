## Running this project

- Clone this repository to your local machine
- Create a file called `arduino-secrets.h` (should not be checked in to version control), and add the following two lines
	- `#define SSID "my_network_ssid"`
	- `#define PASSWORD "my_network_password"`
- Upload to your ESP32. Make sure to select the correct board type and port. You may need to hold the `BOOT` button on the physical board for uploading to work properly.
- The board will attempt to connect to the specified WiFi network and start a server on port `80`. Keep in mind that you probably only want the ESP32 visible to your local network, so do not attempt to expose it to the internet (don't port forward it).
- Debug information will be outputted to the serial channel, at `115200 baud`. Check here to find the local IP address of the board, which you can navigate to in your browser (example: `192.168.1.x`).