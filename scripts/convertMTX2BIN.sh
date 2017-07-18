#!/bin/bash
for i in ../matricesMTX/*; do echo $i; filename=$(basename "$i"); filename="${filename%.*}"; ../sparse-tools/build/mtx2bin/mtx2bin $i $filename.bin; done
