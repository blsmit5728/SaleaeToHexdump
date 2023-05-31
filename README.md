# Saleae To Hexdump

This program looks to convert a Saleae logic analyzer I2C decoded output and turn it into a hexdump format.
This allows for the IPMB protocol to be decoded using the great tool Wireshark

```bash
mkdir build && cd build && cmake ../
text2pcap -t "%D %H:%M:%S." -l 209 hexdump.hex ipmbfile.pcap
```

## How to setup wireshark for decoding

[Setting up Wireshark](https://ask.wireshark.org/question/18644/how-do-i-get-it-to-decode-the-ipmbipmb_tracedpcap-example-as-ipmi/)
