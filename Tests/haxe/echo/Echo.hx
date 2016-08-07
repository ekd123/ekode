class Echo {
	static function main () {
		for (arg in Sys.args()) {
			Sys.print (arg + " ");
		}
		Sys.println ("");
	}
}
