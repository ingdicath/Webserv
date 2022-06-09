//
// Created by Diani on 05/06/2022.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "Configurator.hpp"

// If exception is found, exit code is 0 for this tests.
TEST_CASE("Out of range port values", "[Config file][Port]") {
	Configurator config;
	CHECK_THROWS(config._isValidPortRange("0"));
	CHECK_THROWS(config._isValidPortRange("65536"));
}

TEST_CASE("Incorrect port values", "[Config file][Port]") {
	Configurator config;
	CHECK_THROWS(config._isValidPortRange("-152"));
	CHECK_THROWS(config._isValidPortRange("hola"));
}

TEST_CASE("Correct port values", "[Config file][Port]") {
	Configurator config;
	INFO("Correct port values");
	CHECK(config._isValidPortRange("1"));
	CHECK(config._isValidPortRange("65535"));
}

TEST_CASE("invalid port value in ip:port pair", "[Config file][ipPort]"){
	Configurator config;
	CHECK_THROWS(config._isValidIpPort("localhost:-6"));
	CHECK_THROWS(config._isValidIpPort("1.1.1.1:500000"));
}

TEST_CASE("invalid port value (single) in ip:port pair", "[Config file][ipPort]"){
	Configurator config;
	CHECK_THROWS(config._isValidIpPort("0"));
	CHECK_THROWS(config._isValidIpPort("65536"));
}

TEST_CASE("invalid address (single) in ip:port pair", "[Config file][ipPort]"){
	Configurator config;
	CHECK_THROWS(config._isValidIpPort("127.1.1"));
	CHECK(config._isValidIpPort("localhost:") == false);
}
