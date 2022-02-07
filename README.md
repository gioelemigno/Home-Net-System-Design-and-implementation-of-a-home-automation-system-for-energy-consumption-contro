# Abstract 
The project developed is part of the development of automation systems for energy
consumption control in home and small business environments, to cut energy supply
costs by reducing the duration of overloads, carrying out a targeted disconnection
of low priority loads. This technique is called load management and requires the
installation of a home automation system equipped with smart sockets, that can
share real-time energy consumption and allow the detachment of the load via remote
control. In order to do that, I dealt with the design and implementation of a home
automation system (Home Net System) designed to be installed in an existing
electrical system, developing devices that can be mounted directly inside wall outlets.
These devices are based on the Atmega328P microcontroller and communicate within
a room via a RS485 bus. Inside each room there is a supervisor based on an ARM
microprocessor that allows the management of the devices by a local controller
through WiFi network, in particular by MQTT protocol. During the tests carried
out through the realization of a prototype, the response times obtained are in order
of hundreds of milliseconds, via local control, and times of about a second with
remote control managed by a server. The timing obtained allows effective load
management so as to reduce the duration of overloads and prevent the disconnection
of the overall plant by energy meter.
