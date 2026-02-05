dbus-update-activation-environment --systemd DISPLAY XAUTHORITY XDG_CURRENT_DESKTOP

/usr/lib/xdg-desktop-portal-gtk &
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &

wal -R
xrdb -merge ~/.Xresources

feh --bg-fill $(cat ~/.cache/wal/wal)
picom &
dunst &
dwmblocks &

greenclip daemon &
xset s 300 &
xss-lock -l -- slock &
