/usr/lib/xdg-desktop-portal-gtk &
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &

xrdb -merge ~/.Xresources

feh --bg-fill $(cat ~/.cache/cwal/cwal)
sxhkd &
picom &
dunst &
dwmblocks &
emacs --daemon &

greenclip daemon &

awake off
