cmake_minimum_required(VERSION 3.0)

project(Decision)

set(DECISION_HEADERS
		src/Decision.h
		src/ArgumentParsing.h
		src/AttributeTree.h
		src/DecisionTree.h
		src/ForwardList.h
		src/BinarySearchTree.h
		src/BasicTree.h
	)

set(DECISION_SOURCES
		src/Main.cpp
		src/Decision.cpp
		src/ArgumentParsing.cpp
		src/DecisionTree.cpp
	)

add_executable(Decision ${DECISION_HEADERS} ${DECISION_SOURCES})
set_property(TARGET Decision PROPERTY CXX_STANDARD 11)