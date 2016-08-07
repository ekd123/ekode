require 'gir_ffi'
GirFFI.setup :Gdk
Gdk.init []
puts Gdk::Keymap.get_default.get_caps_lock_state

