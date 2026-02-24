/usr/lib/xdg-desktop-portal-gtk &
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &

wal -R
xrdb -merge ~/.Xresources

feh --bg-fill $(cat ~/.cache/wal/wal)
sxhkd &
picom &
dunst &
dwmblocks &
emacs --daemon &

greenclip daemon &

awake off
