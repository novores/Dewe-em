#!/bin/dash

# ^c$var^ = fg color
# ^b$var^ = bg color

interval=0

# load colors
. ~/Developments/deweem/dwmstatus/gruvbox

cpu() {
  cpu_val=$(grep -o "^[^ ]*" /proc/loadavg)
  printf "^c$black^^b$green^ "CPU" ^d^%s" "^c$white^^b$grey^ $cpu_val "
}

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
	printf "^c$black^^b$darkblue^ 󱑆 ^d^%s" "^c$black^^b$blue^ $(date '+%_a,%e %H:%M') "
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
  printf "^b$darkyellow^^c$black^ MPD ^d^"
  get_artist=$(mpc -f "%artist%"| awk NR==1 | cut -c 1-30)
  get_title=$(mpc -f "%title%"| awk NR==1 | cut -c 1-30)
  if pgrep mpd > /dev/null; then
    [ "$get_title" = "volume: n/a   repeat: off   ra" ] && printf "^b$grey^ Stopped ^d^" || printf "^b$grey^^c$yellow^ $get_artist ^d^%s" "^b$grey^^c$white^ $get_title ^d^"
  else
    printf "^c$grey^^b$black^ Offline ^d^"
  fi
}

while true; do
  sleep 1 && xsetroot -name "$(mpd) $(volume) $(brightness) $(cpu) $(mem) $(wlan) $(battery) $(clock)"
done
