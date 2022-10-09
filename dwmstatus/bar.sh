#!/bin/dash

# ^c$var^ = fg color
# ^b$var^ = bg color

# load colors
. ~/Developments/deweem/dwmstatus/gruvbox

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)
  printf "^c$black^^b$green^ "CPU" ^d^%s" "^c$white^^b$grey^ $cpu_val ^d^"
}

battery() {
  get_capacity="$(cat /sys/class/power_supply/BAT0/capacity)"
  printf "^c$green^ ^d^%s" "^c$green^ $get_capacity "
}

brightness() {
  get_brightness=$(xbacklight -get | cut -d '.' -f 1)
  printf "^c$red^  ^d^%s" "^c$red^$get_brightness%"
}

mem() {
  get_mem=$(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)
  printf "^c$blue^  ^d^%s" "^c$blue^$get_mem "
}

 wlan() {
  #  #iwd
  # get_ssid=$(iwctl station wlan0 show | awk 'NR==7 {print $3}')
  get_ssid=$(iwctl station wlan0 show | awk '/network/ {print $3}')
   # get_ssid=$(wpa_cli status | awk NR==4 | cut -c 6-20) #wpa_supplicant
 	case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
     up) printf "^c$black^^b$blue^ 直 ^d^%s" "^c$blue^ $get_ssid" ;;
     down) printf "^c$black^^b$blue^ 睊 ^d^%s" "^c$blue^ Disconnect" ;;
 	esac
 }

clock() {
	printf "^c$darkblack^^b$white^  ^d^%s" "^c$white^^b$darkblack^ $(date '+%_a,%e %H:%M') "
}

volume() {
  vol=$(pamixer --get-volume)
  state=$(pamixer --get-mute)

  if [ "$state" = "true" ] || [ "$vol" -eq 0 ]; then
    printf " 婢 "
  else
    printf "^c$green^墳 %s" "^c$green^$vol%"
  fi
}

mpd() {
  printf "^b$yellow^^c$black^ MPD ^d^"
  get_artist=$(mpc -f "%artist%"| awk NR==1 | cut -c 1-30)
  get_title=$(mpc -f "%title%"| awk NR==1 | cut -c 1-30)
  if pgrep mpd > /dev/null; then
    case "$(mpc status -f %state% | awk NR==2 | cut -c-9)" in
      "[playing]") printf "^b$grey^  ^d^%s" "^b$grey^^c$yellow^$get_artist^c$white^ $get_title ^d^" 
      ;;
      "[paused] ") printf "^b$grey^  ^d^%s" "^b$grey^^c$yellow^$get_artist^c$white^ $get_title ^d^" 
      ;;
      *) printf "^b$grey^ Stopped ^d^" 
      ;;
    esac
  else
    printf "^b$grey^^c$white^ Offline ^d^"
  fi
}

while true; do
  sleep 1 && xsetroot -name "$(mpd) $(volume) $(brightness) $(cpu) $(mem) $(wlan) $(battery) $(clock)"
done
