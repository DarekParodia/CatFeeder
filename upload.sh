#!/bin/bash
avrdude -c usbasp -p m328p -U flash:w:build/firmware.hex:i -F