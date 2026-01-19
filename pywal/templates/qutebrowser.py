palette = {{
    "background": "{background}",
    "background_alt": "{color0}",
    "background_selected": "{color8}",
    "background_dim": "{color0}",

    "foreground": "{foreground}",
    "foreground_muted": "{color7}",

    "red": "{color1}",
    "orange": "{color3}",
    "yellow": "{color3}",
    "green": "{color2}",
    "blue": "{color4}",
    "magenta": "{color5}",
    "cyan": "{color6}",
    "teal": "{color6}",

    "black": "{background}",
}}

# --- Completion ---
c.colors.completion.fg = palette["foreground"]
c.colors.completion.odd.bg = palette["background_alt"]
c.colors.completion.even.bg = palette["background"]
c.colors.completion.category.fg = palette["blue"]
c.colors.completion.category.bg = palette["background_alt"]
c.colors.completion.category.border.top = palette["black"]
c.colors.completion.category.border.bottom = palette["black"]
c.colors.completion.item.selected.fg = palette["cyan"]
c.colors.completion.item.selected.bg = palette["background_selected"]
c.colors.completion.item.selected.border.top = palette["background_selected"]
c.colors.completion.item.selected.border.bottom = palette["background_selected"]
c.colors.completion.item.selected.match.fg = palette["orange"]
c.colors.completion.match.fg = palette["yellow"]
c.colors.completion.scrollbar.fg = palette["foreground"]
c.colors.completion.scrollbar.bg = palette["background"]

# --- Context Menu ---
c.colors.contextmenu.disabled.bg = palette["background_alt"]
c.colors.contextmenu.disabled.fg = palette["foreground_muted"]
c.colors.contextmenu.menu.bg = palette["background"]
c.colors.contextmenu.menu.fg = palette["foreground"]
c.colors.contextmenu.selected.bg = palette["background_selected"]
c.colors.contextmenu.selected.fg = palette["foreground"]

# --- Statusbar ---
c.colors.statusbar.normal.fg = palette["green"]
c.colors.statusbar.normal.bg = palette["background"]
c.colors.statusbar.insert.fg = palette["background"]
c.colors.statusbar.insert.bg = palette["blue"]
c.colors.statusbar.passthrough.fg = palette["background"]
c.colors.statusbar.passthrough.bg = palette["teal"]
c.colors.statusbar.command.fg = palette["foreground"]
c.colors.statusbar.command.bg = palette["background"]
c.colors.statusbar.url.fg = palette["foreground"]
c.colors.statusbar.url.error.fg = palette["red"]
c.colors.statusbar.url.hover.fg = palette["cyan"]
c.colors.statusbar.url.success.http.fg = palette["green"]
c.colors.statusbar.url.success.https.fg = palette["blue"]

# --- Tabs ---
c.colors.tabs.bar.bg = palette["background"]
c.colors.tabs.odd.fg = palette["foreground"]
c.colors.tabs.odd.bg = palette["background_selected"]
c.colors.tabs.even.fg = palette["foreground"]
c.colors.tabs.even.bg = palette["background_selected"]
c.colors.tabs.selected.odd.fg = palette["foreground"]
c.colors.tabs.selected.odd.bg = palette["background_dim"]
c.colors.tabs.selected.even.fg = palette["foreground"]
c.colors.tabs.selected.even.bg = palette["background_dim"]

# --- Messages ---
c.colors.messages.error.fg = palette["red"]
c.colors.messages.error.bg = palette["background"]
c.colors.messages.warning.fg = palette["orange"]
c.colors.messages.warning.bg = palette["background"]
c.colors.messages.info.fg = palette["cyan"]
c.colors.messages.info.bg = palette["background"]
