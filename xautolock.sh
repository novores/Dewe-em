#!/bin/bash
xautolock \
  -time 5 -locker slock -killtime 30 \
  -killer "loginctl suspend" -notify 30 \
  -notifier "notify-send 'NOTICE' 'will be locked in 30s'"  \
  -detectsleep &
