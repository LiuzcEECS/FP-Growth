# FP-Growth
A simple FP-Growth Project
# Data format
Two data format is supported.
1. ".txt" file: every line is a transcation, separated by space.
2. ".csv" file: every line except the first is a transcation, separated by comma.

You can find some samples in src folder.
# How to use it?
## 1. UI
The Qt Project is in the `FP_dev` folder of `Qt` branch.
You can also download the excuable file from [Dropbox][1].
## 2. Build from source
```
$ cd src
$ g++ -std=c++11 main.cpp -o main.o
$ ./main.o path_to_data support confidence
```

# Contributing
Bug reports and pull requests are welcome on Github at https://github.com/LiuzcEECS/FP-Growth

[1]: https://www.dropbox.com/s/qxq5g133nojtzol/FP_boxed.exe?dl=0
