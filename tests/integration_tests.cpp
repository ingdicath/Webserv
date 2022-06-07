//
// Created by Diani on 05/06/2022.
//

#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "Configurator.hpp"

//TEST_CASE("Out of range port values", "[Config file]"){
//	Configurator config;
//	REQUIRE(config._isValidPortRange("0") == false );
//	REQUIRE(config._isValidPortRange("65536") == false );
//	REQUIRE(config._isValidPortRange("13246832000") == false );
//}

//TEST_CASE("Incorrect port values", "[Config file]"){
//	Configurator config;
//	REQUIRE(config._isValidPortRange("-0") == false );
//	REQUIRE(config._isValidPortRange("hola") == false );
//	REQUIRE(config._isValidPortRange("800+") == false );
//}

//TEST_CASE("Correct port values", "[Config file]"){
//	Configurator config;
//	REQUIRE(config._isValidPortRange("1") == true );
//	REQUIRE(config._isValidPortRange("65535") == true );
//	REQUIRE(config._isValidPortRange("8000") == true );
//}

// If exception is found, exit code is 0 for this tests.
TEST_CASE("Out of range port values", "[Config file]") {
	Configurator config;
	CHECK_THROWS(config._checkPortRange("0"));
	CHECK_THROWS(config._checkPortRange("65536"));
}

TEST_CASE("Incorrect port values", "[Config file]") {
	Configurator config;
	CHECK_THROWS(config._checkPortRange("-152"));
	CHECK_THROWS(config._checkPortRange("hola"));
}

TEST_CASE("Correct port values", "[Config file]") {
	Configurator config;
	INFO("Correct port values");
	CHECK_NOTHROW(config._checkPortRange("1"));
	CHECK_NOTHROW(config._checkPortRange("65535"));
}
