import sys.io.File;

class Cat {
	static public function main () {
		// read all:
		// var content = File.getContent (Sys.args()[0]);
		// Sys.println (content);

		// read line:
		try {
			var file = File.read(Sys.args()[0]);
			while (true) {
				try {
					Sys.println (file.readLine ());
				}
				catch (e : Dynamic) {
					break;
				}
			}
		}
		catch (e : Dynamic) {
			Sys.println ("CAN'T OPEN SUCH FILE");
		}
	}
}
