
# Comparation

## Raw Modules
![Raw](./.img/esp8266RawdComp.png)

## Development Modules
![Dev](./.img/esp8266DevdComp.png)

## Overview
|    | ESP8266EX | ESP-01 | D1 Mini | NodeMCU |
| :--: | :---: | :---: | :---: | :---: |
| **Packaging** | chip | plug-in | board | board |
| **Cost** | ￥3.5 | ￥9.4 | ￥15.8 | ￥22 |
| **Pins** | 14GPIO | 2GPIO+tx+rx | 9Dig+1Ana | 10Dig+2Ana |
| **Dependence** | 12R/L/C+1CRY+1SPI Flash | LD1117/AMS1117 | - | - |
| **Power Supply** | 2.5V-3.6V(Const) | 3.3V(Const) | 5V/MicroUSB | 4.5V-10V/MicroUSB |
| **Flash** | - | 512K/1M | 4M | 4M |
| **Deep Sleep** |  Good | Good | Mid | Bad |
| **OTA** | Need Flash > 1M | - | Support | Support |
| **Lua** | - | - | Support | Support |
| **Arduino IDE** | - | Support | Support | Support |

# ESP8266 Reading Material
 - [ESP8266 Data Sheet](./.spec/esp8266ex_en.pdf)
 - [ESP8266 Technical Reference](http://espressif.com/sites/default/files/documentation/esp8266-technical_reference_en.pdf)
 - [ESP8266 Geting Start Guide](http://espressif.com/sites/default/files/documentation/2a-esp8266-sdk_getting_started_guide_en.pdf)

# Specification

## ESP8266EX

### Picture
![ESP8266EX](./.img/ESP8266EX.png)

### Spec
 - Price: ￥3.5
 - Data Sheet [English](./.spec/esp8266ex_en.pdf) / [Chinese](./.spec/esp8266ex_cn.pdf)


## ESP-01
### Picture
![ESP01](./.img/esp01.png)

### Spec
 - price: ￥9.4
 - Support Arduino IDE
 - [Data Sheet](./.spec/esp01.pdf)
![](./.img/ESP01_spec.png)


### issues
 - 3.3v Power Supply need to be precise
 - New Version with 1M flash but hard to deep sleep

## Wemos D1 Mini

### Picture

![](./.img/d1mini.png)

### Spec
 - Price: ￥15.8
 - Support Arduino IDE
 - Support OTA
 - [Docs](https://wiki.wemos.cc/products:d1:d1_mini#documentation)
![](./.img/d1mini_spec.png)


### NodeMCU

### Picture
![](./.img/nodeMCU.png)

### Spec
 - Price: ￥22
 - [Docs](https://nodemcu.readthedocs.io/en/master/)