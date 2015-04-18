# nerf
Microcontroller interface for Raspberry Pi &lt;-> RF transceiver (PIC18LF2525/NRF24L01+)

This thing allows you to interface a Raspberry Pi (or another uC, technically) with an NRF24L01+ to send/receive basic messages. No star topology or whatever, fancy stuff like that is not supported right now.
Technically, this project does not need to exist because usually you just attach the NRF to the Raspberry Pi and skip the middle-man. But I already had the uC/NRF code working so I figured I'd just leverage that and add a simple Pi/uC protocol instead of porting all that uC/NRF code I had to Raspberry Pi.
