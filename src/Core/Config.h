#pragma once

#define PRODUCT_NAME "Guardian"
#define WINDOW_WIDTH 650
#define WINDOW_HEIGHT 400
#define PIN_SIZE 6

#ifdef _DEBUG
#define WEBSOCKET_ENDPOINT "ws://localhost:3000"
#else
#define WEBSOCKET_ENDPOINT "wss://client.guardian.ac/"
#endif

#define WEBSOCKET_PROTOCOL_VERSION 1
