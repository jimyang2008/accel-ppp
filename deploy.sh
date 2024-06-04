#!/bin/bash

( mkdir -p build; cd build
sudo apt install -y libpcre3-dev build-essential cmake libssl-dev
cmake -DBUILD_DRIVER=TRUE -DRADIUS=FALSE ..
make
sudo make install
)
sudo cp contrib/accel-ppp.conf /etc/
sudo cp contrib/accel-ppp-ip-up /etc/ppp/ip-up.d/
sudo cp contrib/accel-ppp-ip-down /etc/ppp/ip-down.d/
sudo cp contrib/accel-ppp.service /etc/systemd/system/
sudo fgrep 'test * 123456' /etc/ppp/chap-secrets || echo 'test * 123456 *' | sudo tee -a /etc/ppp/chap-secrets
sudo systemctl daemon-reload
sudo systemctl restart accel-ppp
sudo systemctl status accel-ppp

