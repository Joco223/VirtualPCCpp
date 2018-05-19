# VirtualPCCpp

This is a simulation of a fully fledged PC. It includes the following:

-Custom CPU architecture designed by me with an assembly language, all info in Text_Files/Assembly Documentation.txt

-Custom GPU arhcitecture designed by me with an assembly language, all info in Text_Files/GPU Assembly.Documentation.txt

-Fully working permanent storage (referred as HDD in vPC code), currently 16MB but may be expanded in the future

-Ram and vRam sizes of 16MB

-Interrupts, current are keyboard input and network packages

-A screen (aka the window) of size 640x480 which supports 106x60 8bit color character space and 24bit color for individual pixels

-Full networking system. Works on the base of a central vPC Server where vPCs connect to and the server gives each vPC a unique address    which can be used by other vPCs to send messages to. Messages consist of senders address (first 3 bytes) and 5 bytes of additional data where you can put anything, more info in Assembly Documentation.txt. Network system works on port 52239 in case port forwarding is needed.


There are no stupid suggestions, feel free to message me and suggest what you think should be added. Have fun programming! :)

Discord server invite for anyone who wants to suggest stuff, report bugs, show-off programs etc: https://discord.gg/bN8jy2X
