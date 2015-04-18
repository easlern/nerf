# nerf
Microcontroller interface for Raspberry Pi &lt;-> RF transceiver (PIC18LF2525/NRF24L01+)

This thing allows you to get a Raspberry Pi (or another uC, technically) talking to an NRF24L01+ to send/receive basic messages. No star topology or whatever, fancy stuff like that is not supported right now.
Technically, this project does not need to exist because you could just attach the NRF to the Raspberry Pi and skip the middle-man. But I already had the uC/NRF code working so I figured I'd just leverage that and add a simple Pi/uC protocol instead of porting all that uC/NRF code I had to Raspberry Pi. So now we've got this uC that sits in the middle: Pi<->uC<->NRF .

This will probably work with any PIC with a decent number of GPIO pins. Four of your pins go to the NRF and three more go to the Pi though, so keep that in mind.

You'll probably find it ludicrous that I'm bit-banging stuff by hand instead of using hardware SPI. I don't really have an excuse for that except that I thought it would be more fun to do it that way.
