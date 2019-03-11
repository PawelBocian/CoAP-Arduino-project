# CoAP-Arduino-project

* Introduction
* Project architecture
* Description
* Manual
* Example of work

## Introduction
---
Project was made by Wojciech Jenczalik,Łukasz Michowski and Paweł Bocian.  
Hello, I am glad to see you reading this file. Project was created using Constrained Application Protocol (CoAP) and Arduino while studying at Warsaw University of Technology. Aspect of radio communication is explained in another [repo](https://github.com/PawelBocian/Arduino-nRF24L01-Radio-Communication-project) on my github. I am aware that code style isn't clean but I did as much as possible to make it understandable for all of you.
Any questions send via mail at : **boocianpawel@gmail.com**

![](readme_images/readme_introduction.png)

## Project architecture
---
Elements included in project architecture :
* Client CoAP – plugin Copper in Mozilla Firefox browser.
* Server CoAP implemented on device Arduino UNO 
* Connected with Arduino Mini Pro buzzer and potentiometer.

![](readme_images/readme_architecture.png)

### Components and interfaces

System contains Arduino Mini Pro, Buzzer, Potentiometer, Converter 5V -> 3.3V and [nRF24L01](https://github.com/PawelBocian/Arduino-nRF24L01-Radio-Communication) radio module.

![](readme_images/readme_miniSystem.png)

System contains Arduino UNO and [nRF24L01](https://github.com/PawelBocian/Arduino-nRF24L01-Radio-Communication) radio module.

![](readme_images/readme_Unosystem.png)

### Resources

#### Buzzer 
Project handles request GET and PUT. Using it, client received actual value of frequency in Hertz. Using PUT request client can set rectangular signal generated with 50% fill and demanding frequency. 

#### Potentiometer
Project handles GET request on resource. Client received then AnalogRead value from pin when Potentiometer is attached.

## Description 
---
All code is endowed with comments, describing classes objects and methods in project. 
CoAP is part implemented in this project. Functionality implemented :
* Sending request from Cooper `GET, PUT and CoAP PING`
* Receiving and handling responses from `CoAP server`. 
* Handling message `NON` (GET in case potentiometer and GET,PUT in case of buzzer) 
* Handling message `CON` (GET in case potentiometer and GET,PUT in case of buzzer) 
* Handling returning `diagnostics payloads` fx. „ Request type not supported at this Uri-Path” 
* Handling options `Content-Format, Uri-Path, Accept` 
* Content-Format: option number `12`, to define format of payloads - available options: 
    -text/plain (id=0) 
    -application/link-format (id=40) 
    -application/json (id=50) 
* Handling `Token and MID Token` – adjusts response type to request Message ID – id to correlate ACK i RST with original message and 
`detection duplicates`
* Handling mechanism of `rejecting unimplemented options` along with diagnostic payload fx. for Proxy-Uri – „Proxying not supported”  
* Handling message `discover` 
* Returning `metrics regarding the radio connection` between Arduino Uno and Mini Pro


## Manual
---
If you want to use this code u need to set correct MAC adress and your ethernet ip adress. You can find it using for example [WireShark](https://www.wireshark.org/).  
To learn more about CoAP protocol read [this](https://tools.ietf.org/html/rfc7252).

## Example of work

Example scenario:
1. We start with a ping operation.
2. We perform discover
3. We call the discover with the Accept option: 40 (Link-format)
4. We go to the metrics (Uri-Path)
5. We are attempting to execute the POST command
6. We perform the GET command
7. We perform the GET command with the Accept: 50 (JSON) option

![](readme_images/example_of_work.png)
