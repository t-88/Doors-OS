# Doors OS

OS am devloping from scratch to understand low lvl programming and sys dev   
i dont want it to be a fad, so it will take as long as it needs to take

## Goals
after reading the [beginner mistakes](https://wiki.osdev.org/Beginner_Mistakes) and testing a bit in my [leanring repo](https://github.com/t-88/Code-Bongo-III/tree/main/os-dev), i knew i needed a end goal and i should do it the right way   
- [x] getting keyboad input 
- [x] mouse Input 
- [x] VGA driver 
- [x] building a repl (read excute print loop) 
- [ ] making some game 
and the most important goal is to learn and explore     

## How To Run
-   install nasm => ```apt install nasm```
-   install qemu => ``` apt install qemu-system-i386```
-   run make


## Personal Notes
-   day and a half for not copying code right, in idt the low then the hight addr, fuck that too long, dang it.
-   \[2023-09-04]: better understanding for booting, int 0x13 


## Dev Imgs
### Memory Maping
![mmap](./dev-imgs/memory_map.png)
### Terminal Achived
![basic terminal overview](./dev-imgs/terminal_overview.gif)
#### Mouse Driver
![mouse_driver](./dev-imgs/mouse_driver.gif)

### Kinda Funny
#### Printing Inside Print :O
![calling_printf_inside_printf](./dev-imgs/calling_printf_inside_printf.png)
#### Printing Os Memery
![printing_os_mem](./dev-imgs/printing_os_memory.gif)

## Credits
-   ctr-c ctr-v from [cfenollosa os dev](https://github.com/cfenollosa/os-tutorial/tree/master)
-   ofc [os-dev](https://wiki.osdev.org/Expanded_Main_Page) 

### License
[![MIT License](https://img.shields.io/badge/License-MIT-green.svg)](https://choosealicense.com/licenses/mit/)