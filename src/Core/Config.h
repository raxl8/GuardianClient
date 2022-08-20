#pragma once

#define PRODUCT_NAME "Guardian"
#define WINDOW_WIDTH 650
#define WINDOW_HEIGHT 400
#define PIN_SIZE 6

#ifndef GDN_RELEASE
#define WEBSOCKET_ENDPOINT "ws://localhost:9000"
#else
#define WEBSOCKET_ENDPOINT "wss://client.guardian.ac/"
#endif

#define WEBSOCKET_PROTOCOL_VERSION 1

#define SENTRY_URL "http://sentry.guardian.ac"
#define SENTRY_PROJECT_ID "2"
#define SENTRY_KEY "d44b37efca574c67af2c35d8b9e6e12d"
