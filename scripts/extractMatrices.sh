#!/bin/bash
for i in ../matricesArch/*.tar.gz; do filename=$(basename "$i");filename="${filename%.*.*}"; tar zxvf $i $filename/$filename.mtx; done
