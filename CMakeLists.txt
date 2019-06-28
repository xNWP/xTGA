# CMakeLists.txt

cmake_minimum_required(VERSION 3.0)

project(xTGA	VERSION 100
				DESCRIPTION "A C/C++ library for opening/saving/manipulating TrueVision Targa (TGA) Images."
				LANGUAGES CXX)
				
set(XTGA_VERSION ${PROJECT_VERSION})

add_subdirectory(xTGA)