STA + optical flow
================

Implementation of STA descriptor presented in "Histogram-Based Description of Local Space-Time Appearance": 
http://www.zemris.fer.hr/~ssegvic/pubs/brkic11scia.pdf

Example usage
-----

    git clone git://github.com/srdanrasic/optical_flow_sta.git
    cd optical_flow_sta
    cmake .
    make
    vim example_kth.py  # configure dataset directory path
    ./example_kth.py


Requirements
-----
* OpenCV 2.4.5
* CMake
