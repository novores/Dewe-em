# Personal DWM build
![screenshot](screenshot/screenshot.png) 
## Introduction
This is my build dwm in version `6.3` with several patching including:
- alwayscenter
- cool-autostart
- fadeinactive
- fullgaps
- hide_vacant_tags
- moveresize
- status2d
- statuspadding
## Dependency
- `gcc` (for compiling)
### Optional 
- `feh` (setting wallpaper)
- `xbanish` (for autohide cursor when typing)
- `polkit-mate` (polkit gui)
- `xautolock` (to trigger lockscreen)
- `xcompmgr` (compositor)
## Installation
  ```bash
  git clone https://github.com/novores/dewe-em`
  cd dewe-em/dwm && sudo make install`
  ```
##Start DWM
1. copy this script, put in `~/.local/bin/` and naming it "startdwm. 
  ```bash
  #/bin/sh
  while true; do
    /usr/local/bin/dwm 2> ~/.dwm.log
  done
  ```
2. put following to `~/.xinitrc`
```bash
exec dbus-run-session ~/.local/bin/startdwm
```
3. exec `startx` on TTY
