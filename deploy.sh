#!/bin/bash

( mkdir -p build; cd build
sudo apt install -y libpcre3-dev build-essential cmake libssl-dev
cmake -DBUILD_DRIVER=TRUE -DRADIUS=FALSE ..
make
sudo make install
)
sudo cp contrib/accel-ppp.conf /etc/
sudo cp contrib/accel-ppp.service /etc/systemd/system/
sudo fgrep 'test * 123456' /etc/ppp/chap-secrets || echo 'test * 123456 *' | sudo tee -a /etc/ppp/chap-secrets
sudo systemctl daemon-reload
sudo systemctl restart accel-ppp
sudo systemctl status accel-ppp
sudo iptables -t nat -C POSTROUTING -j MASQUERADE -s 192.168.0.0/24 || sudo iptables -t nat -I POSTROUTING -j MASQUERADE -s 192.168.0.0/24
while ! ip --br a show ppp0; do sleep 1; done
sudo ip r add 192.168.0.0/24 dev ppp0 table wan_routable
