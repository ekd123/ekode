/* -*- Mode: vala; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*-  */
/*
 * biligo.vala
 * Copyright (C) 2015 Ma Kai <crtmike@hotmail.com>
 * 
 * biligo is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * biligo is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

using GLib;
using Gtk;

public class Main : Object {
	Grid layout;
	Entry url;
	Window window;
	Label info;
	const string bilidan_exe = "bilidan.py";

	public Main () {
		window = new Window();
		info = new Label("Type the URL and press Enter.");
		layout = new Grid();
		url = new Entry();
	}

	public void run() {
		window.title = "Biligo";
		window.border_width = 10;
		window.window_position = WindowPosition.CENTER;
		layout.row_spacing = 5;
		layout.attach(info, 0, 0, 1, 1);
		layout.attach(url, 0, 1, 1, 1);
		window.add(layout);
		url.key_press_event.connect(on_key_press);
		window.destroy.connect(on_destroy);
		window.show_all();
	}

	public bool on_key_press (Gdk.EventKey event) {
		if (event.keyval == Gdk.Key.Return) {
			window.sensitive = false;
			string[] spawn_args = {};
			spawn_args += bilidan_exe;
			spawn_args += url.get_text();
			Pid child_pid;
			int errfd;
			try {
				Process.spawn_async_with_pipes("/", spawn_args, 
					Environ.get(), SpawnFlags.SEARCH_PATH 
					| SpawnFlags.DO_NOT_REAP_CHILD, null, out child_pid, 
					null, null, out errfd);
				ChildWatch.add(child_pid, (pid, status) => {
					Process.close_pid(pid);
					url.set_text("");
					window.sensitive = true;
				});
				IOChannel errchan = new IOChannel.unix_new(errfd);
				errchan.add_watch(IOCondition.IN | IOCondition.HUP, 
					(channel, condition) => {
						string? e;
						try {
							channel.read_line(out e, null, null);
						} catch (Error e) {
							stderr.printf("%s\n", e.message);
							Process.abort();
						}
						if(e == null)
							return false;
						else if (!e.has_prefix("ERROR:"))
							return true;
						var dlg = new MessageDialog(window, 
							Gtk.DialogFlags.MODAL, Gtk.MessageType.ERROR, 
							Gtk.ButtonsType.OK, "%s", e);
						dlg.run();
						dlg.destroy();
						return false;
					});
			} catch (SpawnError e) {
				var dlg = new MessageDialog(window, 
					Gtk.DialogFlags.MODAL, Gtk.MessageType.ERROR, 
					Gtk.ButtonsType.OK, "%s", e);
				dlg.run();
				dlg.destroy();
			}
			return true;
		}
		return false;
	}

	public void on_destroy (Widget window) {
		Gtk.main_quit();
	}

	static int main (string[] args) {
		Gtk.init(ref args);
		var app = new Main();
		app.run();
		Gtk.main();
		return 0;
	}
}
