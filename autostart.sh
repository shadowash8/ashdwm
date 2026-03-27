/usr/lib/xdg-desktop-portal-gtk &
/usr/lib/polkit-gnome/polkit-gnome-authentication-agent-1 &

xrdb -merge ~/.Xresources

setxkbmap us -variant colemak_dh
feh --bg-fill $(cat ~/.cache/ashwal/ashwal)
sxhkd &
picom &
dunst &
dwmblocks &
emacs --daemon &

greenclip daemon &

awake off
