/usr/libexec/lxqt-policykit-agent &
wal -R
xrdb -merge ~/.Xresources

feh --bg-fill $(cat ~/.cache/wal/wal)
picom &
dunst &
dwmblocks &

greenclip daemon &
xset s 300 &
xss-lock -l -- slock &
