# CoAP-Arduino-project

* Introduction
* Description
* Manual
* Example of work

## Introduction
---
Project was made by Wojciech Jenczalik,Łukasz Michowski and Paweł Bocian.
Hello, I am glad to see you reading this file. Project was created using Constrained Application Protocol (CoAP) and Arduino while studying at Warsaw University of Technology. Aspect of radio communication is explained in another [repo](https://github.com/PawelBocian/Arduino-nRF24L01-Radio-Communication-project) on my github. I am aware that code style isn't clean but I did as much as possible to make it understandable for all of you.
Any questions send via mail at : **boocianpawel@gmail.com**


## Description 
---
All code is endowed with comments, describing classes objects and methods in project. 
CoAP is part implemented in this project. Functionality implemented :
* Sending request from Cooper `GET, PUT and CoAP PING`
* Setting options `Content-Format, Uri-Path, Accept`
* Receiving and handling responses from `CoAP server`.

##Manual
---
If you want to use this code u need to set correct MAC adress and your ethernet ip adress. You can find it using for example [WireShark](https://www.wireshark.org/).
To learn CoAP protocol read [this](https://tools.ietf.org/html/rfc7252).

