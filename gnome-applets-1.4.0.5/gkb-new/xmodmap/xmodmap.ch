! Converted keytable file to xmodmap file
! with mk_modmap by root@chanae.alphanet.ch vie nov 27 02:11:59 CET 1998
clear Mod1
clear Mod2
!  Swiss keyboard
!charset "iso-8859-1"
keycode   9 = Escape Escape
keycode  10 = 1 plus bar
keycode  11 = 2 quotedbl at
keycode  12 = 3 asterisk numbersign
keycode  13 = 4 ccedilla
keycode  14 = 5 percent
keycode  15 = 6 ampersand notsign
keycode  16 = 7 slash bar
keycode  17 = 8 parenleft cent
keycode  18 = 9 parenright
keycode  19 = 0 equal
keycode  20 = apostrophe question dead_acute
keycode  21 = dead_circumflex dead_grave dead_tilde
keycode  22 = BackSpace Delete
keycode  23 = Tab Tab
keycode  24 = q
keycode  25 = w
keycode  26 = e E EuroSign
keycode  27 = r
keycode  28 = t
keycode  29 = z
keycode  30 = u
keycode  31 = i
keycode  32 = o
keycode  33 = p
keycode  34 = udiaeresis egrave bracketleft
keycode  35 = dead_diaeresis exclam bracketright
keycode  36 = Return
keycode  37 = Control_L
keycode  38 = a
keycode  39 = s
keycode  40 = d
keycode  41 = f
keycode  42 = g
keycode  43 = h
keycode  44 = j
keycode  45 = k
keycode  46 = l
keycode  47 = odiaeresis eacute
keycode  48 = adiaeresis agrave braceleft
keycode  49 = section degree
keycode  50 = Shift_L
keycode  51 = dollar sterling braceright
keycode  52 = y
keycode  53 = x
keycode  54 = c
keycode  55 = v
keycode  56 = b
keycode  57 = n
keycode  58 = m
keycode  59 = comma semicolon
keycode  60 = period colon Multi_key
keycode  61 = minus underscore
keycode  62 = Shift_R
keycode  63 = KP_Multiply
keycode  64 = Alt_L Meta_L
keycode  65 = space space
keycode  66 = Caps_Lock
keycode  67 = F1 F11
keycode  68 = F2 F12
keycode  69 = F3 F13
keycode  70 = F4 F14
keycode  71 = F5 F15
keycode  72 = F6 F16
keycode  73 = F7 F17
keycode  74 = F8 F18
keycode  75 = F9 F19
keycode  76 = F10 F20
keycode  77 = Num_Lock
keycode  78 = Scroll_Lock
keycode  79 = KP_7
keycode  80 = KP_8
keycode  81 = KP_9
keycode  82 = KP_Subtract
keycode  83 = KP_4
keycode  84 = KP_5
keycode  85 = KP_6
keycode  86 = KP_Add
keycode  87 = KP_1
keycode  88 = KP_2
keycode  89 = KP_3
keycode  90 = KP_0
keycode  94 = less greater backslash
keycode  95 = F11 F11
keycode  96 = F12 F12
keycode 108 = KP_Enter
keycode 109 = Control_R
keycode 112 = KP_Divide
keycode 113 = Mode_switch
keycode 114 = Break
keycode 110 = Find
keycode  98 = Up
keycode  99 = Prior
keycode 100 = Left
keycode 102 = Right
keycode 115 = Select
keycode 104 = Down
keycode 105 = Next
keycode 106 = Insert
! right windows-logo key
! in "windows" keyboards the postion of the key is annoying, is where AltGr
! usually resides, so go definie it as AltGr
keycode 116 = Mode_switch
! right windows-menu key
keycode 117 = Multi_key
!
add Mod1 = Alt_L
add Mod2 = Mode_switch
