#!/bin/bash
g++ depth-stream.cpp -lrealsense2 `pkg-config --libs opencv` `pkg-config --cflags opencv` -o depth-stream

