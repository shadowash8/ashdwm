dbus-update-activation-environment --systemd DISPLAY XAUTHORITY XDG_CURRENT_DESKTOP

/usr/lib/xdg-desktop-portal &
/usr/lib/xdg-desktop-portal-lxqt &
/usr/bin/lxqt-policykit-agent &

wal -R
xrdb -merge ~/.Xresources

feh --bg-fill $(cat ~/.cache/wal/wal)
picom &
dunst &
dwmblocks &

emacs --daemon &
greenclip daemon &
xset s 300 &
xss-lock -l -- slock &
