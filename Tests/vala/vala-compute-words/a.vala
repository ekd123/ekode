void main () {
	var file = File.new_for_path ("/usr/share/dict/words");
	try {
		var dis = new DataInputStream (file.read ());
		string line;
		while ((line = dis.read_line (null, null)) != null) {
			int sum = 0;
			string dup = line.dup();
			dup.down ();
			for (int i = 0; i < line.length; i ++) {
				if (!dup[i].isalpha())
					continue;
				sum += line[i] - 'a';
			}
			if (sum == 100)
				stdout.printf (@"$line\n");
		}
	} catch (Error e) {
		error (e.message);
	}
}
