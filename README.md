# Saleae To Hexdump

This program looks to convert a Saleae logic analyzer I2C decoded output and turn it into a hexdump format.
This allows for the IPMB protocol to be decoded using the great tool Wireshark

```bash
mkdir build && cd build && cmake ../
./saleae_to_hex -f <file-from-termial-of-i2c>
text2pcap -t "%D %H:%M:%S." -l 209 hexdump.hex ipmbfile.pcap
```
