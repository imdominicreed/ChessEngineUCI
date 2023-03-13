#!/bin/bash

./cutechess-cli -engine cmd=./engines/past name=past -engine cmd=./engines/curr name=curr -each proto=uci tc=0/60 -rounds 1 -pgnout output_pgn_file.pgn