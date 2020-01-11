cmake_minimum_required(VERSION 3.0)

project(Decision)

set(DECISION_HEADERS
		src/AttributeTree.h
	)

set(DECISION_SOURCES
		src/Main.cpp
		src/AttributeTree.cpp
	)

add_executable(Decision ${DECISION_HEADERS} ${DECISION_SOURCES})
set_property(TARGET Decision PROPERTY CXX_STANDARD 11)