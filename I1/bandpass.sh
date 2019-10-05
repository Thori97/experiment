#!/bin/bash
sox doremi.wav -t raw -c 1 -| ./bandpass 8192 300 3000