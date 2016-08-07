import sys.FileSystem;

class Demo {
	static function getHome () : String {
		return Sys.getEnv ("HOME");
	}

	static function main () {
		Sys.println ("Create..");
		FileSystem.createDirectory (getHome() + "/test");
		Sys.println ("Sleeping..");
		Sys.sleep (10);
		Sys.println ("Delete..");
		FileSystem.deleteDirectory (getHome() + "/test");
	}
}
