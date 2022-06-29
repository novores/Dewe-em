#!/bin/dash

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors
. ~/deweem/dwmstatus/gruvbox

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)

  printf "^c$black^^b$green^ "CPU" ^d^%s" "^c$white^^b$grey^ $cpu_val "
}

# pkg_updates() {
#   updates=$(doas xbps-install -un | wc -l) # void
#   # updates=$(pacman -Qu | wc -l)   # arch
#   # updates=$(aptitude search '~U' | wc -l)  # apt (ubuntu,debian etc)
#
#   if [ -z "$updates" ]; then
#     printf "^c$green^  Fully Updated"
#   else
#     printf "^c$green^  $updates"" updates"
#   fi
# }

battery() {
  get_capacity="$(cat /sys/class/power_supply/BAT0/capacity)"
  printf "^c$green^ ^d^%s" "^c$green^ $get_capacity "
}

brightness() {
  get_brightness=$(xbacklight -get)
  printf "^c$red^  ^d^%s" "^c$red^$get_brightness%"
}

mem() {
  get_mem=$(free -h | awk '/^Mem/ { print $3 }' | sed s/i//g)
  printf "^c$blue^^b$black^  ^d^%s" "^c$blue^$get_mem "
}

wlan() {
  get_ssid=$(wpa_cli status | awk NR==4 | cut -c 6-20)
	case "$(cat /sys/class/net/wl*/operstate 2>/dev/null)" in
    up) printf "^c$black^^b$blue^ 󰤨 ^d^%s" "^c$blue^ $get_ssid" ;;
    down) printf "^c$black^^b$blue^ 󰤭 ^d^%s" "^c$blue^ Disconnect" ;;
	esac
}

clock() {
	printf "^c$black^^b$darkblue^ 󱑆 ^d^%s" "^c$black^^b$blue^ $(date '+%H:%M') "
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

while true; do

  # [ $interval = 0 ] || [ $(($interval % 3600)) = 0 ] && updates=$(pkg_updates)
  # interval=$((interval + 1))

  sleep 1 && xsetroot -name "$(volume) $(brightness) $(cpu) $(mem) $(wlan) $(battery) $(clock)"
done
