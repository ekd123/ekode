require 'gir_ffi'
GirFFI.setup :Gtk
Gtk::init []
win = Gtk::Window.new 0
win.title = "Hello world!"
win.has_resize_grip = true
win.show_all
win.signal_connect "delete-event" do |userdata| Gtk::main_quit end
Gtk::main
